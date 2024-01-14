#include "helping_functions.h"

//--------------------------------------Functions prototypes for Reverse Serach by LSH method-------------------------------------------

// Function to initialize LSH data structures and hash MNIST images
void initializeLSH(vector<MNISTImage>& mnistData, vector<LSHBucket>& lshTable, int numBuckets, int numHashFunctions);

// Function that hashes MNIST images into LSH buckets
unsigned int hashLSH(const MNISTImage& image, int numBuckets, int hashFunctionIndex);
