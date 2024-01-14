#include "methods_and_entities_declarations.h"
#include "helping_functions.h"

// Lloyd's K-Means clustering function
void lloydsKMeansClustering(vector<MNISTImage>& mnistData, int k, bool complete) {
    
    // Start the timer
    auto start = chrono::high_resolution_clock::now();

    // Check if the number of clusters is valid
    if (k <= 0) {

        cerr << "Invalid number of clusters." << endl;
        return;
    }

    // Initialize cluster centroids using k-means++ method
    vector<MNISTImage> centroids, previousCentroids;
    initializeCentroids(mnistData, k, centroids);

    // initialize the first form of the centroids, it will be updated later
    previousCentroids = centroids;

    // Maximum number of iterations or convergence threshold
    int maxIterations = 10;
    double convergenceThreshold = 0.01;
    int counter = 0;

    // Vector to store cluster assignments
    vector<int> clusterAssignments(mnistData.size()/SIZER);

    // Perform Lloyd's K-Means clustering
    for (int iteration = 0; iteration < maxIterations; ++iteration) {

        // Assign data points to the nearest cluster
        for (MNISTImage& dataPoint : mnistData) {

            int nearestCluster = 0;
            double minDistance = calculateL2Norm(dataPoint, centroids[0]);

            for (int clusterIndex = 1; clusterIndex < k; ++clusterIndex) {

                double distance = calculateL2Norm(dataPoint, centroids[clusterIndex]);

                if (distance < minDistance) {

                    nearestCluster = clusterIndex;
                    minDistance = distance;
                }
            }

            // Assign the data point to the nearest cluster (update a cluster assignment variable)
            dataPoint.cluster = nearestCluster;

            // Assign the data point to the nearest cluster
            clusterAssignments[counter++] = nearestCluster;
            
        }

        // Check for convergence
        // Calculate the maximum movement of centroids to determine convergence
        double maxCentroidMovement = 0.0;

        for (int clusterIndex = 0; clusterIndex < k; ++clusterIndex) {

            double movement = calculateL2Norm(centroids[clusterIndex], previousCentroids[clusterIndex]);
            maxCentroidMovement = max(maxCentroidMovement, movement);
        }

        // Update previous centroids for the next iteration
        previousCentroids = centroids;

        // Implement a convergence condition based on centroid movement
        if (maxCentroidMovement < convergenceThreshold) {

            break; // Converged, exit the loop
        }
    }

    // Update cluster centroids using the MacQueen method
    macQueenUpdate(centroids, mnistData, k);

    // Calculate and print the Silhouette score
    double silhouetteScore = calculateSilhouetteScore(mnistData, clusterAssignments);

    // End the timer
    auto end = chrono::high_resolution_clock::now();

    // Calculate the duration
    auto duration = chrono::duration_cast<chrono::seconds>(end - start);

    const string outputFileName = "Classic_output_file.txt";

    // If the output file already exists, delete it
    if (fileExists(outputFileName)) {

        if (!deleteFile(outputFileName)) {

            return; // Exit the function if deletion fails
        }
    }

    // Open the output file for writing
    ofstream output("Classic_output_file.txt");

    if (!output.is_open()) {

        cerr << "Error: Could not open the output file." << endl;
        return;
    }

    // Print clustering results in the specified format
    output << "Algorithm: Lloyds\n";

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

    output << "clustering_time: " << duration.count() << " seconds\n";
    output << "Silhouette: [";
    
    for (int clusterIndex = 0; clusterIndex < k; ++clusterIndex) {

        output << silhouetteScorePerCluster(clusterAssignments, clusterIndex, mnistData);

        if (clusterIndex < k - 1) {

            output << ", ";
        }
    }

    output << ", " << silhouetteScore << "]\n";

    // Optionally with command line parameter –complete
    if (complete) {

        for (int clusterIndex = 0; clusterIndex < k; ++clusterIndex) {

            output << "CLUSTER-" << clusterIndex + 1 << " {centroid, ";

            for (int i = 0; i < mnistData.size()/SIZER; ++i) {

                if (clusterAssignments[i] == clusterIndex) {

                    output << "image_" << i;

                    if (i < mnistData.size()/SIZER - 1) {

                        output << ", ";
                    }
                }
            }
            output << "}\n";
        }
    }

    cout << "You can find the results in 'Classic_output_file.txt'" << endl;

    // Close the output file
    output.close();
}
