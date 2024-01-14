<a name="readme-top"></a>

<!-- PROJECT SHIELDS --> <!---->
[![Contributors](https://img.shields.io/badge/Contributors-2-brightgreen?style=for-the-badge&logo=width=150&height=50)][contributors-url]

<!-- PROJECT LOGO -->
<br />
<div align="center">
    <img src="resources/neural.png" alt="Logo" width="400" height="240">
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

* First, we are searching for the best parameters of a neural network, in order to reduce the dimension of the MNIST images dataset.
* Then, we test the results quality of the algorithms in Projects [1-modified](https://github.com/LeoSarlas/Software-Development-for-Algorithmic-Problems-Project3/tree/main/Project-1-modified) & [2-modified](https://github.com/LeoSarlas/Software-Development-for-Algorithmic-Problems-Project3/tree/main/Project-2-modified) when the MNIST images of reduced dimension from the NN are feeded in these algorithms.

<p align="right">(<a href="#readme-top">back to top</a>)</p>


### Built with

This project is primarily built using:

* <img src="resources/python.png" alt="Python Logo" width="40" height="40" align="center"> : The main programming language for the Project 3.
* <img src="resources/c++.png" alt="C++ Logo" width="40" height="40" align="center"> : The main programming language for the Projects 1 & 2.
* <img src="resources/gnu.jpg" alt="Make Logo" width="40" height="40" align="center"> : Used for compiling and building the Projects 1 & 2.


<p align="right">(<a href="#readme-top">back to top</a>)</p>



<!-- GETTING STARTED -->
## Getting Started - Download

Clone the repo
   ```sh
   git clone https://github.com/LeoSarlas/Software-Development-for-Algorithmic-Problems-Project3.git
   ```

<p align="right">(<a href="#readme-top">back to top</a>)</p>


<!-- USAGE EXAMPLES -->
## Compilation and execution

* First, execute the main code through the command
  
          python3 reduce.py -d MNIST_60k.idx3-ubyte -q MNIST_10k.idx3-ubyte -od NN_MNIST_60k.idx3-ubyte -oq NN_MNIST_10k.idx3-ubyte
    or

          python3 reduce.py -d MNIST_60k.idx3-ubyte -q MNIST_10k.idx3-ubyte -od NN_MNIST_60k.idx3-ubyte -oq NN_MNIST_10k.idx3-ubyte

* After the datasets 'NN_MNIST_60k.idx3-ubyte' (input file) and 'NN_MNIST_10k.idx3-ubyte' (query file) are created, use them as input parameters in the terminal commands of the previous Projects 1 & 2:
<br><br>
>  LSH

Eexecute this terminal command to compile LSH

            make LSH
&emsp; LSH algorithm

        ./lsh –d <input file> –q <query file> –k <int> -L <int> -ο <output file> -Ν <number of nearest> -R <radius>
<br>

>  HyperCube

Execute this terminal command to compile HC

            make HC

&emsp; HC algorithm

        ./cube –d <input file> –q <query file> –k <int> -M <int> -probes <int> -ο <output file> -Ν <number of nearest> -R <radius>

<br><br>

** Lloyd's - Reverse search by LSH - Reverse search by random projection (Hybercube)

There, execute this terminal command to compile

          make cluster

and one of these commands, depending on which algorithm/method of clustering you wish to test
<br><br><br>

   >    Classic Lloyd's algorithm

        ./cluster -i input.idx3-ubyte -c cluster.conf -o Classic_output_file.txt -m Classic -complete
<br>

   >    Reverse search by LSH method

        ./cluster -i input.idx3-ubyte -c cluster.conf -o LSH_output_file.txt -m LSH -complete
<br>

   >    Reverse search by random projection (Hybercube) method

        ./cluster -i input.idx3-ubyte -c cluster.conf -o Random_projection_output_file.txt -m Hypercube -complete
<br>    

Keep in mind that the ***[-complete]*** parameter is optional, and determines whether the log file will print results
concisely or analytically.
 
For more information, please refer to the [README](https://github.com/LeoSarlas/Software-Development-for-Algorithmic-Problems-Project3/blob/main/README.txt](https://github.com/LeoSarlas/Software-Development-for-Algorithmic-Problems-Project3/blob/main/README.pdf) file.

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
