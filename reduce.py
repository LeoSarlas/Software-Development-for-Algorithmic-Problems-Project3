# |_______________________________________________________________________________________| 
# |                                Project 2023-2024 (3)                                  |
# |                                                                                       |
# |                     Σαρλάς Λεωνίδας-Μάρκελλος (ΑΜ 1115201700135)                      |
# |                     Βεκρής Δημήτριος-Αλέξανδρος (ΑΜ 1115202000021)                    |
# |                                                                                       |
# |_______________________________________________________________________________________|


#   python3 reduce.py -d MNIST_60k.idx3-ubyte -q MNIST_10k.idx3-ubyte -od NN_MNIST_60k.idx3-ubyte -oq NN_MNIST_10k.idx3-ubyte

import argparse                                         # For handling command-line arguments
import os                                               # For working with file paths and directories
os.environ['TF_CPP_MIN_LOG_LEVEL'] = '3'                # Set TensorFlow logging level to suppress unnecessary messages
from sklearn.model_selection import train_test_split    # For splitting the dataset
import idx2numpy                                        # For working with binary files in the IDX format
import numpy as np                                      # NumPy library for numerical operations
import struct                                           # For handling binary data and structuring data in memory
import tensorflow as tf                                 # Open-source ML library
from tensorflow import keras                            # High-level neural networks API
from keras import layers, models                        # For constructing neural network models
from keras.optimizers import Adam, RMSprop              # Optimizers for training neural networks
import sys                                              # System-specific parameters and functions



# Function to save MNIST images to a file in IDX format
def save_mnist_images(images, filename,rows,cols):

    # Check if the input images are in 3D or 2D format
    if images.ndim == 3:

        num_images, rows, cols = images.shape

    elif images.ndim == 2:

        num_images, flattened_size = images.shape

        # Check if the 2D images can be reshaped to (num_images, 28, 28)
        if flattened_size % (rows * cols) != 0:

            raise ValueError("Invalid shape for 2D 'images' array.")
               
        images = images.reshape(num_images, rows, cols)

    else:

        raise ValueError("Invalid shape for 'images' array. It must be either 2D or 3D.")

    # Open the file in binary write mode
    with open(filename, 'wb') as file:

        # Write the magic number, number of images, rows, and columns
        file.write(struct.pack('>IIII', 2051, num_images, rows, cols))

        # Write the image data to the file
        images.tofile(file)



# Function to load MNIST images from a file in IDX format
def load_mnist_images(filename):

    # Open the file in binary read mode
    with open(filename, 'rb') as file:

        # Read the magic number, number of images, rows, and columns
        magic, num_images, rows, cols = struct.unpack('>IIII', file.read(16))
        print(num_images, rows, cols)

        # Read the image data
        image_data = np.fromfile(file, dtype=np.uint8)

    # Check if the image_data size is consistent with the specified dimensions
    expected_size = num_images * rows * cols

    # Adjust the condition to check for an exact match
    if image_data.size != expected_size:

        raise ValueError(f"Invalid size for 'image_data'. Expected {expected_size}, got {image_data.size}.")

    # Reshape the image data to the expected shape
    image_data = image_data.reshape(num_images, rows, cols)

    return image_data



# Function to build a convolutional autoencoder model
def build_convolutional_autoencoder( conv_layers, filter_size, filters_per_layer, epochs, batch_size, latent_dim, dataset_filename):
    
    # Load the MNIST dataset from the provided filename
    images = load_mnist_images(dataset_filename)
    
    # Check if the dataset is valid
    if (images is None):
        print("Error! Invalid dataset.\n")
        return None


    # Convert the dataset to a NumPy array if not already
    images = np.array(images)
    
    # Normalize pixel values to be in the range [0, 1]
    images = images / 255.0

    # Add a channel dimension for convolutional layers
    images = np.expand_dims(images, axis=-1)

    # Split the data into training, validation, and testing sets
    X_train, X_val, train_ground, val_ground = train_test_split(images, images, test_size=0.2, random_state=42)

    # Define the input layer for the autoencoder model
    input_img = layers.Input(shape=(28, 28, 1))
    x = input_img

    best_validation_loss = float('inf')


    # Build the encoder layers
    for _ in range(conv_layers):
        x = layers.Conv2D(filters=filters_per_layer, kernel_size=(filter_size, filter_size), activation='relu', padding='same')(x)
        x = layers.MaxPooling2D((2, 2), padding='same')(x)

    # Flatten the encoded representation  
    x = layers.Flatten()(x)
    encoder = layers.Dense(latent_dim, activation=None,name='latent_layer')(x)
    x=encoder

    if(conv_layers==2):

        x = layers.Dense(filters_per_layer*(7**2),activation=None)(x)
        x = layers.Reshape((7,7,filters_per_layer))(x)

    elif(conv_layers==1):

        x = layers.Dense(filters_per_layer*(14**2),activation=None)(x)
        x = layers.Reshape((14,14,filters_per_layer))(x)
    
    # Build the decoder layers
    for _ in range(conv_layers):

        x = layers.Conv2D(filters=filters_per_layer, kernel_size=(filter_size, filter_size), activation='relu', padding='same')(x)
        x = layers.UpSampling2D((2, 2))(x)
   
    # Output layer of the autoencoder
    decoded = layers.Conv2D(1, (filter_size, filter_size), activation='sigmoid', padding='same')(x)

    # Define the autoencoder model
    autoencoder = models.Model(input_img, decoded)

    # Compile the autoencoder model using mean squared error as the loss function
    autoencoder.compile( loss='mean_squared_error',optimizer=RMSprop())

    # Train the autoencoder model
    history = autoencoder.fit(X_train, train_ground, batch_size, epochs, validation_data=(X_val, val_ground))

    # Evaluate the model on the validation set
    validation_loss = history.history['val_loss'][-1]

    # Print validation loss for the current combination
    print(f"Validation Loss (conv_layers = {conv_layers}, filter_size = {filter_size}, filters_per_layer = {filters_per_layer}, epochs={epochs}, batch_size={batch_size}): {validation_loss}")
    neural = models.Model(input_img,encoder)
    

    # Return the metrics instead of handling internally
    return {
        'X_test': images,
        'input_img': input_img,
        'conv_layers': conv_layers,
        'filter_size': filter_size,
        'filters_per_layer': filters_per_layer,
        'epochs': epochs,
        'batch_size': batch_size,
        'validation_loss': validation_loss,
        'best_model': neural
    }



# Main part of the code
def main():

    # Parse command line arguments
    parser = argparse.ArgumentParser(description='Reduce MNIST dataset.')
    parser.add_argument('-d', '--dataset', required=True, help='Path to the MNIST dataset file')
    parser.add_argument('-q', '--queryset', required=True, help='Path to the MNIST queryset file')
    parser.add_argument('-od', '--output_dataset_file', required=True, help='Path to the MNIST output dataset file')
    parser.add_argument('-oq', '--output_query_file', required=True, help='Path to the MNIST output queryset file')
    args = parser.parse_args()

    # Load MNIST images from the dataset and queryset files
    X = load_mnist_images(args.dataset)
    Y = load_mnist_images(args.queryset)

    # Convert MNIST images to NumPy arrays using idx2numpy
    X = idx2numpy.convert_from_file(args.dataset)
    Y = idx2numpy.convert_from_file(args.queryset)

    # Reshape the images to be 2D arrays
    X = X.reshape(X.shape[0], -1)
    Y = Y.reshape(Y.shape[0], -1)

    # Extract the base file names with extensions
    dataset_name = os.path.basename(args.dataset)
    queryset_name = os.path.basename(args.queryset)
    output_dataset_file =  os.path.basename(args.output_dataset_file)
    output_query_file = os.path.basename(args.output_query_file)

    # Asking the user if he wants to execute choice [1], [2] or [q] (quit program)

    # Display the menu
    print("Please select '1', '2' or 'Quit' from the following menu:")
    print("[1] Run the code using the best neural model straight forward (fastest).")
    print("[2] Run the code that tests many different parameters combinations, to extract the best neural model (takes many hours).")
    print("[q] Terminate the program and exit.")

    while True:

        # Get user input
        selection = input("Enter your selection ('1', '2', or 'q'): ")

        # Check user input and run the corresponding task or terminate
        if selection == '1':

            print("Executing code using best neural model...")

            # Fastest code using the best parameters combination for the neural model

            # Best parameters combination
            best_conv_layers=2
            best_filter_size=8 
            best_filters_per_layer=8 
            best_epochs=45
            best_batch_size=70
            latent_dim=10

            # Create ant train the neural model with the best parameters combination
            autoencoder_metrics = build_convolutional_autoencoder(
                                    best_conv_layers, best_filter_size,
                                    best_filters_per_layer, best_epochs, best_batch_size,
                                    latent_dim, dataset_filename=dataset_name)
            

            break  # Exit the loop if a valid selection is made

        elif selection == '2':

            print("Executing full code to find best neural model through parameters combinations tests...")
            print("This will take some time...")

            # Full code
            conv_layers=3
            filter_size=10 
            filters_per_layer=10 
            epochs=50 
            batch_size=100
            latent_dim=10

            # Collect metrics for each combination
            all_metrics = []

            for current_conv_layers in range(2, conv_layers):
                for current_filter_size in range (9, filter_size):
                    for current_filters_per_layer in range (2, filters_per_layer, 2):
                        for current_epochs in range (45, epochs, 5):
                            for current_batch_size in range (70, batch_size, 10):
                                
                                # Choosing the best NN parameters to produce the best dimension reduction
                                autoencoder_metrics = build_convolutional_autoencoder(
                                    current_conv_layers, current_filter_size,
                                    current_filters_per_layer, current_epochs, current_batch_size,
                                    latent_dim, dataset_filename=dataset_name)
                                
                                all_metrics.append(autoencoder_metrics)

            # Find the best model based on validation loss
            best_metrics = min(all_metrics, key=lambda x: x['validation_loss'])

            # Print the metrics of the best model
            print(f"\nBest Model - Test Loss: {best_metrics['validation_loss']}")
            print(f'''Best Combination - Conv_layers: {best_metrics['conv_layers']}, Filter size: {best_metrics['filter_size']}, 
                Filters per layer: {best_metrics['filters_per_layer']}, Epochs: {best_metrics['epochs']}, Batch Size: {best_metrics['batch_size']}''')

            break  # Exit the loop if a valid selection is made

        elif selection == 'q':

            print("Quitting program...")
            sys.exit()  # Terminate the program if 'quit' is entered

        else:

            print("Invalid selection. Please enter '1', '2', or 'q'.")


    # Encode the input data to get the reduced images
    input_img = layers.Input(shape=(28, 28, 1))
    x = input_img

    # Load MNIST images from the dataset file, convert to NumPy array, and preprocess
    d_images = load_mnist_images(dataset_name)
    d_images = np.array(d_images)
    d_images = d_images / 255.0
    d_images = np.expand_dims(d_images, axis=-1)

    # Load MNIST images from the queryset file, convert to NumPy array, and preprocess
    q_images = load_mnist_images(queryset_name)
    q_images = np.array(q_images)
    q_images = q_images / 255.0
    q_images = np.expand_dims(q_images, axis=-1)

    # Retrieve the best autoencoder model from the metrics
    neural = autoencoder_metrics['best_model']

    # Use the autoencoder to reduce the dimensionality of the input images
    reduced_images = neural.predict(d_images)
    reduced_q = neural.predict(q_images)

    # Normalize pixel values to be in the range [0, 255]
    reduced_images = (reduced_images * 255).astype(np.uint8)

    # Get the number of input images and reshape the reduced images accordingly
    n_in, _ = reduced_images.shape
    reduced_images = reduced_images.reshape((n_in, latent_dim, 1))

    # Normalize pixel values of queryset to be in the range [0, 255]
    reduced_q = (reduced_q * 255).astype(np.uint8)

    # Get the number of queryset images and reshape the reduced queryset accordingly
    n_q, _ = reduced_q.shape
    reduced_q = reduced_q.reshape((n_q, latent_dim, 1))

    # Save the reduced images to a new IDX file
    save_mnist_images(reduced_images, f"{output_dataset_file}", latent_dim, 1)

    # Save the reduced queryset images to a new IDX file
    save_mnist_images(reduced_q, f"{output_query_file}", latent_dim, 1)


if __name__ == "__main__":

    main()
