#include "Header_files/MRNG.h"

using namespace std;

// Function to calculate the L2 norm of 2 vectors
double calculateDistance(const image& dataPoint, const image& centroid, int p) {

    // Check if the dimensions of the vectors match
    if (dataPoint.cords.size() != centroid.cords.size()) {
        
        // Print an error message and return -1.0 if dimensions don't match
        cerr << "Error: Dimensions of vectors don't match." << endl;
        return -1.0;
    }

    double distance = 0.0;

    // Calculate the Lp norm (L2 norm for p = 2) between the two vectors
    for (size_t i = 0; i < dataPoint.cords.size(); ++i) {

        double diff = dataPoint.cords[i] - centroid.cords[i];
        distance += pow(diff, p);
    }

    return pow(distance, 1.0 / p);
}


// Function to construct MRNG (Minimum Redundancy Neighbor Graph) using adjacency lists
vector<Graph_Node> constructMRNGGraph(const vector<image>& images) {

    vector<Graph_Node> adjacencyList; // List to store nodes and their neighbors in the MRNG graph

    // Iterate over each data point (image) in the dataset
    for (int p = 0; p < images.size()   ; ++p) {
        cout<<p<<endl;

        Graph_Node node;      // Create a node for the current data point
        node.index = p;       // Set the index of the node to the current data point index
        node.marked = 0;

        // Candidates (all data points except the current one)
        temp_nums t;
        vector<temp_nums> Rp;

        // Fill Rp with indices of all data points except the current one
        for (int i = 0; i < images.size(); ++i) {

            if (i != p) {

                double dist =  calculateDistance(images[p], images[i], norm);
                t.dist = dist;
                t.id=i;
                Rp.push_back(t);

            }
        }

        sort(Rp.begin(), Rp.end(), compareMyObjects);

        // Initialize Lp (list of closest data points to the current one)
        vector<temp_nums> Lp;
        double minDistance = numeric_limits<double>::max();

        // Lines 69-79 is the general solution of the problem, because it is possible that
        // the query point has >1 nearest neighbors with exactly the same distance but different coordinates

        // Find the closest data points to the current one and store them in Lp
        // for (temp_nums x : Rp) {
        //     double distance = calculateDistance(images[p], images[x.id], norm);
        //     if (distance < minDistance) {
        //         minDistance = distance;
        //         Lp = { x };
        //     }
        //     else if (distance == minDistance) {
        //         Lp.push_back(x);
        //     }
        // }
        
        Lp.push_back(Rp[0]);
        // sort(Lp.begin(), Lp.end(), compareMyObjects);

        vector<temp_nums> difference;

        // Custom comparison function for sorting based on distance and ID
        auto customComparison = [](const temp_nums& a, const temp_nums& b) {
            return (a.dist < b.dist) || ((a.dist== b.dist) && (a.id < b.id));
        };

        // Compute the set difference between Rp and Lp and store it in the 'difference' vector
        set_difference(Rp.begin(), Rp.end(),Lp.begin(), Lp.end(),back_inserter(difference),customComparison);

        // Check conditions for each r in Rp - Lp
        for (temp_nums r : difference) {

            bool condition = true;

            // Check conditions between each candidate (r) and the closest points (Lp)
            for (temp_nums t : Lp) {

                double prDistance = calculateDistance(images[p], images[r.id], norm);
                double ptDistance = calculateDistance(images[p], images[t.id], norm);
                double rtDistance = calculateDistance(images[r.id], images[t.id], norm);

                // Check conditions for MRNG connectivity
                if (prDistance > rtDistance && prDistance > ptDistance) {

                    condition = false;
                    break;

                }
            }

            // If conditions are met, add the candidate (r) to the neighbors of the current node
            if (condition) {
                node.neighbors.push_back(r.id);
            }
        }
        // Add the current node to the adjacency list
        adjacencyList.push_back(node);
    }

    // Return the constructed MRNG graph represented by the adjacency list
    return adjacencyList;
}


// Function to extract MRNG edges from the adjacency list


// Function to calculate the centroid of the dataset
image calculateCentroid(const vector<image>& images) {

    // Check if the dataset is empty
    if (images.empty()) {

        cerr << "Error! Dataset is empty." << endl;
        return image{};
    }

    // Initialize the centroid with zeros
    image centroid;
    centroid.cords.resize(images[0].cords.size(), 0);

    // Calculate the sum of coordinates for all images in the dataset
    for (const image& img : images) {
        transform(centroid.cords.begin(), centroid.cords.end(), img.cords.begin(), centroid.cords.begin(), plus<int>());
    }

    // Normalize the sum to get the average coordinates (centroid)
    for (int& value : centroid.cords) {
        value /= static_cast<int>(images.size());
    }

    // Return the computed centroid
    return centroid;
}



// Function to search for the nearest neighbor using brute force
vector<int> search_NN_of_NavNode(const vector<image>& images, const image& queryPoint, int k) {
    
    vector<int> NN;

    // Initialize with a large value for comparison
    double minDistance = numeric_limits<double>::max();

    for (size_t i = 0; i < images.size(); ++i) {

        // Skip the query point itself
        if (&images[i] == &queryPoint) {
            continue;
        }

        double distance = calculateDistance(queryPoint, images[i], norm);

        // If the current distance is smaller than the current minimum
        if (distance < minDistance) {

            NN.clear();             // Reset the result
            NN.push_back(i);        // Add the new nearest neighbor
            minDistance = distance; // Update the minimum distance

        } else if (distance == minDistance && NN.size() < static_cast<size_t>(k)) {

            // If there are ties and we need more neighbors, add the current index
            NN.push_back(i);
        }
    }

    return NN;
}


// Function to search for N nearest neighbors on the MRNG graph using the concept of Navigating Node
avg_time searchOnGraph(int l,int Neigh,vector<Graph_Node> &adjacencyList,vector<image> &q_images, vector<image> &v_images,int count,char* output) {

    avg_time time;
    fstream MyFile;
    MyFile.open(output, fstream::app);

    clock_t begin = clock();

    image NavNode = calculateCentroid(v_images);            //calculate the centroid image
    vector<int> NN_of_NavNode = search_NN_of_NavNode(v_images, NavNode, 1);         //find its nearest neighbour with brute force

    vector<temp_nums> R;
    temp_nums temp;

    double distance = calculateDistance(q_images[0], v_images[NN_of_NavNode[0]], norm);

    temp.dist = distance;
    temp.id = NN_of_NavNode[0];
    R.push_back(temp);

    int i=1;
    int p;
    // Graph searching
    while(i<l){     

        for(temp_nums t: R){    //find first unmarked node in graph

            if(adjacencyList[t.id].marked==0){

                p=t.id;
                adjacencyList[t.id].marked=1;
                break;

            }
        }
        for(int x:adjacencyList[p].neighbors){  //search its neighbours

            // Search the neighbors of the unmarked node
            if(adjacencyList[x].marked==0){ 

                // if(i==l){
                //     break;
                // }

                distance = calculateDistance(q_images[0], v_images[x], norm);
                temp.dist = distance;
                temp.id = x;
                R.push_back(temp);
                i++;

            }
        }
    }
    // Sort the results from the graph search
    sort(R.begin(), R.end(), compareMyObjects); 

    clock_t end = clock();
    double time_spent2 = (double)(end - begin) / CLOCKS_PER_SEC;

    // Temporary vector for storing true nearest neighbors
    temp_nums t;
    vector<temp_nums> sort_ims2;

    begin = clock();

    // Find true nearest neighbors
    for(int i = 0; i < v_images.size(); i++) {  

        double dist =  calculateDistance(q_images[0], v_images[i], norm);
        t.dist = dist;
        t.id = i;
        sort_ims2.push_back(t);

    }

    // Sort true nearest neighbors
    sort(sort_ims2.begin(), sort_ims2.end(), compareMyObjects); 

    end = clock();

    double time_spent1 = (double)(end - begin) / CLOCKS_PER_SEC;

    // Write results to the output file
    MyFile << "Query: " << count << endl;

    for(int i = 0; i < Neigh; i++){

        MyFile<<"Nearest neighbor-"<<i<<": "<<R[i].id<<endl;
		MyFile<<"DistanceApproximate: "<<R[i].dist<<endl;
		MyFile<<"DistanceTrue: "<<sort_ims2[i].dist<<endl;
        MyFile<<"True Neighbor: "<<sort_ims2[i].id<<endl;
    }
    time.Approx = time_spent2;
    time.True = time_spent1;

    // Calculate and write MAF
    double maf = R[0].dist/sort_ims2[0].dist;
    time.MAF = maf;

    MyFile.close();

    return time;

}


// Function to check if a file exists
bool fileExists(const string& filename) {

    ifstream file(filename);
    return file.good();
}

// Function to delete a file
bool deleteFile(const string& filename) {

    if (remove(filename.c_str()) != 0) {

        cerr << "Error: Could not delete the file." << endl;
        return false;
    }

    return true;
}


//------------------------------------------------GNNS--------------------------------------------------------------//

GNNS* initGNNS(lshNode *lsh,image *img_list,int k,int image_num){

    GNNS *graph = new GNNS [image_num];

    //We create a graph with image num verticies
    for(int i=0;i<image_num;i++){

        //Here we take the nearest neighbours of each image (minus it self cause the distance would be 0)
        vector<temp_nums> NN_lsh = query(lsh,img_list,img_list,i);
        int count =0;
        graph[i].marked = 0;

        //Link each image with its k neighbours 
        for(temp_nums j :NN_lsh){

            if(count==(k)){

                break;
            }

            if(j.dist!=0){

                int temp =j.id; 
                graph[i].img_ids.push_back(temp);
                count++;
            }

        }

    }

    return graph;
}
//Function that searches in the graph for the N nearest neighbours in GNNS and prints the results given a query
avg_time searchGNNS(int k,int e,int r,int n,GNNS *graph,image *query_list, image *img_list, int q,int image_num,int count,char* output){
    
    avg_time time;
    fstream MyFile;

    MyFile.open(output, fstream::app);
    int dim = query_list[q].ro * query_list[q].co;
    vector<temp_nums> S;
    clock_t begin = clock();

    // Loop to perform the graph search for the specified number of rounds (r)
    for(int i=0;i<r;i++){

        int t = rand()%image_num;       //random starting node in the graph
        int t_next;
        graph[t].marked = 1;
        temp_nums temp;
        double dist = distance_metric(&query_list[q].cords, &img_list[t].cords, dim);
        temp.dist = dist;
        temp.id = t;
        S.push_back(temp);

        for(int j=0;j<50;j++){

            int count =0;
            double min = 1000000;

            // Loop over neighbors of the current node
            for(int m:graph[t].img_ids){

                if(count == e){

                    break;
                }

                if(graph[m].marked==0){
                    
                    // Calculate distance between query and neighbor
                    double dist = distance_metric(&query_list[q].cords, &img_list[m].cords, dim);
                    // Update minimum distance and the next node to visit
                    if(min>dist){

                        min = dist;
                        t_next = m;
                    }
                    // Store distance and node information
                    temp.dist = dist;
                    temp.id = m;
                    S.push_back(temp);
                    count++;
                    graph[m].marked=1;  // Mark the node as visited
                }
            }
            t = t_next; // Move to the next node in the graph

        }
    }
    // Sort the results from the graph search
    sort(S.begin(), S.end(), compareMyObjects);

    clock_t end = clock();
    double time_spent2 = (double)(end - begin) / CLOCKS_PER_SEC;

    // Temporary vector for storing true nearest neighbors
    temp_nums t;
    vector<temp_nums> sort_ims2;
    begin = clock();

    for(int i = 0; i < image_num; i++) {

        double dist =  distance_metric(&query_list[q].cords, &img_list[i].cords, dim);
        t.dist = dist;
        t.id = i;
        sort_ims2.push_back(t);
    }

    // Sort true nearest neighbors
    sort(sort_ims2.begin(), sort_ims2.end(), compareMyObjects);
    end = clock();
    double time_spent1 = (double)(end - begin) / CLOCKS_PER_SEC;

    // Write results to the output file
    MyFile << "Query: " << q << endl;

    for(int i = 0; i < n; i++){

        MyFile<<"Nearest neighbor-"<<i<<": "<<S[i].id<<endl;
		MyFile<<"DistanceApproximate: "<<S[i].dist<<endl;
		MyFile<<"DistanceTrue: "<<sort_ims2[i].dist<<endl;
        MyFile<<"True Neighbor: "<<sort_ims2[i].id<<endl;
    }

    time.Approx = time_spent2;
    time.True = time_spent1;
    // Calculate and write MAF
    double maf = S[0].dist/sort_ims2[0].dist;
    time.MAF = maf;
    // Close the output file
    MyFile.close();
    // Return time information
    return time;
    
}