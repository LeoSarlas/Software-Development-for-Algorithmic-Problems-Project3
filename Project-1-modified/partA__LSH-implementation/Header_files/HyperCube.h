#include "HashMap.h"
#include "Hashtable.h"

struct cubeNode{
	h_function *h_functions;
	HashMap *f_funs;
	HashTable hypercube;
};

bool compareMyObjects(const temp_nums& a, const temp_nums& b);

int binaryArrayToDecimal(int *s,int size);

double distance_metric(vector<int> *v1,vector<int> *v2,int d);

double normalRandom();

double uniform_distribution(int rangeLow, int rangeHigh);

void generateH_Cube(h_function *hfun);

void initializeHyperCube(int k,cubeNode *hc);

double dot_product(double *v, vector<int> cords,int d);

int computeH_Cube(h_function hfun,image *im);

int computeF(HashMap f_fun,int key);

void insertToHyperCube(cubeNode *hc,image *v);

void insertFromListToHyperCube(image *img_list,cubeNode *hc ,int img_num);

void HammingDistance(cubeNode *hc,image *img_list,int *index,int hammingDist,int startFrom,vector<temp_nums> *sort_ims,int *numOfSearched,int m,int n,int *nodesSearched,int probes,int q);

void query(cubeNode *hc,image *query_list,image *img_list,int n,double radius,int hammingDist,int m,int q,char *output,int img_num,int count);

image* file_manip(char *fname,int *image_num);