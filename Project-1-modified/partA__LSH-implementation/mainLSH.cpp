
#include "LSH.h"

using namespace std;

int d;
int w;
int k;
int hashTableSize;	

int main(int argc, char *argv[]){
	int l=5;
	int n=1;
	double radius=10000;
	char inputf[1000];
	char path[1000];
	char output[1000];
	char r_inputf[1000];
	char r_path[1000];
	k = 4;
	for(int i=1; i<argc; i+=2){ //we take the argumets given from the user and store them corectly
		if(strcmp(argv[i],"-d")==0){
			strcpy(inputf,argv[i+1]);
		}
		if(strcmp(argv[i],"-q")==0){
			strcpy(path,argv[i+1]);
		}
		if(strcmp(argv[i],"-k")==0){
			k = atoi(argv[i+1]);
		}
		if(strcmp(argv[i],"-L")==0){
			l = atoi(argv[i+1]);
		}
		if(strcmp(argv[i],"-o")==0){
			strcpy(output,argv[i+1]);
		}
		if(strcmp(argv[i],"-N")==0){
			n = atoi(argv[i+1]);
		}
		if(strcmp(argv[i],"-R")==0){
			radius = atoi(argv[i+1]);
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
	lshNode lsh;
	lshNode r_lsh;
	int d1 =  images[0].ro*images[0].co;
	int d2 =  r_images[0].ro*r_images[0].co;
	hashTableSize = image_n/16;	
	w = 32;
	d=d1;
	initializeLSH(l,&lsh);
	insertFromListToLSH(images,&lsh,image_n);
	d=d2;
	initializeLSH(l,&r_lsh);
	insertFromListToLSH(r_images,&r_lsh,image_n);
	int repeat =1;
	int count =0;
	do{
		int t;
		d=d1;
		image *queries = file_manip(path,&t);
		d=d2;
		image *r_queries = file_manip(r_path,&t);
		int q = rand()%t;
		query(&lsh,queries,images,n,radius,q,output,image_n,count);
		query(&r_lsh,r_queries,r_images,n,radius,q,output,image_n,count);
		char answer;
		cout << "Do you want to repeat the process? (y/n): ";
		cin >> answer;
		if (answer != 'y' && answer != 'Y') {
			repeat = 0;
		}
		if(repeat){
			cout << "Enter the path of true query file: ";
			delete[] queries;
			delete[] r_queries;
			cin >>path;
			cout << "Enter the path of reduced query file: ";
			cin >>r_path;
		}
		count++;
	}while (repeat);
	
	for(int i=0;i<l;i++){
		delete[] lsh.hts[i]->table;
		delete[] r_lsh.hts[i]->table;
	}
	for(int i=0;i<l;i++){
		for(int j=0;j<k;j++){
			delete[] lsh.g_fun[i].h_functions[j].v;
			delete[] r_lsh.g_fun[i].h_functions[j].v;
		}
		delete[] lsh.g_fun[i].h_functions;
		delete[] r_lsh.g_fun[i].h_functions;
	}
	delete[] lsh.g_fun;
	delete[] lsh.hts;
	delete[] images;
	delete[] r_lsh.g_fun;
	delete[] r_lsh.hts;
	delete[] r_images;
}
