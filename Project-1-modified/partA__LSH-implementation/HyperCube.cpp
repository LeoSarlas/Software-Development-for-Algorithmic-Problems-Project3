#include "HyperCube.h"

using namespace std;


extern int d;
extern int k;
extern int m;
extern int probes;
extern int w;

bool compareMyObjects(const temp_nums& a, const temp_nums& b) {
    return a.dist < b.dist;
}

int binaryArrayToDecimal(int *s,int size){
    int n = 0;
    int i;
    for (i = 0; i < size; ++i){
      n <<= 1;
      n += s[i];
    }
    return n;
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
   return ( (double)(rand()) + 1. )/( (double)(RAND_MAX) + 1. );
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

void generateH_Cube(h_function *hfun){
	hfun->v=new double[d];
	for(int i=0;i<d;i++){
		hfun->v[i] = normalRandom();
	}
	// pick t variable uniformly ~ R [0, w)
	double temp = uniform_distribution(0,w);
	hfun->t=temp;
}
void initializeHyperCube(int k,cubeNode *hc){
	hc->h_functions = new h_function[k];
	for(int i=0;i<k;i++){
		generateH_Cube(&(hc->h_functions[i]));
	}
// allocate and generate f functions based on the given dimension
	hc->f_funs = new HashMap[k];
	for(int i=0;i<k;i++){
		hc->f_funs[i] = hmCreate(100);  // init the hash map
	}
	// finally allocate and initialize tha hash table that hyper cube needs
	hc->hypercube = htInitialize((int)pow(2,k));  // 2^k buckets
}

double dot_product(double *v, vector<int> cords,int d){
	// calculate the dot product between the given vectors and return it
	double result = 0.0;
	for (int i = 0; i < d; i++){
			result += v[i]*(double)cords[i];
	}
	return result;
}

int computeH_Cube(h_function hfun,image *im){
	// compute the dot product of the given vector with the v vector of h function (p * v)
	double pv = dot_product(hfun.v,im->cords,d);
	// finally calculate the value of h function
	double temp = (double) (pv+hfun.t)/(double)w;
	return floor(temp);
}

int computeF(HashMap f_fun,int key){
  // every f function is represented as a hashMap 
  // a f function for each  input (h(p)) returns a specific value of 0 or 1.
  // The item of a hash map is randomly generated and stored in the corresponding bucket (if the key doesn't already exist).
  // if f receive the same input twice, it returns the same output
  // The value of the h function is the key of the a hash map
  // The value of the hash map is a random value (bit) 0 or 1 .
  int bit = rand()%2;	//generate the value bit (0 or 1)
  int v = hmSearchOrInsert(f_fun,key,bit);

  return v;
}

void insertToHyperCube(cubeNode *hc,image *v){
	int indexArray[k];
	for(int i=0;i<k;i++){ // form/find the corresponding index of the hypercube (hash table) that the given vector will be inserted
		int h_result = computeH_Cube(hc->h_functions[i],v); // compute the h function's value based the given v vector
		int f_result = computeF(hc->f_funs[i],h_result); // compute the f function's value based on the previous value that return the h function
		// f returns a bit, 0 or 1, take this bit and append it to the index to form that for the hypercube
		indexArray[i]=f_result;
		// and continue for the next dimension
	}
	// index formed, convert it to decimal
	int decimal_index = binaryArrayToDecimal(indexArray,k);  // hash Table key
	// finally insert the given vector at the hash table
	htInsert(hc->hypercube,v,decimal_index,-1);
}

void insertFromListToHyperCube(image *img_list,cubeNode *hc ,int img_num){
	for(int i=0;i<img_num;i++){
		insertToHyperCube(hc,&img_list[i]);
	}
}

 	
 
 void HammingDistance(cubeNode *hc,image *img_list,int *index,int hammingDist,int startFrom,vector<temp_nums> *sort_ims,int *numOfSearched,int m,int n,int *nodesSearched,int probes,int q){
	// recursive function that computes all the numbers that have hamming distance equal to hammingDist
	int dim = img_list[q].ro*img_list[q].co;
	temp_nums t;
	if(hammingDist<=0){	//if hamm dist reaches 0 then check the distance with query
		int n_index = binaryArrayToDecimal(index,k);
		for(int i=0;i<hc->hypercube->table[n_index].num_img;i++){
			if((*numOfSearched)>=m){
				break;
			}
			double dist = distance_metric(&img_list[q].cords,&hc->hypercube->table[n_index].labels[i]->cords,dim);
			t.dist = dist;
			t.id =  hc->hypercube->table[n_index].labels[i]->img_id;
			(*numOfSearched)++;
			sort_ims->push_back(t);
		}
		if(hc->hypercube->table[n_index].num_img!=0){
			(*nodesSearched)++;
		}
		return;	
	}
	for(int i=startFrom;i<k;i++){
		index[i] = index[i] ^ 1;	//change the bit
		HammingDistance(hc,img_list,index,hammingDist-1,i+1,sort_ims,numOfSearched,m,n,nodesSearched,probes,q);
		index[i] = index[i] ^ 1;	//then change it back
		if((*numOfSearched)>=m || (*nodesSearched)>=probes){
			break; 	
		}
	}
 }
 
void query(cubeNode *hc,image *query_list,image *img_list,int n,double radius,int hammingDist,int m,int q,char *output,int img_num,int count){
	vector<temp_nums> sort_ims2;
	temp_nums t;
	fstream MyFile;
	MyFile.open (output, fstream::app);
	clock_t begin = clock();
	int dim = query_list[q].ro*query_list[q].co;
	for(int i=0;i<img_num;i++){
		double dist =  distance_metric(&query_list[q].cords,&img_list[i].cords,dim);
		t.dist = dist;
		t.id = i;
		sort_ims2.push_back(t); 
	}
	sort(sort_ims2.begin(), sort_ims2.end(), compareMyObjects);
	clock_t end = clock();
	double time_spent1 = (double)(end - begin) / CLOCKS_PER_SEC;
	vector<temp_nums> sort_ims;
	int index[k];
	double ndist = -1.0;
	int searched=0;
	begin = clock();
	for(int i=0;i<k;i++){
		int h_res = computeH_Cube(hc->h_functions[i],&query_list[q]);
		int f_res = computeF(hc->f_funs[i],h_res);
		index[i] = f_res;
	}
	int decimal_index = binaryArrayToDecimal(index,k);
	for(int i=0;i<hc->hypercube->table[decimal_index].num_img;i++){
		if(searched>=m){
			break;
		}
		double dist = distance_metric(&query_list[q].cords,&hc->hypercube->table[decimal_index].labels[i]->cords,dim);
		t.dist = dist;
		t.id =  hc->hypercube->table[decimal_index].labels[i]->img_id;
			sort_ims.push_back(t);
			searched++;
		
	}
	int nodesSearched = 0;
	//hammingDist represente the probes num
	for(int i=1;;i++){	//if searched is smaller than M then we must search the neighbours as well
		if((searched>=m) || (nodesSearched>=hammingDist) || (i>k)){
			break;
		}
		//at fisrt we check the neighbours with hamm dist 1 till hamm dist = max probes or searched = M
		HammingDistance(hc,query_list,index,i,0,&sort_ims,&searched,m,n,&nodesSearched,hammingDist,q);
	}
	sort(sort_ims.begin(), sort_ims.end(), compareMyObjects);
	end = clock();
	double time_spent2 = (double)(end - begin) / CLOCKS_PER_SEC;
	MyFile<<"Query: "<<q<<endl;	
	for(int i=0;i<n;i++){
		MyFile<<"Nearest neighbor-"<<i<<": "<<sort_ims[i].id<<endl;
		MyFile<<"DistanceCube: "<<sort_ims[i].dist<<endl;
		MyFile<<"DistanceTrue: "<<sort_ims2[i].dist<<endl;
		MyFile<<"True neighbor: "<<sort_ims2[i].id<<endl;
	}
	double maf = sort_ims[0].dist/sort_ims2[0].dist;
	MyFile<<"MAF: "<<maf<<endl;
	MyFile<<"tCube: "<<time_spent2<<endl;
	MyFile<<"tTrue: "<<time_spent1<<endl;
	// MyFile<<"R-near neighbors: "<<endl;
	// for(int i=0;i<searched;i++){
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