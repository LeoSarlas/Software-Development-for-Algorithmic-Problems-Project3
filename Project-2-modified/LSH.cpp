#include "Header_files/MRNG.h"

using namespace std;

// External declarations for global variables
extern int d; 				// dimensionality of data points
extern int w; 				// window size for range queries
extern int K; 				// number of hash functions for each hash table
extern int hashTableSize; 	// size of each hash table

// Function used for comparing
bool compareMyObjects(const temp_nums& a, const temp_nums& b) {

    return a.dist < b.dist;
}

// Function used for comparing
auto compareMyObjects2(const temp_nums& a, const temp_nums& b) {

    return a.dist == b.dist;
}

// Function to calculate Euclidean distance between two vectors
double distance_metric(vector<int> *v1, vector<int> *v2, int d) {

    // Calculate the Euclidean distance (L2) between the given vectors and return it
    double sum = 0.0;
    vector<int> cords1 = *v1;
    vector<int> cords2 = *v2;

    for(int i = 0; i < d; i++) {

        double temp = cords1[i] - cords2[i];
        sum += pow(temp, 2);
    }

    return sqrt(sum);
}

// Function to generate a uniformly distributed random value
double rand_gen() {

    // Return a uniformly distributed random value
    return ( (double)(rand()) + 1.0 )/( (double)(RAND_MAX) + 1.0 );
}

// Function to generate a normally distributed random value
double normalRandom() {

    double v1 = rand_gen(); 
    double v2 = rand_gen();
    return (cos(2 * 3.14 * v2) * sqrt(-2.0 * log(v1))) * 1.00 + 0.00;
}

// Function to generate a random value within a given range
double uniform_distribution(int rangeLow, int rangeHigh) {

    double myRand = rand() / (1.0 + RAND_MAX);
    int range = rangeHigh - rangeLow;
    double myRand_scaled = (myRand * range) + rangeLow;
    return myRand_scaled;
}

// Function to generate v array coordinates of h function, v ~ N (0, 1)^d
void generateH(h_function *hfun) {

    hfun->v = new double[d];

    for(int i = 0; i < d; i++) {

        hfun->v[i] = normalRandom();
    }

    // Pick t variable uniformly ~ R [0, w)
    double temp = uniform_distribution(0, w);
    hfun->t = temp;
}

// Function to generate h functions that will be used in the calculation of G function
void generateG(g_function *gfun) {

    // Allocate and generate the h functions used in the calculation of the G function
    gfun->h_functions = new h_function[K];

    for(int i = 0; i < K; i++) {

        generateH(&(gfun->h_functions[i]));
    }

    // Allocate and generate as many variables r as the functions h
    gfun->r = new int[K];

    for(int i = 0; i < K; i++) {

        int temp = (rand() % (10)) + 1;
        gfun->r[i] = temp;
    }

    gfun->m = (UINT_MAX - 4);
}


// Function to initialize a hash table
HashTable htInitialize_LSH(int buckets) {

    HashTable ht = new hashtable_head;
    ht->table = new hashtable_node[buckets];
    ht->buckets = buckets;

    // Initialize each bucket in the hash table
    for (int i = 0; i < buckets; i++) {

        ht->table[i].key = i;
        ht->table[i].num_img = 0;
    }

    ht->numberOfVectors = 0;  // Initialize the number of images in the hash table
    return ht;
}


// Function to insert images into the hash table along with their IDs
void htInsert_LSH(HashTable ht, image *v, int index, int id) {

    ht->table[index].labels.push_back(v);
    ht->table[index].id.push_back(id);
    ht->table[index].num_img++;
    ht->numberOfVectors++;
}


// Function to initialize LSH structure with G functions and hash tables
void initializeLSH(int l, lshNode *lsh) {

    // Allocate as many G functions as the number of hash tables
    lsh->g_fun = new g_function[l];

    // Allocate the hash tables needed for LSH
    lsh->hts = new HashTable[l];

    // Generate G functions and initialize the hash tables
    for(int i = 0; i < l; i++) {

        generateG(&(lsh->g_fun[i]));
        lsh->hts[i] = htInitialize_LSH(hashTableSize);
    }

    // Save l (the number of hash tables)
    lsh->l = l;
}

// Function to calculate the dot product between vectors
double dot_product(double *v, vector<int> cords, int d) {

    double result = 0.0;

    for (int i = 0; i < d; i++) {
        result += v[i] * (double)cords[i];
    }

    return result;
}

// Function to compute the value of the h function
int computeH(h_function hfun, image *im) {

    double pv = dot_product(hfun.v, im->cords, d);

    // Finally, calculate the value of the h function
    double temp = (double) (pv + hfun.t) / (double) w;
    return floor(temp);
}

// Function to compute the value of the g function
temp_nums computeG(g_function gfun, image *p) {

    long long int sum = 0;

    // g is a random combination of hi, every g function has k h functions
    // Compute g function
    for(int i = 0; i < K; i++) {

        int h = computeH(gfun.h_functions[i], p);
        int r = (gfun.r[i] * h) % gfun.m;
        unsigned int result = (r < 0) ? (r + gfun.m) : r;
        sum += result;
    }

    long long int temp_ID = sum % gfun.m;
    long long int res = (temp_ID < 0) ? (temp_ID + gfun.m) : temp_ID;
    temp_ID = res;
    temp_nums te;
    te.id = temp_ID;  // Store the id for the querying trick
    int r = temp_ID % hashTableSize;
    int result = (r < 0) ? (r + hashTableSize) : r;
    te.index = result;
    return te; 
}


// Function that inserts an image in all LSH hash tables
void insertToLSH(lshNode *lsh,image *v){

	for(int i=0;i<lsh->l;i++){ // Go at every hash table of lsh

		// Compute the value of the g function for the given vector that will be inserted
		temp_nums index = computeG(lsh->g_fun[i],v); 
		// Insert the vector at the corresponding bucket of the current hash table
		htInsert_LSH(lsh->hts[i],v,index.index,index.id);
	}
}

// Function that takes each image and inserts it in LSH
void insertFromListToLSH(image *img_list,lshNode *lsh,int img_num){ 

	for(int i=0;i<img_num;i++){

		insertToLSH(lsh,&img_list[i]);
	}
}

// Function to insert the query into LSH and find KNN as well as neighbors within a specified radius
vector<temp_nums> query(lshNode *lsh, image *query_list, image *img_list, int q) {
    
	// Vector to store distances, sorting is used to find the top n distances
    int L = lsh->l;

    int counter = 0; 			// Counter for neighbors within radius 
    int dim = query_list[q].ro * query_list[q].co;

    vector<temp_nums> sort_ims; // Vector to store sorted distances
    temp_nums t;
    clock_t begin = clock();

    // Querying LSH
    for(int i = 0; i < L; i++) {

        temp_nums index = computeG(lsh->g_fun[i], &query_list[q]);

        for(int j = 0; j < lsh->hts[i]->table[index.index].num_img; j++) {

            if(index.id == lsh->hts[i]->table[index.index].id[j]) {

                double dist = distance_metric(&query_list[q].cords, &lsh->hts[i]->table[index.index].labels[j]->cords, dim);
                t.dist = dist;
                t.id =  lsh->hts[i]->table[index.index].labels[j]->img_id;
                sort_ims.push_back(t);
                counter++;
            }
        }
    }

    sort(sort_ims.begin(), sort_ims.end(), compareMyObjects); // Sorting distances from LSH
    auto last = unique(sort_ims.begin(), sort_ims.end(), compareMyObjects2);
    sort_ims.erase(last,sort_ims.end());
    return sort_ims;
    
}

// Function to read and process image data from a binary file
image* file_manip(char *fname, int *image_num) {

    ifstream myFile(fname, ios::binary);
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
    (*image_num) = result; 

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
    image *images = new image[(*image_num)];

    // Reading image data from the binary file
    for(int k = 0; k < (*image_num); k++) {

        for(int i = 0; i < rows; i++) {

            for(int j = 0; j < columns; j++) {

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