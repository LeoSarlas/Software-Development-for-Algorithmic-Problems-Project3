#include "methods_and_entities_declarations.h"
#include "helping_functions.h"

// Function to reverse the endianness of a 32-bit integer
int reverseInt(int i) {

    unsigned char c1, c2, c3, c4;
    c1 = i & 255;
    c2 = (i >> 8) & 255;
    c3 = (i >> 16) & 255;
    c4 = (i >> 24) & 255;
    return ((int)c1 << 24) + ((int)c2 << 16) + ((int)c3 << 8) + c4;
}


// Function that reads the .conf file and acquires parameter values
Configuration readConfiguration(const string& configFile) {

    ifstream configFileStream(configFile);
    Configuration config;

    if (!configFileStream) {

        cerr << "Error: Could not open configuration file." << endl;
        // Assign default values if the file is not found
        config.numberOfClusters = 4;
        config.numberOfVectorHashTables = 3;
        config.numberOfVectorHashFunctions = 4;
        config.maxNumberMHypercube = 10;
        config.numberOfHypercubeDimensions = 3;
        config.numberOfProbes = 2;
        return config;

    }

    string line;
    cout << "Reading the configuration (.conf) file... " << line << endl;

    // parse .conf parameters to acquire values
    while (getline(configFileStream, line)) {

        istringstream iss(line);
        string key, assignment;
        int value;
        
        if (iss >> key >> assignment >> value) {

            if (key == "number_of_clusters") {

                config.numberOfClusters = value;

            } else if (key == "number_of_vector_hash_tables") {

                config.numberOfVectorHashTables = value;

            } else if (key == "number_of_vector_hash_functions") {

                config.numberOfVectorHashFunctions = value;

            } else if (key == "max_number_M_hypercube") {

                config.maxNumberMHypercube = value;

            } else if (key == "number_of_hypercube_dimensions") {

                config.numberOfHypercubeDimensions = value;

            } else if (key == "number_of_probes") {

                config.numberOfProbes = value;
            }
        }
    }

    // Assign the default values for missing parameters
    if (config.numberOfClusters == 0) {

        config.numberOfClusters = 10;
    }
    if (config.numberOfVectorHashTables == 0) {

        config.numberOfVectorHashTables = 3;
    }
    if (config.numberOfVectorHashFunctions == 0) {

        config.numberOfVectorHashFunctions = 4;
    }
    if (config.maxNumberMHypercube == 0) {

        config.maxNumberMHypercube = 10;
    }
    if (config.numberOfHypercubeDimensions == 0) {

        config.numberOfHypercubeDimensions = 3;
    }
    if (config.numberOfProbes == 0) {

        config.numberOfProbes = 2;
    }

    return config;
}



// Function to calculate the L2 norm of 2 vectors
double calculateL2Norm(const MNISTImage& dataPoint, const MNISTImage& centroid) {

    if (dataPoint.data.size() != centroid.data.size()) {

        cerr << "Error: Dimensions of vectors don't match." << endl;
        return -1.0;
    }

    double distance = 0.0;

    for (size_t i = 0; i < dataPoint.data.size(); ++i) {

        double diff = dataPoint.data[i] - centroid.data[i];
        distance += diff * diff;
    }

    return sqrt(distance);
}



// Function to read MNIST images from IDX file (ignoring labels)
void readMNISTImages(const string& imagesFile, vector<MNISTImage>& mnistData) {

    // Open the image file
    ifstream imageStream(imagesFile, ios::binary);

    if (!imageStream.is_open()) {

        cerr << "Error: Could not open image file." << endl;
        return;
    }

    // Read the magic number and other header information from the image file
    int magicNumber, numImages, numRows, numCols;

    // Read the header information
    imageStream.read(reinterpret_cast<char*>(&magicNumber), 4);
    // Endianness manipulatio for correct data reading
    magicNumber = reverseInt(magicNumber);

    imageStream.read(reinterpret_cast<char*>(&numImages), 4);
    numImages = reverseInt(numImages);

    imageStream.read(reinterpret_cast<char*>(&numRows), 4);
    numRows = reverseInt(numRows);

    imageStream.read(reinterpret_cast<char*>(&numCols), 4);
    numCols = reverseInt(numCols);

    // Print header information for debugging
    cout << "Magic Number: " << magicNumber << endl;
    cout << "Number of Images: " << numImages << endl;
    cout << "Number of Rows: " << numRows << endl;
    cout << "Number of Columns: " << numCols << endl;

    mnistData.clear();

    for (int i = 0; i < numImages / SIZER; i++) {

        MNISTImage mnistImage;
        mnistImage.data.resize(numRows * numCols);
        mnistImage.index = i;

        // Read the image data (pixel values)
        imageStream.read(reinterpret_cast<char*>(mnistImage.data.data()), numRows * numCols);
        mnistData.push_back(mnistImage);

    }

    cout << "Images Read: " << mnistData.size() / SIZER << endl;

    // Close the image file
    imageStream.close();
}



// Function to initialize cluster centroids using K-Means++ with L2 norm
void initializeCentroids(const vector<MNISTImage>& mnistData, int k, vector<MNISTImage>& centroids) {

    if (k <= 0 || k > mnistData.size()/SIZER) {

        cerr << "Error: Invalid number of clusters." << endl;
        return;
    }

    centroids.clear();
    centroids.reserve(k);

    // Select the first centroid randomly
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> randomIndex(0, mnistData.size()/SIZER - 1);
    int firstCentroidIndex = randomIndex(gen);
    centroids.push_back(mnistData[firstCentroidIndex]);

    // Select the remaining centroids using K-Means++ logic
    for (int i = 1; i < k; i++) {

        vector<double> minDistances(mnistData.size()/SIZER, numeric_limits<double>::max());

        for (int j = 0; j < mnistData.size()/SIZER; j++) {

            // Calculate the distance from each data point to the nearest centroid using L2 norm
            for (size_t centroidIdx = 0; centroidIdx < centroids.size(); centroidIdx++) {

                double distance = 0.0;

                for (size_t pixel = 0; pixel < centroids[centroidIdx].data.size(); pixel++) {

                    double diff = mnistData[j].data[pixel] - centroids[centroidIdx].data[pixel];
                    distance += diff * diff;
                }

                distance = sqrt(distance); // Calculate L2 norm (Euclidean distance)

                if (distance < minDistances[j]) {

                    minDistances[j] = distance;
                }
            }
        }

        // Select the next centroid with probability proportional to distance
        uniform_real_distribution<double> distribution(0, accumulate(minDistances.begin(), minDistances.end(), 0.0));
        double randomValue = distribution(gen);
        double sum = 0.0;

        for (int j = 0; j < mnistData.size()/SIZER; j++) {

            sum += minDistances[j];

            if (sum >= randomValue) {

                centroids.push_back(mnistData[j]);
                break;
            }
        }
    }
}



// Function to perform the MacQueen update step for K-Means
void macQueenUpdate(vector<MNISTImage>& centroids, vector<MNISTImage>& mnistData, int k) {

    if (centroids.empty() || mnistData.empty()) {

        cerr << "Error: Centroids vector or MNIST data vector is empty." << endl;
        return;
    }

    size_t numClusters = k;

    if (numClusters == 0) {

        cerr << "Error: Invalid number of clusters." << endl;
        return;
    }

    size_t dimensionality = centroids[0].data.size();

    if (dimensionality == 0) {

        cerr << "Error: Invalid dimensionality." << endl;
        return;
    }

    // Initialize temporary variables to store the sum of data points and the count in each cluster
    vector<vector<int>> clusterCounts(numClusters, vector<int>(dimensionality, 0));
    vector<vector<double>> clusterSums(numClusters, vector<double>(dimensionality, 0.0));

    // Initialize cluster labels to -1 (unassigned)
    for (MNISTImage& dataPoint : mnistData) {

        dataPoint.label = -1;
    }

    // Assign data points to the nearest cluster (recomputing assignments)
    for (size_t dataIndex = 0; dataIndex < mnistData.size(); ++dataIndex) {

        priority_queue<pair<double, size_t>> minDistances; // Stores distances and cluster indices

        for (size_t cluster = 0; cluster < numClusters; ++cluster) {

            double distance = 0.0;

            for (size_t pixel = 0; pixel < dimensionality; ++pixel) {

                double diff = mnistData[dataIndex].data[pixel] - centroids[cluster].data[pixel];
                distance += diff * diff;
            }

            distance = sqrt(distance); // Calculate L2 norm (Euclidean distance)
            
            minDistances.push({ distance, cluster });
        }

        // Assign the data point to the nearest cluster
        size_t closestCluster = minDistances.top().second;
        mnistData[dataIndex].label = closestCluster;

        // Update the sums and counts for the assigned cluster using L2 norm
        for (size_t pixel = 0; pixel < dimensionality; ++pixel) {

            clusterSums[closestCluster][pixel] += mnistData[dataIndex].data[pixel];
            clusterCounts[closestCluster][pixel]++;
        }
    }

    // Recalculate the centroids based on updated sums and counts using L2 norm
    for (size_t cluster = 0; cluster < numClusters; ++cluster) {

        for (size_t pixel = 0; pixel < dimensionality; ++pixel) {

            if (clusterCounts[cluster][pixel] != 0) {

                centroids[cluster].data[pixel] = clusterSums[cluster][pixel] / clusterCounts[cluster][pixel];
            }
        }
    }
}



// Function to calculate the distance from a point to the centroid of a cluster
double calculateCentroidDistance(const vector<MNISTImage>& cluster, const MNISTImage& point) {

    double sumDistance = 0.0;

    for (const MNISTImage& member : cluster) {

        sumDistance += calculateL2Norm(member, point);
    }

    return sumDistance / (cluster.size() + 1); // Incremental update
}



// Function to calculate the Silhouette score
double calculateSilhouetteScore(const vector<MNISTImage>& mnistData, const vector<int>& clusterAssignments) {

    int k = *max_element(clusterAssignments.begin(), clusterAssignments.end()) + 1;
    vector<double> a(mnistData.size()/SIZER, 0.0); // Average distance to own cluster
    vector<double> b(mnistData.size()/SIZER, 0.0); // Average distance to other clusters
    vector<int> clusterSize(k, 0); // Number of data points in each cluster

    // Calculate 'a' and 'b' for each data point
    for (size_t i = 0; i < mnistData.size()/SIZER; ++i) {

        int currentCluster = clusterAssignments[i];
        int dataSize = 0;

        // Calculate 'a' for each data point
        // 'a' = Average Intra-Cluster Dissimilarity
        for (size_t j = 0; j < mnistData.size()/SIZER; ++j) {

            if (clusterAssignments[j] == currentCluster) {

                a[i] += calculateL2Norm(mnistData[i], mnistData[j]);
                dataSize++;
            }
        }

        if (dataSize > 1) {

            a[i] /= dataSize;
        }

        // Calculate 'b' for each data point
        // 'b' = Average Inter-Cluster Dissimilarity
        for (int cluster = 0; cluster < k; ++cluster) {

            if (cluster != currentCluster) {

                double sumDistance = 0.0;
                int clusterSize = 0;

                for (size_t j = 0; j < mnistData.size()/SIZER; ++j) {

                    if (clusterAssignments[j] == cluster) {

                        sumDistance += calculateL2Norm(mnistData[i], mnistData[j]);
                        clusterSize++;
                    }
                }

                if (clusterSize > 0) {

                    sumDistance /= clusterSize;
                    b[i] = (b[i] == 0.0) ? sumDistance : min(b[i], sumDistance);
                }
            }
        }
    }

    // Calculate the Silhouette score
    double silhouetteScore = 0.0;

    for (size_t i = 0; i < mnistData.size()/SIZER; ++i) {

        double maxAB = max(a[i], b[i]);
        silhouetteScore += (b[i] - a[i]) / maxAB;
    }

    silhouetteScore /= static_cast<double>(mnistData.size()/SIZER);
    return silhouetteScore;
}



// Function to calculate the size of a cluster
int clusterSize(const vector<MNISTImage>& mnistData, int clusterIndex) {

    if (clusterIndex < 0 || clusterIndex >= mnistData.size()/SIZER) {

        cerr << "Error: Invalid cluster index." << endl;
        return -1;
    }

    int size = 0;

    for (const MNISTImage& dataPoint : mnistData) {

        if (dataPoint.label == clusterIndex) {

            size++;
        }
    }
    
    return size;
}


// Function that calculates the centroid coordinates of a cluster
MNISTImage calculateClusterCentroid(const vector<MNISTImage>& cluster) {

    MNISTImage centroid;

    if (cluster.empty()) {

        return centroid; // Return an empty MNISTImage if the cluster is empty.
    }

    const size_t numPoints = cluster.size();
    const size_t dimensionality = cluster[0].data.size(); // Assuming all points have the same dimensionality.

    // Initialize centroid data with zeros.
    centroid.data = vector<uint8_t>(dimensionality, 0);

    for (size_t i = 0; i < dimensionality; ++i) {

        double sum = 0.0;

        for (size_t j = 0; j < numPoints; ++j) {

            sum += cluster[j].data[i];
        }

        centroid.data[i] = static_cast<uint8_t>(sum / numPoints);
    }

    return centroid;
}



// Function to calculate the silhouette score for a specific cluster
double silhouetteScorePerCluster(const vector<int>& clusterAssignments, int clusterIndex, const vector<MNISTImage>& mnistData) {

    int clusterSize = 0;
    double cohesion = 0.0;
    double separation = 0.0;

    for (int i = 0; i < clusterAssignments.size(); ++i) {

        if (clusterAssignments[i] == clusterIndex) {

            clusterSize++;
            double a_i = 0.0;
            double b_i = numeric_limits<double>::max();

            for (int j = 0; j < clusterAssignments.size(); ++j) {

                if (clusterAssignments[j] == clusterIndex) {

                    if (i == j) {

                        continue; // Skip the same point.
                    }

                    // Calculate the average distance between the current point and other points in the same cluster (cohesion).
                    a_i += calculateL2Norm(mnistData[i], mnistData[j]);

                } else {

                    // Calculate the average distance between the current point and points in other clusters (separation).
                    b_i = min(b_i, calculateL2Norm(mnistData[i], mnistData[j]));
                }
            }

            if (clusterSize > 1) {

                a_i /= (clusterSize - 1); // Calculate the average cohesion.
            }

            cohesion += a_i;
            separation += b_i;
        }
    }

    if (clusterSize == 0) {

        return 0.0; // If the cluster is empty, the silhouette score is 0.
    }

    cohesion /= clusterSize;
    double silhouette = (separation - cohesion) / max(cohesion, separation);
    return silhouette;
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
