#include "methods_and_entities_declarations.h"

//----------------------------------------------Functions prototypes for 'helping_functions.cpp'-----------------------------------------------------------

// Function to reverse the endianness of a 32-bit integer
int reverseInt(int value);

// Function that reads the .conf file and acquires parameter values
Configuration readConfiguration(const string& configFile);

// Function to calculate the L2 norm of 2 vectors
double calculateL2Norm(const MNISTImage& dataPoint, const MNISTImage& centroids);

// Function to read MNIST images and labels from IDX files
void readMNISTImages(const string& imagesFile, vector<MNISTImage>& mnistData);

// Function to initialize cluster centroids using K-Means++
void initializeCentroids(const vector<MNISTImage>& mnistData, int k, vector<MNISTImage>& centroids);

// Function to perform the MacQueen update step for K-Means
void macQueenUpdate(vector<MNISTImage>& centroids, vector<MNISTImage>& mnistData, int clusters);

// Function to calculate the distance from a point to the centroid of a cluster
double calculateCentroidDistance(const vector<MNISTImage>& cluster, const MNISTImage& point);

// Function to calculate the Silhouette score
double calculateSilhouetteScore(const vector<MNISTImage>& mnistData, const vector<int>& clusterAssignments);

// Function to calculate the size of a cluster
int clusterSize(const vector<MNISTImage>& mnistData, int clusterIndex);

// Function that calculates the centroid coordinates of a cluster
MNISTImage calculateClusterCentroid(const vector<MNISTImage>& cluster);

// Function to calculate the silhouette score for a specific cluster
double silhouetteScorePerCluster(const vector<int>& clusterAssignments, int clusterIndex, const vector<MNISTImage>& mnistData);

// Function to check if a file exists
bool fileExists(const string& filename);

// Function to delete a file
bool deleteFile(const string& filename);