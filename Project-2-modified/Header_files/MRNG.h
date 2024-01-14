#include "Structs.h"

 //----------------------------------------------------LSH----------------------------------------------------------------

// Structure representing a node in the Locality-Sensitive Hashing (LSH)
struct lshNode {
    int l;               // Number of hash functions
    g_function *g_fun;   // Array of g functions
    HashTable *hts;      // Array of hash tables
};

// Function to insert images into the hash table along with their IDs
void htInsert_LSH(HashTable ht, image *v,int index,int id);

// Function to compare two objects based on their distances
bool compareMyObjects(const temp_nums& a, const temp_nums& b);

// Function to compare two objects based on their distances
auto compareMyObjects2(const temp_nums& a, const temp_nums& b);

// Function to calculate the Euclidean distance between two vectors
double distance_metric(vector<int> *v1, vector<int> *v2, int d);

// Function to generate a uniformly distributed random value
double rand_gen();

// Function to generate a normally distributed random value
double normalRandom();

// Function to generate a hash function for the LSH
void generateH(h_function *hfun);

// Function to generate a g function for the LSH
void generateG(g_function *gfun);

// Function to initialize the LSH
void initializeLSH(int l, lshNode *lsh);

// Function to calculate the dot product between two vectors
double dot_product(double *v, vector<int> cords, int d);

// Function to compute the value of the h function for a given vector
int computeH(h_function hfun, image *im);

// Function to compute the value of the g function for a given vector
temp_nums computeG(g_function gfun, image *p);

// Function to insert a vector into the LSH
void insertToLSH(lshNode *lsh, image *v);

// Function to insert a list of vectors into the LSH
void insertFromListToLSH(image *img_list, lshNode *lsh, int img_num);

// Function to perform a query using LSH
vector<temp_nums> query(lshNode *lsh, image *query_list, image *img_list, int q);

// Function to manipulate files and read images
image* file_manip(char *fname, int *image_num);

//------------------------------------------------GNNS - MRNG--------------------------------------------------------


#include <numeric>          // For numeric algorithms 
#include <unordered_set>    // Manipulation of unordered data sets
#include <vector>
#include <chrono>   

using namespace std;

// Data structure to represent a node in the adjacency list
struct Graph_Node {
    int index;
    vector<int> neighbors;
    int marked;
	image img;
};

// Structure to represent an edge in the MRNG graph
struct MRNGEdge {
    int from_index;
    int to_index;
    double distance; // distance between 'from_index' and 'to_index'
};

// Struct to store the different times of calculation
struct avg_time{
    double Approx;
    double True;
    double MAF;
};

// Function to calculate the L2 norm of 2 vectors
double calculateDistance(const image& dataPoint, const image& centroid, int p);

// Function to calculate the centroid of the dataset
image calculateCentroid(const vector<image>& images);

// Function to construct MRNG (Minimum Redundancy Neighbor Graph) using adjacency lists
vector<Graph_Node> constructMRNGGraph(const vector<image>& images);

// Function to search for the nearest neighbor using brute force
vector<int> search_NN_of_NavNode(const vector<image>& images, const image& queryPoint, int k);

// Function to search for N nearest neighbors on the MRNG graph using the concept of Navigating Node
avg_time searchOnGraph(int l,int Neigh,vector<Graph_Node>& adjacencyList,vector<image>& q_images, vector<image>& v_images,int count,char* output);

// Function to check if a file exists
bool fileExists(const string& filename);

// Function to delete a file
bool deleteFile(const string& filename);


// Struct representing a neighboring node in the GNNS algorithm
struct GNNS{
	vector<int> img_ids;
	int marked;
};

//Function that initializes GNNS graph
GNNS* initGNNS(lshNode *lsh,image *img_list,int k,int image_num);

//Function that searches in GNNS graph for NNs
avg_time searchGNNS(int k,int e,int r,int n,GNNS *graph,image *query_list, image *img_list, int q,int image_num,int count,char* output);