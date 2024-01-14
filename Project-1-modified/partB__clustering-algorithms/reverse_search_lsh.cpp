#include "Header_files/methods_and_entities_declarations.h"
#include "Header_files/helping_functions.h"
#include "Header_files/reverse_search_lsh.h"

// Function to perform reverse search using LSH and MacQueen update
void reverseSearchLSH(vector<MNISTImage>& mnistData, int k, bool complete, Configuration config) {

    auto start = chrono::high_resolution_clock::now();
    int numBuckets = config.numberOfVectorHashTables;
    int numHashFunctions = config.numberOfVectorHashFunctions;
    int img = 0;

    vector<LSHBucket> lshTable;
    initializeLSH(mnistData, lshTable, numBuckets, numHashFunctions);

    vector<vector<MNISTImage>> clusters(k);
    vector<int> clusterAssignments(mnistData.size(), -1); // Initialize cluster assignments

    // Initialize cluster centroids by selecting k random data points
    vector<MNISTImage> clusterCentroids;
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> dis(0, mnistData.size() - 1);

    for (int i = 0; i < k; ++i) {

        int randomIndex = dis(gen);
        clusters[i].push_back(mnistData[randomIndex]);
        clusterCentroids.push_back(mnistData[randomIndex]);
        clusterAssignments[randomIndex] = i;
    }

    for (MNISTImage& queryImage : mnistData) {

        unordered_set<MNISTImage> candidates;

        for (int i = 0; i < numHashFunctions; i++) {

            int bucketIndex = hashLSH(queryImage, numBuckets, i);
            candidates.insert(lshTable[bucketIndex].images.begin(), lshTable[bucketIndex].images.end());
        }

        // Find the nearest centroid and assign the point to the cluster
        int bestCluster = -1;
        double bestDistance = numeric_limits<double>::max();

        for (int clusterIndex = 0; clusterIndex < k; clusterIndex++) {

            double clusterDistance = calculateCentroidDistance(clusters[clusterIndex], queryImage);

            if (clusterDistance < bestDistance) {

                bestDistance = clusterDistance;
                bestCluster = clusterIndex;
            }
        }

        // Assign the point to the best cluster
        clusters[bestCluster].push_back(queryImage);
        clusterAssignments[queryImage.index] = bestCluster;
    }

    // Calculate the new cluster centroids
    for (int i = 0; i < k; ++i) {

        clusterCentroids[i] = calculateClusterCentroid(clusters[i]);
    }

    double silhouetteScore = calculateSilhouetteScore(mnistData, clusterAssignments);

    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::seconds>(end - start);

    const string outputFileName = "Reverse_search_LSH_output_file.txt";

    if (fileExists(outputFileName)) {

        if (!deleteFile(outputFileName)) {

            return;
        }
    }

    ofstream output("Reverse_search_LSH_output_file.txt");

    if (!output.is_open()) {

        cerr << "Error: Could not open the output file." << endl;
        return;
    }

    output << "Algorithm: Reverse Search LSH" << endl;

    for (int clusterIndex = 0; clusterIndex < k; clusterIndex++) {

        output << "CLUSTER-" << clusterIndex + 1 << " {size: " << clusters[clusterIndex].size() << ", centroid: ";

        // Print centroid coordinates here
        const MNISTImage& centroid = clusterCentroids[clusterIndex];  // Use the calculated centroids

        output << "[";

        for (int j = 0; j < centroid.data.size(); j++) {

            // Check if the value is a valid numerical value (between 0 and 255)
            if (centroid.data[j] >= 0 && centroid.data[j] <= 255) {

                // Cast the uint8_t value to an integer
                int numericValue = static_cast<int>(centroid.data[j]);
                output << numericValue;

                if (j < centroid.data.size() - 1) {

                    output << ", ";
                }
            }
        }
        output << "]}" << endl;
    }

    output << "clustering_time: " << duration.count() << " seconds" << endl;

    output << "Silhouette: [";

    for (int clusterIndex = 0; clusterIndex < k; clusterIndex++) {

        double s_i = silhouetteScorePerCluster(clusterAssignments, clusterIndex, mnistData);
        output << s_i;

        if (clusterIndex < k - 1) {

            output << ", ";
        }
    }

    output << ", " << silhouetteScore << "]\n";

    if (complete) {

        for (int clusterIndex = 0; clusterIndex < k; clusterIndex++) {

            output << "CLUSTER-" << clusterIndex + 1 << " {centroid, ";

            for (const MNISTImage& image : clusters[clusterIndex]) {

                output << "image_" << image.index << ", ";
            }

            output << "}" << endl;
        }
    }

    cout << "You can find the results in 'Reverse_search_LSH_output_file.txt'" << endl;

    output.close();
}



// Function to initialize LSH data structures and hash MNIST images
void initializeLSH(vector<MNISTImage>& mnistData, vector<LSHBucket>& lshTable, int numBuckets, int numHashFunctions) {

    // Initialize the LSH data structures, such as lshTable
    lshTable.resize(numBuckets);

    // Perform LSH hashing for each MNIST image
    int y=0;

    for (MNISTImage& image : mnistData) {

        y++;

        for (int i = 0; i < numHashFunctions; i++) {

            // Hash the image using LSH and place it in the corresponding bucket            
            int bucketIndex = hashLSH(image, numBuckets, i);
            lshTable[bucketIndex].images.push_back(image);
        }
    }
}


// Function to hash an MNISTImage using LSH
unsigned int hashLSH(const MNISTImage& image, int numBuckets, int hashFunctionIndex) {

    const vector<uint8_t>& data = image.data;
    unsigned int hash = 5381; // Initial hash value

    for (double value : data) {

        // Update the hash with the feature values
        hash = ((hash << 5) + hash) + static_cast<unsigned int>(value);
    }

    // Apply a hash function specific to the hashFunctionIndex
    return hash % static_cast<unsigned int>(numBuckets);
}
