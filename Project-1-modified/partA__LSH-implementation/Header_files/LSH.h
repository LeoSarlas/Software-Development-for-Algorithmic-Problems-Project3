#include "Hashtable.h"

struct lshNode{
	int l;
	g_function *g_fun; 
	HashTable *hts; 
}; 

bool compareMyObjects(const temp_nums& a, const temp_nums& b);

double distance_metric(vector<int> *v1,vector<int> *v2,int d);

double rand_gen();

double normalRandom();

double uniform_distribution(int rangeLow, int rangeHigh);

void generateH(h_function *hfun);

void generateG(g_function *gfun);

void initializeLSH(int l,lshNode *lsh);

double dot_product(double *v, vector<int> cords,int d);

int computeH(h_function hfun,image *im);

temp_nums computeG(g_function gfun,image *p);

void insertToLSH(lshNode *lsh,image *v);

void insertFromListToLSH(image *img_list,lshNode *lsh,int img_num);

void query(lshNode *lsh,image *query_list,image *img_list,int n,double radius,int q,char *output,int img_num,int count);

image* file_manip(char *fname,int *image_num);