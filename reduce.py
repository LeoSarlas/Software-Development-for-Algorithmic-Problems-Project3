#   python3 reduce.py -d MNIST_60k.idx3-ubyte -q MNIST_10k.idx3-ubyte -od <output_dataset_file> -oq <output_query_file>

import argparse                                         # For handling command-line arguments
import os
os.environ['TF_CPP_MIN_LOG_LEVEL'] = '3'                                               # For working with file paths and directories
from sklearn.model_selection import train_test_split    # For splitting the dataset
import idx2numpy                                        # For working with binary files in the IDX format
import numpy as np                                      # NumPy library for numerical operations
import struct                                           # For handling binary data and structuring data in memory
import tensorflow as tf                                 # Open-source ML library
from tensorflow.keras import layers, models             # For constructing neural network models
import sys


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
        
        # rows, cols = num_images, flattened_size // (28 * 28)
        # rows = 28
        # cols = 28
        images = images.reshape(num_images, rows, cols)

    else:

        raise ValueError("Invalid shape for 'images' array. It must be either 2D or 3D.")

    # Normalize pixel values to the range [0, 255] and convert to uint8
    # images = (images * 255.0).astype(np.uint8)

    # Open the file in binary write mode
    with open(filename, 'wb') as file:

        # Write the magic number, number of images, rows, and columns
        file.write(struct.pack('>IIII', 2051, num_images, rows, cols))

        # Write the image data to the file
        images.tofile(file)



# Function to load MNIST images from a file in IDX format

# def load_mnist_images(filename):

#     # Open the file in binary read mode
#     with open(filename, 'rb') as file:

#         # Read the magic number, number of images, rows, and columns
#         magic, num_images, rows, cols = struct.unpack('>IIII', file.read(16))
#         print(num_images, rows, cols)

#         # Read the image data
#         image_data = np.fromfile(file, dtype=np.uint8)

#     # Check if the image_data size is consistent with the specified dimensions
#     expected_size = num_images * rows * cols

#     # Adjust the condition to check for an exact match
#     if image_data.size != expected_size:
#         raise ValueError(f"Invalid size for 'image_data'. Expected {expected_size}, got {image_data.size}.")

#     # Reshape the image data to the expected shape
#     # Adjust the shape to (num_images, 28, 28) based on your previous reshaping logic
#     image_data = image_data.reshape(num_images, rows, cols)

#     return image_data


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

    # Split the data into training and testing sets
    X_train, X_test = train_test_split(image_data, test_size=0.2, random_state=42)

    return X_train, X_test





# Function to delete existing files with a specified prefix
def delete_existing_files(prefix):

    # Iterate through files in the current directory
    for file_name in os.listdir('.'):

        # Check if the file has the specified prefix
        if file_name.startswith(prefix):

            # Remove the file
            os.remove(file_name)


def generate_valid_shape():

    while True:

        # Generate random values for the dimensions
        rows = np.random.randint(1, 6)
        cols = np.random.randint(1, 6)
        filters_per_layer = np.random.randint(1, 6)

        # Check if the product is equal to 10
        if rows * cols * filters_per_layer == 10:
            return (rows, cols, filters_per_layer)
        

# Function to build a convolutional autoencoder model
def build_convolutional_autoencoder(conv_layers, filter_size, filters_per_layer, latent_dim, epochs, batch_size, dataset_filename):
    
    # Load the MNIST dataset from the provided filename
    (X_train, X_test) = load_mnist_images(dataset_filename)
    
    # Check if the dataset is valid
    if (X_train is None) or (X_test is None):
        print("Error! Invalid dataset.\n")
        return None

    # Convert the dataset to a NumPy array using idx2numpy
    # dataset = idx2numpy.convert_from_file(dataset_filename)


    # Convert the dataset to a NumPy array if not already
    X_train, X_test = np.array(X_train), np.array(X_test)
    
    # Unpack the dataset into training and testing sets
    # (X_train, X_test) = dataset
    # (X_train, _), (X_test, _) = dataset

    # Normalize pixel values to be in the range [0, 1]
    X_train, X_test = X_train / 255.0, X_test / 255.0

    # Add a channel dimension for convolutional layers
    X_train = np.expand_dims(X_train, axis=-1)
    X_test = np.expand_dims(X_test, axis=-1)

    # Split the data into training, validation, and testing sets
    X_train, X_val, _, _ = train_test_split(X_train, X_train, test_size=0.2, random_state=42)

    # Define the input layer for the autoencoder model
    input_img = layers.Input(shape=(28, 28, 1))
    x = input_img

    best_validation_loss = float('inf')
    best_model = None
    best_epochs = None
    best_batch_size = None
    ep = 2
    bc = 15
    # Iterate through different combinations of epochs and batch sizes
    for current_epochs in range(epochs):
        # ep = current_epochs+1
        for current_batch_size in range(batch_size):
            # bc = current_batch_size+1
            # Build the encoder layers
            for _ in range(conv_layers):
                x = layers.Conv2D(filters=filters_per_layer, kernel_size=(filter_size, filter_size), activation='relu', padding='same')(x)
                x = layers.MaxPooling2D((2, 2), padding='same')(x)

            # Flatten the encoded representation
            x = layers.Flatten()(x)
            encoded = layers.Dense(latent_dim, activation='relu')(x)

            # (rows, cols, filters_per_layer) = generate_valid_shape()

            # Build the decoder layers
            x = layers.Reshape((1, 1, 10))(encoded)  # Adjust reshaping

            # for _ in range(conv_layers):
            #     x = layers.Conv2D(filters=10, kernel_size=(filter_size, filter_size), activation='relu', padding='same')(x)
            #     x = layers.UpSampling2D((2, 2))(x)
            if(conv_layers==1):
                x = layers.Conv2D(filters=10, kernel_size=(filter_size, filter_size), activation='relu', padding='same')(x)
                x = layers.UpSampling2D((28, 28))(x)
            elif(conv_layers==2):
                x = layers.Conv2D(filters=10, kernel_size=(filter_size, filter_size), activation='relu', padding='same')(x)
                x = layers.UpSampling2D((7, 7))(x)
                x = layers.Conv2D(filters=10, kernel_size=(filter_size, filter_size), activation='relu', padding='same')(x)
                x = layers.UpSampling2D((4, 4))(x)
            elif(conv_layers==3):
                x = layers.Conv2D(filters=10, kernel_size=(filter_size, filter_size), activation='relu', padding='same')(x)
                x = layers.UpSampling2D((2, 2))(x)
                x = layers.Conv2D(filters=10, kernel_size=(filter_size, filter_size), activation='relu', padding='same')(x)
                x = layers.UpSampling2D((2, 2))(x)
                x = layers.Conv2D(filters=10, kernel_size=(filter_size, filter_size), activation='relu', padding='same')(x)
                x = layers.UpSampling2D((7, 7))(x)
            # Output layer of the autoencoder
            decoded = layers.Conv2D(1, (filter_size, filter_size), activation='relu', padding='same')(x)

            # Define the autoencoder model
            autoencoder = models.Model(input_img, decoded)

            # Compile the autoencoder model using mean squared error as the loss function
            autoencoder.compile(optimizer='adam', loss='mean_squared_error')

            # Train the autoencoder model
            history = autoencoder.fit(X_train, X_train, epochs=ep, batch_size=bc, validation_data=(X_val, X_val))
            # print(history.history)
            # Evaluate the model on the validation set
            validation_loss = history.history['val_loss'][-1]

            # Print validation loss for the current combination
            print(f"Validation Loss (epochs={ep}, batch_size={bc}): {validation_loss}")
            bc+=1
            # Check if the current model has a lower validation loss
            if validation_loss < best_validation_loss:
                best_validation_loss = validation_loss
                best_model = autoencoder
                best_epochs = ep
                best_batch_size = bc
        ep+=1
    # Evaluate the best model on the test set
    test_loss = best_model.evaluate(X_test, X_test)
    print(f"\nBest Model - Test Loss: {test_loss}")
    print(f"Best Combination - Epochs: {best_epochs}, Batch Size: {best_batch_size}")

    # Return the best trained autoencoder model
    return best_model






# Main part of the code
def main():

    # Parse command line arguments
    parser = argparse.ArgumentParser(description='Reduce MNIST dataset.')
    parser.add_argument('-d', '--dataset', required=True, help='Path to the MNIST dataset file')
    parser.add_argument('-q', '--queryset', required=True, help='Path to the MNIST queryset file')
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

    # Split the dataset and queryset into training and validation sets (80% training, 20% validation)
    X_train, X_validation = train_test_split(X, test_size=0.2, random_state=42)
    Y_train, Y_validation = train_test_split(Y, test_size=0.2, random_state=42)

    # Extract the base file names without extensions
    dataset_name = os.path.splitext(os.path.basename(args.dataset))[0]
    queryset_name = os.path.splitext(os.path.basename(args.queryset))[0]

    # Delete existing files with the specified prefix
    delete_existing_files(f"{dataset_name}_80%_training")
    delete_existing_files(f"{queryset_name}_80%_training")
    delete_existing_files(f"{dataset_name}_20%_validation")
    delete_existing_files(f"{queryset_name}_20%_validation")
    
    # Save the training and validation subsets into IDX files
    save_mnist_images(X_train, f"{dataset_name}_80%_training.idx3-ubyte",28,28)
    save_mnist_images(Y_train, f"{queryset_name}_80%_training.idx3-ubyte",28,28)
    save_mnist_images(X_validation, f"{dataset_name}_20%_validation.idx3-ubyte",28,28)
    save_mnist_images(Y_validation, f"{queryset_name}_20%_validation.idx3-ubyte",28,28)

    # Define file paths for training and validation datasets and querysets
    training_dataset = f"{dataset_name}_80%_training.idx3-ubyte"
    training_queryset = f"{queryset_name}_80%_training.idx3-ubyte"
    validation_dataset = f"{dataset_name}_20%_validation.idx3-ubyte"
    validation_queryset = f"{queryset_name}_20%_validation.idx3-ubyte"

    # Build a convolutional autoencoder model using the training dataset
    autoencoder_model = build_convolutional_autoencoder(
        conv_layers=3, filter_size=5, filters_per_layer=5, latent_dim=10, epochs=9, batch_size=15, dataset_filename=training_dataset
    )

if __name__ == "__main__":

    main()
