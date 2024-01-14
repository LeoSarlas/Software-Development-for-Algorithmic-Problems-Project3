#include "methods_and_entities_declarations.h"
#include "helping_functions.h"

// Function to perform reverse search using random projection with MacQueen update
void reverseSearchRandomProjection(vector<MNISTImage>& mnistData, int k, bool complete, Configuration config) {
    
    // Start the timer
    auto start = chrono::high_resolution_clock::now();

    // Generate random projections
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<double> distribution(-1.0, 1.0);

    // Use the values from the configuration
    int numProjections = config.maxNumberMHypercube;
    int numDimensions = config.numberOfHypercubeDimensions;
    int numProbes = config.numberOfProbes;

    vector<vector<double>> randomProjections(numProjections);
    vector<MNISTImage> centroids;

    initializeCentroids(mnistData, k, centroids);
    
    for (int i = 0; i < numProjections; i++) {

        randomProjections[i].resize(numDimensions); // Use the specified number of dimensions

        for (int j = 0; j < numDimensions; j++) {

            randomProjections[i][j] = distribution(gen);
        }
    }
    
    // Initialize cluster assignments
    vector<int> clusterAssignments(mnistData.size()/SIZER, 0);  // Assuming all data points belong to the same cluster

    // Perform reverse search using random projection with probes
    for (int i = 0; i < mnistData.size() / SIZER; i++) {

        double minDistance = numeric_limits<double>::max();
        int closestCluster = -1;

        // Use probes to limit the search
        for (int p = 0; p < numProbes; p++) {

            // Calculate the dot product between the random projection vector and the data point
            double projectionResult = 0.0;

            for (int j = 0; j < numDimensions; j++) {

                projectionResult += randomProjections[p][j] * mnistData[i].data[j];
            }

            // Find the closest cluster
            for (int cluster = 0; cluster < k; cluster++) {

                double distance = calculateL2Norm(mnistData[i], centroids[cluster]);

                if (distance < minDistance) {

                    minDistance = distance;
                    closestCluster = cluster;
                }
            }
        }

        // Update the cluster assignment
        clusterAssignments[i] = closestCluster;
    }

    // Perform the MacQueen update after assigning all data points
    macQueenUpdate(centroids, mnistData, k);

    // Calculate and print the Silhouette score
    double silhouetteScore = calculateSilhouetteScore(mnistData, clusterAssignments);
    
    // End the timer
    auto end = chrono::high_resolution_clock::now();

    // Calculate the duration
    auto duration = chrono::duration_cast<chrono::seconds>(end - start);

    
    const string outputFileName = "Random_projection_output_file.txt";

    // If the output file already exists, delete it
    if (fileExists(outputFileName)) {

        if (!deleteFile(outputFileName)) {

            return; // Exit the function if deletion fails
        }
    }

    // Open the output file for writing
    ofstream output("Random_projection_output_file.txt");

    if (!output.is_open()) {

        cerr << "Error: Could not open the output file." << endl;
        return;
    }

    // Printing results in the specified format
    output << "Algorithm: Range Search Random Projection with MacQueen Update" << endl;

    // Iterate through clusters
    for (int clusterIndex = 0; clusterIndex < k; clusterIndex++) {

        output << "CLUSTER-" << clusterIndex + 1 << " {size: " << clusterSize(mnistData, clusterIndex) << ", centroid: [";

        // Print centroid coordinates
        const MNISTImage& centroid = centroids[clusterIndex];

        for (int j = 0; j < centroid.data.size(); j++) {

                // Check if the value is a valid numerical value (between 0 and 255)
                if (centroid.data[j] >= 0 && centroid.data[j] <= 255) {

                    // Cast the uint8_t value to an integer
                    int numericValue = static_cast<int>(centroid.data[j]);
                    output << numericValue;

                    if (j < centroid.data.size() - 1) {

                        output << ", ";
                    }
                }
            }

        output << "]}" << endl;
    }
    
    output << "clustering_time: " << duration.count() << " seconds" << endl;
    
    // Print silhouette scores
    output << "Silhouette: [";

    for (int clusterIndex = 0; clusterIndex < k; clusterIndex++) {

        double s_i = silhouetteScorePerCluster(clusterAssignments, clusterIndex, mnistData);
        output << s_i;

        if (clusterIndex < k - 1) {

            output << ", ";
        }
    }
    
    output << ", " << silhouetteScore << "]\n";
    
    if (complete) {

        // Optionally, print additional details showing which data points belong to each cluster
        for (int clusterIndex = 0; clusterIndex < k; clusterIndex++) {

            output << "CLUSTER-" << clusterIndex + 1 << " {centroid, ";

            for (int i = 0; i < mnistData.size()/SIZER; i++) {

                if (clusterAssignments[i] == clusterIndex) {

                    // Print image number i for the current cluster
                    output << "image_" << i << ", ";
                }
            }
            output << "}" << endl;
        }
    }

    cout << "You can find the results in 'Reverse_Search_random_projection_output_file.txt'" << endl;

    // Close the output file
    output.close();
}
