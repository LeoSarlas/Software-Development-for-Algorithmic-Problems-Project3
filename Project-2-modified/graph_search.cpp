// |_______________________________________________________________________________________| 
// |                                Project 2023-2024 (2)                                  |
// |                                                                                       |
// |                     Σαρλάς Λεωνίδας-Μάρκελλος (ΑΜ 1115201700135)                      |
// |                     Βεκρής Δημήτριος-Αλέξανδρος (ΑΜ 1115202000021)                    |
// |                                                                                       |
// |_______________________________________________________________________________________|

#include "Header_files/MRNG.h"

using namespace std;

int d; 				// dimensionality of the data points
int w; 				// window size for range queries
int K; 				// number of hash functions for each hash table
int hashTableSize;	// size of each hash table

//------------------------------Parsing command line arguments for the GNNS/MRNG-----------------------------------------

int main(int argc, char *argv[]) {

	char inputf[1000]; 		// input file path for data points
	char path[1000]; 		// input file path for query points
	char output[1000];		// input file name for results
	char r_inputf[1000]; 		// input file path for data points
	char r_path[1000]; 
    int k = 50, E = 30, R = 1, Neigh = 1, l = 20, m = 1;
    
    // Parse through command-line arguments
    for (int i = 1; i < argc; i += 2) {

        char* arg = argv[i];

        if (strcmp(arg, "-d") == 0 && i + 1 < argc) {

            strncpy(inputf, argv[i + 1], sizeof(inputf) - 1);
            inputf[sizeof(inputf) - 1] = '\0'; // Null-terminate the string

        } else if (strcmp(arg, "-q") == 0 && i + 1 < argc) {

            strncpy(path, argv[i + 1], sizeof(path) - 1);
            path[sizeof(path) - 1] = '\0'; // Null-terminate the string

        } else if (strcmp(arg, "-k") == 0 && i + 1 < argc) {

            k = atoi(argv[i + 1]);

        } else if (strcmp(arg, "-E") == 0 && i + 1 < argc) {

            E = atoi(argv[i + 1]);

        } else if (strcmp(arg, "-R") == 0 && i + 1 < argc) {

            R = atoi(argv[i + 1]);

        } else if (strcmp(arg, "-N") == 0 && i + 1 < argc) {

            Neigh = atoi(argv[i + 1]);

        } else if (strcmp(arg, "-l") == 0 && i + 1 < argc) {

            l = atoi(argv[i + 1]);

        } else if (strcmp(arg, "-m") == 0 && i + 1 < argc) {

            m = atoi(argv[i + 1]);

        } else if (strcmp(arg, "-o") == 0 && i + 1 < argc) {

            strncpy(output, argv[i + 1], sizeof(output) - 1);
            output[sizeof(output) - 1] = '\0'; // Null-terminate the string
        } else if (strcmp(arg, "-rd") == 0 && i + 1 < argc) {

            strncpy(r_inputf, argv[i + 1], sizeof(r_inputf) - 1);
            inputf[sizeof(r_inputf) - 1] = '\0'; // Null-terminate the string

        } else if (strcmp(arg, "-rq") == 0 && i + 1 < argc) {

            strncpy(r_path, argv[i + 1], sizeof(r_path) - 1);
            path[sizeof(r_path) - 1] = '\0'; // Null-terminate the string

        }
    }

    // Check if required options are missing
    if (inputf==NULL || path==NULL || k == 0 || E == 0 || R == 0 || Neigh == 0 || (l == 0) || (m != 1 && m !=2) || output==NULL) {
        cerr << "Usage: ./graph_search -d <input file> -q <query file> -k <int> -E <int> -R <int> -N <int> -l <int, only for Search-on-Graph> -m <1 for GNNS, 2 for MRNG> -o <output file>" << endl;
        return 1;
    }

	if (l < Neigh){

		cerr << "You must have l >= N. Retry." << endl;
        return 1;
	}

    // Verification of terminal input
    cout << "Input File: " << inputf << endl;
    cout << "Query File: " << path << endl;
    cout << "K: " << k << endl;
    cout << "E: " << E << endl;
    cout << "R: " << R << endl;
    cout << "N: " << Neigh << endl;
    cout << "L: " << l << endl;
    cout << "Method: " << (m == 1 ? "GNNS" : "MRNG") << endl;
	string out(output);
    cout << "Output File: " << out << endl;


 //----------------------------------------------------LSH----------------------------------------------------------------


	// LSH hyperparameter variables
	// Default parameter values 
	int L=5; 				
	K = 4;
	int image_n;
	image *images = file_manip(inputf,&image_n);
	image *r_images = file_manip(r_inputf,&image_n);
	lshNode lsh;
	lshNode r_lsh;
	int d1 =  images[0].ro*images[0].co;
	int d2 =  r_images[0].ro*r_images[0].co;
	hashTableSize = image_n/16;				// Calculating hash table size based on the number of data points
	w = 32; 								// Default value for window size

//-----------------------------------------------Execution of GNNS or MRNG section----------------------------------------------------------

	

	// Convert the array to a vector v_images
	vector<image> v_images;
	vector<image> vr_images;

	for(int i=0;i<image_n;i++){
		v_images.push_back(images[i]);
		vr_images.push_back(r_images[i]);
	}

	// for(int i=0;i<1000;i++){
	// 	v_images.push_back(images[i]);
	// }

	// Checking if we are going to run GNNS or MRNG
	if (m==1){
		d=d1;
		initializeLSH(L,&lsh);
		insertFromListToLSH(images,&lsh,image_n);
		d=d2;
		initializeLSH(L,&r_lsh);
		insertFromListToLSH(r_images,&r_lsh,image_n);
		GNNS *graph = initGNNS(&lsh,images,k,image_n);	//create the GNNS graph 
		GNNS *r_graph = initGNNS(&r_lsh,r_images,k,image_n);
		int repeat = 1;		// Checker for re-execution of the code depending on user's input ('YES' or 'NO')
		int count = 1;
		double avgtrue=0.0;
		double avgapprox=0.0;
		double avgtrue2=0.0;
		double avgapprox2=0.0;
		do{

			int t;
			d=d1;
			image *queries = file_manip(path,&t);
			d=d2;
			image *r_queries = file_manip(r_path,&t);
			int q = rand()%t; 					  // Selecting a random query point
			// Search in the GNNS graph given a query
			avg_time temp1  = searchGNNS(k,E,R,Neigh,graph,queries,images,q,image_n,count,output);
			fstream MyFile;
			MyFile.open(output, fstream::app);

			avgtrue += temp1.True;	// Time-calculation structs
			avgapprox += temp1.Approx;	// Time-calculation structs

			MyFile<<"tApproximate: "<<avgapprox<<endl;	// Printings
			MyFile<<"True: "<<avgtrue<<endl;
			MyFile<<"MAF: "<<temp1.MAF<<endl;
			avg_time temp2  = searchGNNS(k,E,R,Neigh,r_graph,r_queries,r_images,q,image_n,count,output);	

			avgtrue += temp2.True;	// Time-calculation structs
			avgapprox += temp2.Approx;	// Time-calculation structs

			MyFile<<"tApproximate: "<<avgapprox2<<endl;	// Printings
			MyFile<<"tTrue: "<<avgtrue2<<endl;
			MyFile<<"MAF: "<<temp2.MAF<<endl;

			MyFile.close();
			// Asking the user if they want to reeat the process
			char answer;
			cout << "Do you want to repeat the process? (y/n): ";
			cin >> answer;

			if (answer != 'y' && answer != 'Y') {	// 'NO' case
				repeat = 0;
			}

			if(repeat){	// Repeat the process
				cout << "Enter the path of the query file: ";
				delete[] queries;
				delete[] r_queries;
				cin >>path;
				cout << "Enter the path of reduced query file: ";
				cin >>r_path;
				for(int o=0;o<image_n;o++){	//reset the marked nodes
					graph[o].marked=0;
					r_graph[o].marked=0;
				}
			}

			count++;
		} while(repeat);




		// Deallocating memory of LSH


		for(int i=0;i<L;i++){

			delete[] lsh.hts[i]->table;
			delete[] r_lsh.hts[i]->table;
		}

		for(int i=0;i<L;i++){

			for(int j=0;j<K;j++){

				delete[] lsh.g_fun[i].h_functions[j].v;
				delete[] r_lsh.g_fun[i].h_functions[j].v;
			}

			delete[] lsh.g_fun[i].h_functions;
			delete[] lsh.g_fun[i].r;
			delete[] r_lsh.g_fun[i].h_functions;
			delete[] r_lsh.g_fun[i].r;
		}

		delete[] lsh.g_fun;
		delete[] lsh.hts;
		delete[] graph;
		delete[] r_lsh.g_fun;
		delete[] r_lsh.hts;
		delete[] r_graph;

	}
	else{
		// Create an adjacency list to represent the MRNG graph
		vector<Graph_Node> adjacencyList;
		vector<Graph_Node> r_adjacencyList;

		double MAF;
		adjacencyList = constructMRNGGraph(v_images);	// Construct the MRNG graph and store its adjacency list
		r_adjacencyList = constructMRNGGraph(vr_images);

		int repeat = 1;		// Checker for re-execution of the code depending on user's input ('YES' or 'NO')
		int count = 1;
		double avgtrue=0.0;
		double avgapprox=0.0;
		double avgtrue2=0.0;
		double avgapprox2=0.0;

		do{
			
			int t;
			d=d1;
			image *queries = file_manip(path,&t);
			d=d2;
			image *r_queries = file_manip(r_path,&t);
			int q = rand()%t; 					  // Selecting a random query from dataset
			vector<image> q_images;
			q_images.push_back(queries[q]);
			vector<image> qr_images;
			qr_images.push_back(r_queries[q]);
			// Search the MRNG graph for the nearest neighbors of the selected query
			avg_time temp = searchOnGraph(l,Neigh,adjacencyList,q_images,v_images,count,output);

			// Open the output file
			fstream MyFile;
			MyFile.open(output, fstream::app);

			avgtrue += temp.True;
			avgapprox += temp.Approx;

			// Append the results to the output file
			MyFile<<"tApproximate: "<<avgapprox<<endl;
			MyFile<<"tTrue: "<<avgtrue<<endl;
			MyFile<<"MAF: "<<temp.MAF<<endl;
			avg_time temp2 = searchOnGraph(l,Neigh,r_adjacencyList,qr_images,vr_images,count,output);
			avgtrue2 += temp2.True;
			avgapprox2 += temp2.Approx;

			// Append the results to the output file
			MyFile<<"tApproximate: "<<avgapprox2<<endl;
			MyFile<<"tTrue: "<<avgtrue2<<endl;
			MyFile<<"MAF: "<<temp2.MAF<<endl;
			MyFile.close();		// Close the output file

			char answer;
			cout << "Do you want to repeat the process? (y/n): ";
			cin >> answer;

			if (answer != 'y' && answer != 'Y') {
				repeat = 0;
			}

			if(repeat){
				cout << "Enter the path of the query file: ";
				delete[] queries;
				delete[] r_queries;
				cin >>path;
				cout << "Enter the path of reduced query file: ";
				cin >>r_path;
				for(int o=0;o<v_images.size();o++){		//reset the marked nodes
					adjacencyList[o].marked = 0;
					r_adjacencyList[o].marked = 0;
				}
			}
			

			count++;
		} while(repeat);


	}

	// Clean up allocated memory for the dataset
    delete[] images;
	delete[] r_images;
}
