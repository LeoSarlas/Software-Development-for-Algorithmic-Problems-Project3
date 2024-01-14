#include <cstdlib>    // Standard library for general-purpose functions and memory management
#include <string>     // String manipulation functions and classes
#include <vector>     // Container class for dynamic arrays
#include <iostream>   // Input/output stream objects and functions
#include <fstream>    // Input/output stream classes for file handling
#include <cmath>      // Mathematical functions and operations
#include <climits>    // Constants specifying the limits of integral types
#include <algorithm>  // Algorithms for working with sequences of elements
#include <cstring>    // C-style string manipulation functions
#include <chrono>     // To measure and calculate the duration of each algorithm's execution
#include <iterator>

#define norm 2				// L2 norm in this case
using namespace std;

struct image{
	vector<int> cords;
	int ro;
	int co;
	int img_id;
};

struct hashtable_node{
	int key;
	vector<struct image*> labels;
	vector<int> id;
	int num_img;
};

typedef struct hashtable_node *hashtable_nodePtr;

struct hashtable_head{
	hashtable_nodePtr table;
	int buckets;
	int numberOfVectors;
};

typedef struct hashtable_head *HashTable;

struct h_function{
	double *v; // v ~ N (0, 1)^d
	double t; // t variable uniformly ~ R [0, w)
};

struct temp_nums{
	int index;
	int id;
	double dist;
};

struct g_function{
	h_function *h_functions;
	int *r;
	unsigned int m;
};