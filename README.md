<a name="readme-top"></a>

<!-- PROJECT SHIELDS --> <!---->
[![Contributors](https://img.shields.io/badge/Contributors-2-brightgreen?style=for-the-badge&logo=width=150&height=50)][contributors-url]

<!-- PROJECT LOGO -->
<br />
<div align="center">
    <img src="resources/logo.png" alt="Logo" width="170" height="170">
    <br />
  </a>

  <h3 align="center">Software Development for Algorithmic Problems (K23γ) | Project 2023-2024</h3>

  <p align="center">
    Dimension reduction using Neural Networks
    <br />
  </p>
</div>


<!-- ABOUT THE PROJECT -->
## About The Project

This is the 3rd of the total 3 assignments we came to deal with, during the NKUA Project Development Course (K23a) "Software Development for Algorithmic Problems", during
the winter semester of 2023-2024.

In this assignment, we dealt with:

* Searching for the best parameters of a neural network, in order to reduce the dimension of the MNIST images dataset.
* Testing the results quality of the algorithms in Projects [1] (https://github.com/LeoSarlas/Software-Development-for-Algorithmic-Problems-Project1) & [2] (https://github.com/LeoSarlas/Software-Development-for-Algorithmic-Problems-Project2) when the MNIST images of reduced dimension is feeded in these algorithms.

<p align="right">(<a href="#readme-top">back to top</a>)</p>


### Built with

This project is primarily built using:

* <img src="resources/python.png" alt="Python Logo" width="40" height="40" align="center"> : The main programming language for the Project 3.
* <img src="resources/c++.png" alt="C++ Logo" width="40" height="40" align="center"> : The main programming language for the Project 1 & 2.
* <img src="resources/gnu.jpg" alt="Make Logo" width="40" height="40" align="center"> : Used for compiling and building the project.


<p align="right">(<a href="#readme-top">back to top</a>)</p>



<!-- GETTING STARTED -->
## Getting Started

Below, you can see the folders in which this assignment is partitioned into:

### partA__LSH-implementation

&emsp;Contains Vectors comparison and Locality-Sensitive Hashing (LSH) implementations. For more details, please read the
appropriate README file located in there.

### partB__clustering-algorithms

&emsp; Contains vectors clustering implementations. For more details, please read the appropriate README file located in there.

## Download

Clone the repo
   ```sh
   git clone https://github.com/LeoSarlas/Software-Development-for-Algorithmic-Problems-Project1.git
   ```

<p align="right">(<a href="#readme-top">back to top</a>)</p>



<!-- USAGE EXAMPLES -->
## Compilation and execution

Depending on the part of the project you want to compile and execute, head into the appropriate folder.

### part A

Eexecute this terminal command to compile LSH

    make LSH
&emsp; LSH algorithm

    ./lsh –d <input file> –q <query file> –k <int> -L <int> -ο <output file> -Ν <number of nearest> -R <radius>
    
Execute this terminal command to compile HC

    make HC

&emsp; HC algorithm

    ./cube –d <input file> –q <query file> –k <int> -M <int> -probes <int> -ο <output file> -Ν <number of nearest> -R <radius>


### part B

There, execute this terminal command to compile

    make cluster

and one of these commands, depending on which algorithm/method of clustering you wish to test

&emsp; Classic Lloyd's algorithm

    ./cluster -i input.idx3-ubyte -c cluster.conf -o Classic_output_file.txt -m Classic -complete

&emsp; Reverse search by LSH method

    ./cluster -i input.idx3-ubyte -c cluster.conf -o LSH_output_file.txt -m LSH -complete

&emsp; Reverse search by random projection (Hybercube) method

    ./cluster -i input.idx3-ubyte -c cluster.conf -o Random_projection_output_file.txt -m Hypercube -complete
    

Keep in mind that the ***[-complete]*** parameter is optional, and determines whether the log file will print results
concisely or analytically.
 
_For more information, please refer to the [README](https://github.com/LeoSarlas/Software-Development-for-Algorithmic-Problems-Project1/blob/main/partB__clustering-algorithms/README.txt) file.

<p align="right">(<a href="#readme-top">back to top</a>)</p>


<!-- CONTACT -->
## Contact the creators

Alex Vekris - [@_kryptias](https://twitter.com/_kryptias) - [GitHub](https://github.com/alexvek96)

Leo Sarlas - [GitHub](https://github.com/LeoSarlas)

Project Link: [https://github.com/your_username/repo_name](https://github.com/your_username/repo_name)

<p align="right">(<a href="#readme-top">back to top</a>)</p>


<!-- ACKNOWLEDGEMENTS -->
## Acknowledgements

Many regards to:

* [MNIST Dataset by Kaggle](https://www.kaggle.com/datasets/hojjatk/mnist-dataset)

<p align="right">(<a href="#readme-top">back to top</a>)</p>


<!-- MARKDOWNS & LINKS -->
[contributors-shield]: https://img.shields.io/github/contributors/othneildrew/Best-README-Template.svg?style=for-the-badge

[contributors-url]: https://github.com/LeoSarlas/Software-Development-for-Algorithmic-Problems-Project1/graphs/contributors
