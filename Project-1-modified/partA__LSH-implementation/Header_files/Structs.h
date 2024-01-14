#include <cstdlib> 
#include <string>
#include <vector>
#include<iostream>
#include<fstream>
#include<cmath>
#include<climits>
#include <algorithm>
#include <cstring>
#include <ctime>


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

