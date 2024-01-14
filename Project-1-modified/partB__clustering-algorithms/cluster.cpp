// |_______________________________________________________________________________________| 
// |                                Project 2023-2024 (1)                                  |
// |                                                                                       |
// |                     Σαρλάς Λεωνίδας-Μάρκελλος (ΑΜ 1115201700135)                      |
// |                     Βεκρής Δημήτριος-Αλέξανδρος (ΑΜ 1115202000021)                    |
// |                                                                                       |
// |_______________________________________________________________________________________|


#include "methods_and_entities_declarations.h"
#include "helping_functions.h"

//  ./cluster -i input.idx3-ubyte -c cluster.conf -o Classic_output_file.txt -m Classic -complete
//  Classic_output_file.txt


//  ./cluster -i input.idx3-ubyte -c cluster.conf -o LSH_output_file.txt -m LSH -complete
//  LSH_output_file.txt


//  ./cluster -i input.idx3-ubyte -c cluster.conf -o Random_projection_output_file.txt -m Hypercube -complete
//  Random_projection_output_file.txt


// main program
int main(int argc, char* argv[]) {
    
    string inputFile, configFile, outputFile, method;
    bool complete = false;
    
    // Loop through command-line arguments
    for (int i = 1; i < argc; i++) {
        string arg = argv[i];

        if (arg == "-i" && i + 1 < argc) {
            inputFile = argv[i + 1];
            i++;
        } else if (arg == "-c" && i + 1 < argc) {
            configFile = argv[i + 1];
            i++;
        } else if (arg == "-o" && i + 1 < argc) {
            outputFile = argv[i + 1];
            i++;
        } else if (arg == "-complete") {
            complete = true;
        } else if (arg == "-m" && i + 1 < argc) {
            method = argv[i + 1];
            i++;
        }
    }

    // Check if required options are missing
    if (inputFile.empty() || configFile.empty() || outputFile.empty() || method.empty()) {
        cerr << "Usage: ./cluster -i <input file> -c <configuration file> -o <output file> -m <method: Classic or LSH or Hypercube> [-complete]" << endl;
        return 1;
    }

    // Verification of terminal input
    cout << "Input File: " << inputFile << endl;
    cout << "Configuration File: " << configFile << endl;
    cout << "Output File: " << outputFile << endl;
    cout << "Method: " << method << endl;

    // Create the configuration values from the .conf file
    Configuration config = readConfiguration(configFile);

    vector<MNISTImage> mnistData;

    // Read MNIST images and labels
    readMNISTImages(inputFile, mnistData);

    int k = config.numberOfClusters; // Number of clusters from the .conf file

    // Execute the appropriate calculation method depending on the user's choice
    if (method == "Classic") {

        cout << "Executing Lloyd's algorithm..." << endl;
        lloydsKMeansClustering(mnistData, k, complete);

    } else if (method == "LSH") {
        
        cout << "Executing Reverse Search algorithm by LSH..." << endl;
        reverseSearchLSH(mnistData, k, complete, config);

    } else if (method == "Hypercube") {

        cout << "Executing Reverse Search algorithm by Random Projection (Hypercube)..." << endl;
        reverseSearchRandomProjection(mnistData, k, complete, config);

    } else {
        // Error handle
        cerr << "Invalid method. Choose Classic, LSH, or Hypercube." << endl;

        return 1;
    }

    // Termination
    cout << "Program terminated." << endl;
    return 0;
}
