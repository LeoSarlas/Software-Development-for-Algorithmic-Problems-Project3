#include "HyperCube.h"

using namespace std;

int d;
int k;
int m;
int probes;
int w;

int main(int argc, char *argv[]){
	k=14;
	int m=10;
  	int n=1;
  	int r=10000;
  	int probes=2;
	char inputf[1000];
	char path[1000];
	char output[1000];
	char r_inputf[1000];
	char r_path[1000];
	for(int i=1; i<argc; i+=2){
		if(strcmp(argv[i],"-d")==0){
			strcpy(inputf,argv[i+1]);
		}
		if(strcmp(argv[i],"-q")==0){
			strcpy(path,argv[i+1]);
		}
		if(strcmp(argv[i],"-k")==0){
			k = atoi(argv[i+1]);
		}
		if(strcmp(argv[i],"-M")==0){
			m = atoi(argv[i+1]);
		}
		if(strcmp(argv[i],"-probes")==0){
			probes = atoi(argv[i+1]);
		}
		if(strcmp(argv[i],"-o")==0){
			strcpy(output,argv[i+1]);
		}
		if(strcmp(argv[i],"-N")==0){
			n = atoi(argv[i+1]);
		}
		if(strcmp(argv[i],"-R")==0){
			r = atoi(argv[i+1]);
		}
		if(strcmp(argv[i],"-rd")==0){
			strcpy(r_inputf,argv[i+1]);
		}
		if(strcmp(argv[i],"-rq")==0){
			strcpy(r_path,argv[i+1]);
		}
	}
	int image_n;
	image *images = file_manip(inputf,&image_n);
	image *r_images = file_manip(r_inputf,&image_n);
	cubeNode hc;
	cubeNode r_hc;
	int d1 =  images[0].ro*images[0].co;
	int d2 =  r_images[0].ro*r_images[0].co;	
	w =32;
	d=d1;
	initializeHyperCube(k,&hc);
	insertFromListToHyperCube(images,&hc,image_n);
	d=d2;
	initializeHyperCube(k,&r_hc);
	insertFromListToHyperCube(r_images,&r_hc,image_n);
	int count =0;
	int repeat =1;
	do{
		int t;
		d=d1;
		image *queries = file_manip(path,&t);
		d=d2;
		image *r_queries = file_manip(r_path,&t);
		int q = rand()%t;
		query(&hc,queries,images,n,r,probes,m,q,output,image_n,count);
		query(&r_hc,r_queries,r_images,n,r,probes,m,q,output,image_n,count);
		//train-images.idx3-ubyte
		char answer;
		cout << "Do you want to repeat the process? (y/n): ";
		cin >> answer;
		if (answer != 'y' && answer != 'Y') {
			repeat = 0;
		}
		if(repeat){
			cout << "Enter the path of query file: ";
			delete[] queries;
			delete[] r_queries;
			cin >>path;
			cout << "Enter the path of reduced query file: ";
			cin >>r_path;
		}
		count++;
	}while (repeat);
	for(int i=0;i<k;i++){
		delete[] hc.h_functions[i].v;
		delete[] r_hc.h_functions[i].v;
	}
	for(int i=0;i<k;i++){
		delete[] hc.f_funs[i]->table;
		delete[] r_hc.f_funs[i]->table;
	}
	delete[] hc.hypercube->table;
	delete[] hc.h_functions;
	delete[] hc.hypercube;
	delete[] hc.f_funs;
	delete[] images;
	delete[] r_hc.hypercube->table;
	delete[] r_hc.h_functions;
	delete[] r_hc.hypercube;
	delete[] r_hc.f_funs;
	delete[] r_images;
}
