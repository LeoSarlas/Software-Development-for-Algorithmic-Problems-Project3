#include "LSH.h"

using namespace std;


extern int d;
extern int w;
extern int k;
extern int hashTableSize;	


bool compareMyObjects(const temp_nums& a, const temp_nums& b) { //used for sorting purposes only
    return a.dist < b.dist;
}

double distance_metric(vector<int> *v1,vector<int> *v2,int d){
	// calculate the Euclidean distance (or L2) between the given vectors and return it
	double sum = 0.0;
	vector<int> cords1 = *v1;
	vector<int> cords2 = *v2;
	for(int i=0;i<d;i++){
		double temp = cords1[i]-cords2[i];
		sum+=pow(temp,2);
	}
	return sqrt(sum);
}
double rand_gen() {
   	// return a uniformly distributed random value
   	return ( (double)(rand()) + 1.0 )/( (double)(RAND_MAX) + 1.0 );
}

double normalRandom() {
   	// return a normally distributed random value
   	double v1=rand_gen(); 
   	double v2=rand_gen();
   	return (cos(2*3.14*v2)*sqrt(-2.*log(v1)))*1.00+0.00;
}

double uniform_distribution(int rangeLow, int rangeHigh) {
    double myRand = rand()/(1.0 + RAND_MAX);
    int range = rangeHigh - rangeLow;
    double myRand_scaled = (myRand * range) + rangeLow;
    return myRand_scaled;
}

void generateH(h_function *hfun){
	// generate v array coordinates of h function, v ~ N (0, 1)^d
	hfun->v=new double[d];
	for(int i=0;i<d;i++){
		hfun->v[i] = normalRandom();
		
	}
	// pick t variable uniformly ~ R [0, w)
	double temp = uniform_distribution(0,w);
	hfun->t=temp;
}

void generateG(g_function *gfun){
	// allocate and generate the h functions that will be used at the calculation of G function, k (number of h functions)
	// g function is a random combination of hi , every g function has k h functions
	gfun->h_functions = new h_function[k];

	for(int i=0;i<k;i++){
		generateH(&gfun->h_functions[i]);
	}
	// allocate and generate as many variables r as the functions h
	gfun->r = new int[k];
	for(int i=0;i<k;i++){
		int temp = (rand()%(10)) + 1;
		gfun->r[i]=temp;
	}
	gfun->m=(UINT_MAX-4);
}

void initializeLSH(int l,lshNode *lsh){
	// allocate as many G functions as the number of hash tables
	lsh->g_fun = new g_function[l];
	// allocate the hash tables that the LSH need
	lsh->hts = new HashTable[l];
	// generate G functions and initialize the hash tables
	for(int i=0;i<l;i++){
		generateG(&(lsh->g_fun[i]));
		lsh->hts[i] = htInitialize(hashTableSize);
	}
	// save l (the number of hash tables)
	lsh->l=l;
}

double dot_product(double *v, vector<int> cords,int d){
	// calculate the dot product between the given images and return it
	double result = 0.0;
	for (int i = 0; i < d; i++){
		result += v[i]*(double)cords[i];
	}
	return result;
}

int computeH(h_function hfun,image *im){
	// compute the dot product of the given image with the v of h function (p * v)
	double pv = dot_product(hfun.v,im->cords,d);
	// finally calculate the value of h function
	double temp = (double) (pv+hfun.t)/(double)w;
	return floor(temp);
}

temp_nums computeG(g_function gfun,image *p){
	long long int sum = 0;
	// g(p) = [(r1h1(p) + r2h2(p) +...+ rkhk (p)) mod M] mod TableSize
	// g is a random combination of hi, every g function has k h functions
	// compute g function 
	for(int i=0;i<k;i++){
		int h = computeH(gfun.h_functions[i],p);
		// h+=10000;
		 int r =(gfun.r[i]*h)%gfun.m;
		unsigned int result = (r < 0) ? (r + gfun.m) : r;
		sum+= result;
	}
	long long int temp_ID = sum%gfun.m;
	long long int res = (temp_ID < 0) ? (temp_ID + gfun.m) : temp_ID;
	temp_ID = res;
	temp_nums te;
	te.id=temp_ID; 				//store the id for the querying trick
	int r = temp_ID % hashTableSize;
	int result = (r < 0) ? (r + hashTableSize) : r;
	te.index = result;
	return te; 
}
void insertToLSH(lshNode *lsh,image *v){
	// insert the given image in all LSH hash tables
	for(int i=0;i<lsh->l;i++){ // go at every hash table of lsh
		temp_nums index = computeG(lsh->g_fun[i],v); // compute the value of the g function for the given vector that will be inserted
		// finally insert the vector at the corresponding bucket of the current hash table
		htInsert(lsh->hts[i],v,index.index,index.id);
	}
}

void insertFromListToLSH(image *img_list,lshNode *lsh,int img_num){ //function that takes each image and inserts it in LSH
	for(int i=0;i<img_num;i++){
		insertToLSH(lsh,&img_list[i]);
	}
}

void query(lshNode *lsh,image *query_list,image *img_list,int n,double radius,int q,char *output,int img_num,int count){		
	//here we insert the query in lsh and find KNN as well as the neighbors in radius
	//we store the distances in a vector and then we sort them to find the n first
	int L = lsh->l;
	int counter = 0;
	vector<double> dists_vec; 
	int dim = query_list[q].ro*query_list[q].co;
		vector<temp_nums> sort_ims;
		temp_nums t;
	double ndist = -1.0;
	vector<temp_nums> sort_ims2;
	fstream MyFile;
	MyFile.open (output, fstream::app);
	clock_t begin = clock();
	for(int i=0;i<img_num;i++){		//here we find the true NN
		double dist =  distance_metric(&query_list[q].cords,&img_list[i].cords,dim);
		t.dist = dist;
		t.id = i;
		sort_ims2.push_back(t);
	}
	sort(sort_ims2.begin(), sort_ims2.end(), compareMyObjects);
	clock_t end = clock();
	double time_spent1 = (double)(end - begin) / CLOCKS_PER_SEC;
	begin = clock();
	for(int i=0;i<L;i++){
		temp_nums index = computeG(lsh->g_fun[i],&query_list[q]);
		for(int j=0;j<lsh->hts[i]->table[index.index].num_img;j++){
			if(index.id==lsh->hts[i]->table[index.index].id[j]){
				double dist = distance_metric(&query_list[q].cords,&lsh->hts[i]->table[index.index].labels[j]->cords,dim);
				t.dist = dist;
				t.id =  lsh->hts[i]->table[index.index].labels[j]->img_id;
				sort_ims.push_back(t);
				counter++;
			}
		}
	}
	sort(sort_ims.begin(), sort_ims.end(), compareMyObjects);
	end = clock();
	double time_spent2 = (double)(end - begin) / CLOCKS_PER_SEC;
	MyFile<<"Query: "<<q<<endl;
	for(int i=0;i<n;i++){
		MyFile<<"Nearest neighbor-"<<i<<": "<<sort_ims[i].id<<endl;
		MyFile<<"DistanceLSH: "<<sort_ims[i].dist<<endl;
		MyFile<<"DistanceTrue: "<<sort_ims2[i].dist<<endl;
		MyFile<<"True neighbor: "<<sort_ims2[i].id<<endl;
	}
	double maf = sort_ims[0].dist/sort_ims2[0].dist;
	MyFile<<"MAF: "<<maf<<endl;
	MyFile<<"tLSH: "<<time_spent2<<endl;
	MyFile<<"tTrue: "<<time_spent1<<endl;
	// MyFile<<"R-near neighbors: "<<endl;
	// for(int i=0;i<counter;i++){
	// 	if(sort_ims[i].dist>radius){
	// 		break;
	// 	}
	// 	MyFile<<sort_ims[i].id<<endl;
	// }
	MyFile.close();
}
image* file_manip(char *fname,int *image_num){
	ifstream myFile (fname, ios::binary);
	char buffer[4];
	myFile.read(buffer, sizeof(buffer));
	 int result = (static_cast<unsigned char>(buffer[0]) << 24) |
                 (static_cast<unsigned char>(buffer[1]) << 16) |
                 (static_cast<unsigned char>(buffer[2]) << 8) |
                 static_cast<unsigned char>(buffer[3]);
	myFile.read(buffer, sizeof(buffer));
	result = (static_cast<unsigned char>(buffer[0]) << 24) |
                 (static_cast<unsigned char>(buffer[1]) << 16) |
                 (static_cast<unsigned char>(buffer[2]) << 8) |
                 static_cast<unsigned char>(buffer[3]);
	(*image_num)=result; 
	myFile.read(buffer, sizeof(buffer));
	 result = (static_cast<unsigned char>(buffer[0]) << 24) |
                 (static_cast<unsigned char>(buffer[1]) << 16) |
                 (static_cast<unsigned char>(buffer[2]) << 8) |
                 static_cast<unsigned char>(buffer[3]);
	int rows = result;
	myFile.read(buffer, sizeof(buffer));
	 result = (static_cast<unsigned char>(buffer[0]) << 24) |
                 (static_cast<unsigned char>(buffer[1]) << 16) |
                 (static_cast<unsigned char>(buffer[2]) << 8) |
                 static_cast<unsigned char>(buffer[3]);
	int columns = result;
	char buff[1];
	image *images=new image[(*image_num)];
	for(int k=0;k<(*image_num);k++){
		for(int i=0;i<rows;i++){
			for(int j=0;j<columns;j++){
				myFile.read(buff, sizeof(buff));
				result = static_cast<unsigned char>(buff[0]);
				images[k].cords.push_back(result);
				images[k].img_id = k;
				images[k].co = columns;
				images[k].ro = rows;
			}
			
		}
	}
	myFile.close();
	return images;
}
