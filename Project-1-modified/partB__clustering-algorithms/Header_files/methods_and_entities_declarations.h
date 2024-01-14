#ifndef B_DECLARATIONS_H
#define B_DECLARATIONS_H
#define SIZER 1            // Helper constant for manipulating the number of the images in the dataset we like to read by partitioning it.
#include <iostream>        // Standard input/output for displaying information.
#include <vector>          // To use vector for data storage and manipulation.
#include <string>          // For working with strings or file paths.
#include <fstream>         // For file input/output operations.
#include <random>          // To generate random numbers when needed.
#include <algorithm>       // Utilized for various algorithmic operations (e.g., sorting).
#include <ctime>           // For time-related functions and timestamping.
#include <unordered_map>   // To efficiently store key-value pairs in a hash map.
#include <unordered_set>   // For fast lookup of unique elements in a set.
#include <chrono>          // To measure and calculate the duration of each algorithm's execution.
#include <sstream>         // For parsing strings in the .conf file.
#include <cstdint>         // For reading and manipulating integers in the MNIST dataset.
#include <queue>           // For storing and finding 'top-n' values in a struct quickly.
using namespace std;       // Simplifies access to standard C++ library components, avoiding use of "std::..."

//--------------------------------------------Entities and structs declarations-----------------------------------------------------

// Define a struct to represent MNIST images
struct MNISTImage {
    int label;
    int size = 784;
    int cluster;
    int index;
    vector<uint8_t> data; 
    
    // Define an equality operator to compare MNISTImage objects based on their data
    bool operator==(const MNISTImage& other) const {
        return data == other.data;
    }
};

// Custom hash function for MNISTImage
namespace std {
    template <>
    struct hash<MNISTImage> {
        size_t operator()(const MNISTImage& image) const {           
            return hash<int>{}(image.label);
        }
    };
}

// Define a struct to represent LSH bucket
struct LSHBucket {
    vector<MNISTImage> images;
};

// Struct that stores the values read from the .conf file
struct Configuration {
    int numberOfClusters;
    int numberOfVectorHashTables;
    int numberOfVectorHashFunctions;
    int maxNumberMHypercube;
    int numberOfHypercubeDimensions;
    int numberOfProbes;
};

//----------------------------------------------Functions prototypes-----------------------------------------------------------

// Function to perform Lloyd's K-Means clustering
void lloydsKMeansClustering(vector<MNISTImage>& mnistData, int k, bool complete);

// Function to perform reverse search using LSH
void reverseSearchLSH(vector<MNISTImage>& mnistData, int k, bool complete, Configuration config);

// Function to perform reverse search using random projection
void reverseSearchRandomProjection(vector<MNISTImage>& mnistData, int k, bool complete, Configuration config);

#endif