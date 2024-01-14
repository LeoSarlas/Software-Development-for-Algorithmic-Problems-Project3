<!-- PROJECT LOGO -->
<br />
<div align="center">
    <img src="resources/logo.png" alt="Logo" width="170" height="170">
    <br />
  </a>

  <h3 align="center">Software Development for Algorithmic Problems (K23γ) | Project 2023-2024</h3>
  <h3 align="center">part B</h3>
  
  <p align="center">
    Vectors searching & clustering
    <br />
  </p>
</div>

## Introduction

This is the part B of algorithmic implementations. It refers to vectors clustering, using Lloyd's algorithm and Reverse Search approaches such as LSH logic and random projection through the Hypercube concept.

## Files & functions used

Below lies a concise description of the files and functions used in this part of the assignment 1.

<br><br>

```c++
cluster.cpp
```

It is the main source code file that includes the main() function. It contains the implementation of a primary interface and basic status printings in the terminal to confirm the parameter values from the configuration file and to print the method of computing the clusters we have provided as a parameter in the execution command.

<br><br>

    cluster.conf
    
This is the parameter definition file, which is subsequently used by the three different cluster-finding algorithms (Classic Lloyd's, Reverse Search by LSH, Reverse Search by Random Projection (Hypercube)). In case a parameter does not have a predefined value, it is assigned a default value by the program itself. This automatic assignment is done in the `readConfiguration()` function, located in the source file `helping_functions.cpp`.

<br><br>

```c++
helping_functions.cpp
```
It includes the implementations of all the auxiliary functions used by the 3 cluster calculation algorithms. These functions, briefly, are as follows:

<br><br>
1.
```c++
reverseInt(int value);
```
&emsp;Performs endianess conversion to ensure proper reading of data from the MNIST dataset.

<br><br>
2.
```c++
readConfiguration(const string& configFile);
 ```
&emsp;Reads the values of parameters from the .conf file and assigns default values if any parameter is uninitialized.

<br><br>
3.
```c++
double calculateL2Norm(const MNISTImage& dataPoint, const MNISTImage& centroids);
```
&emsp;Calculates the L2 norm of two vectors (in this context, two MNIST images).

<br><br>
4.
```c++
void readMNISTImages(const string& imagesFile, vector<MNISTImage>& mnistData);
```
&emsp;Reads the MNIST dataset, which contains 10,000 handwritten digit images from 0 to 9. It reads and interprets the header section of the dataset to extract information such as the number of images, image dimensions in pixels, and a magic number to ensure the file's format is correct and not corrupted.

<br><br>
5.
```c++
void initializeCentroids(const vector<MNISTImage>& mnistData, int k, vector<MNISTImage>& centroids);
```
&emsp;Initializes the centroids of the clusters using the K-means++ method.

<br><br>
6.
```c++
void macQueenUpdate(vector<MNISTImage>& centroids, vector<MNISTImage>& mnistData, int k);
```
&emsp;Assigns each data point (image) to a cluster by calculating the L2 distance between them. The data point is assigned to the centroid (cluster) with the minimum distance. After calculating the centroids, the cluster points are updated.

<br><br>
7.
```c++
double calculateCentroidDistance(const vector<MNISTImage>& cluster, const MNISTImage& point);
```
&emsp;Calculates the L2 distance of a data point (image) from a cluster centroid by using the `calculateL2Norm` function.

<br><br>
8.
```c++
double calculateSilhouetteScore(const vector<MNISTImage>& mnistData, const vector<int>& clusterAssignments);
```
&emsp;Computes the Silhouette score of the clustering results from each algorithm. The Silhouette score indicates the quality of the clustering.

<br><br>
9.
```c++
int clusterSize(const vector<MNISTImage>& mnistData, int clusterIndex);
```
&emsp;Computes the size (number of data points) of a cluster, which is used in the Silhouette score calculation.

<br><br>
10.
```c++
MNISTImage calculateClusterCentroid(const vector<MNISTImage>& cluster);
```
&emsp;Calculates the coordinates of the centroid of a cluster. This function is used in the reverse search by LSH algorithm.

<br><br>
11.
```c++
double silhouetteScorePerCluster(const vector<int>& clusterAssignments, int clusterIndex, const vector<MNISTImage>& mnistData);
```
&emsp;Computes the Silhouette score of an individual cluster, which is used for printing in the results/log text file after the program's execution.

<br><br>
12.
```c++
bool fileExists(const string& filename);
```
&emsp;Checks if a file (in this case, a .txt file) exists, and if it does, deletes it and creates a new log file. Specifically, if it detects an older log file from the same algorithm, it deletes it and creates a new one.

<br><br>

```c++
helping_functions.h
```
It includes the prototypes of the functions implemented in `helping_functions.cpp`.

<br><br>

```c++
methods_and_entities_declarations.h
```
It includes all the C++ library inclusions needed for the overall implementation, struct declarations, and the prototypes of the functions corresponding to the three clustering algorithms (Classic Lloyd's, Reverse Search by LSH, Reverse Search by Random Projection (Hypercube)). These functions are as follows:

<br><br>
1.
```c++
void lloydsKMeansClustering(vector<MNISTImage>& mnistData, int k, bool complete);
```
&emsp;Implements the classic Lloyd's algorithm with initialization of centroids using Kmeans++ and cluster update according to MacQueen's method.

<br><br>
2.
```c++
void reverseSearchLSH(vector<MNISTImage>& mnistData, int k, bool complete, Configuration config);
```
&emsp;Implements the Reverse Search by LSH algorithm with centroid initialization and data point assignment according to LSH buckets.

<br><br>
3.
```c++
void reverseSearchRandomProjection(vector<MNISTImage>& mnistData, int k, bool complete, Configuration config);
```
&emsp;Implements the Reverse Search by Random Projection algorithm with centroid initialization by Kmeans++ and cluster updates using the MacQueen logic. The assignment of data points to clusters is performed with a 4-dimensional Hypercube (default is 3 dimensions).

<br><br>

```c++
reverse_search_lsh.h
```
It includes the prototypes of the functions called by the Reverse Search by LSH algorithm. Their implementations can be found in the `reverse_search_lsh.cpp` source file.

<br><br>

```c++
lloyds.cpp
```
A function that implements the Lloyd's algorithm, calling the necessary functions as mentioned. Further comments can be found in the code file.

<br><br>

```c++
reverse_search_lsh.cpp
```
A function that implements the Reverse Search by LSH algorithm, calling the necessary functions as mentioned. Further comments can be found in the code file.

<br><br>

```c++
reverse_search_random_projection.cpp
```
A function that implements the Reverse Search by Random Projection (Hypercube) algorithm, calling the necessary functions as mentioned. Further comments can be found in the code file.

<br>

## Compilation and execution

- Using the `make cluster` command, linking and compilation of all source files and header files are performed.

- With the command `./cluster -i input.idx3-ubyte -c cluster.conf -o Classic_output_file.txt -m Classic -complete`, the Lloyd's algorithm is executed for clustering with command-line parameters that configure the execution parameters of the functions and the output.

- With the command `./cluster -i input.idx3-ubyte -c cluster.conf -o LSH_output_file.txt -m LSH -complete`, the Reverse Search by LSH algorithm is executed for clustering with command-line parameters that configure the execution parameters of the functions and the output.

- With the command `./cluster -i input.idx3-ubyte -c cluster.conf -o Random_projection_output_file.txt -m Hypercube -complete`, the Reverse Search by random projection (Hypercube) algorithm is executed for clustering with command-line parameters that configure the execution parameters of the functions and the output.

In each case, the last parameter `[-complete]` determines whether full results will be printed to the output file (i.e., all points in each cluster).

<br>

## Results and output files

For each clustering algorithm, a corresponding log file in .txt format is generated in the program's directory. Specifically, for each of the 3 cases, you will find one of the following 3 log files:

* Classic_output_file.txt

* Reverse_search_LSH_output_file.txt

* Random_projection_output_file.txt

The printing format in these log files adheres to the requirements specified in Assignment 1.





