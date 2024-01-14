#include <iostream>
#include <fstream>
#include <vector>

const int NUM_IMAGES = 100;
const int IMAGE_SIZE = 28 * 28; // 28x28 images
using namespace std;

struct Image {
    unsigned char label;
    vector<unsigned char> pixels;
};

void readMNIST(const string& imagesPath, const string& labelsPath, vector<Image>& images) {
    ifstream imagesFile(imagesPath, ios::binary);
    ifstream labelsFile(labelsPath, ios::binary);

    if (!imagesFile.is_open() || !labelsFile.is_open()) {
        cerr << "Error opening files." << endl;
        return;
    }

    // Read image file header
    imagesFile.seekg(16);
    int numImages;
    imagesFile.read(reinterpret_cast<char*>(&numImages), sizeof(numImages));
    numImages = __builtin_bswap32(numImages);

    // Read labels file header
    labelsFile.seekg(8);

    // Read images and labels
    for (int i = 0; i < NUM_IMAGES; ++i) {
        Image img;
        labelsFile.read(reinterpret_cast<char*>(&img.label), sizeof(img.label));

        for (int j = 0; j < IMAGE_SIZE; ++j) {
            unsigned char pixel;
            imagesFile.read(reinterpret_cast<char*>(&pixel), sizeof(pixel));
            img.pixels.push_back(pixel);
        }

        images.push_back(img);
    }

    imagesFile.close();
    labelsFile.close();
}

void writeMNISTIdx3(const string& filename, const vector<Image>& images) {
    ofstream file(filename, ios::binary);
    cout << "test" << endl;
    int magicNumber = __builtin_bswap32(2051); // 2051 is the magic number for idx3-ubyte
    int numImages = __builtin_bswap32(images.size());
    int numRows = __builtin_bswap32(28);
    int numCols = __builtin_bswap32(28);

    file.write(reinterpret_cast<const char*>(&magicNumber), sizeof(magicNumber));
    file.write(reinterpret_cast<const char*>(&numImages), sizeof(numImages));
    file.write(reinterpret_cast<const char*>(&numRows), sizeof(numRows));
    file.write(reinterpret_cast<const char*>(&numCols), sizeof(numCols));

    for (const auto& image : images) {
        file.write(reinterpret_cast<const char*>(&image.label), sizeof(image.label));
        file.write(reinterpret_cast<const char*>(image.pixels.data()), IMAGE_SIZE);
    }

    file.close();
}

int main() {

    vector<Image> mnistData;
    readMNIST("train-images.idx3-ubyte", "train-labels.idx1-ubyte", mnistData);

    // Save the modified dataset in idx3-ubyte format
    writeMNISTIdx3("mnist_subset_100.idx3-ubyte", mnistData);

    cout << "Subset of MNIST dataset with the first 100 images saved to mnist_subset.idx3-ubyte" << std::endl;

    return 0;
}
