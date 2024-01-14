<!-- PROJECT LOGO -->
<br />
<div align="center">
    <img src="resources/logo.png" alt="Logo" width="170" height="170">
    <br />
  </a>

  <h3 align="center">Software Development for Algorithmic Problems (K23γ) | Project 2023-2024</h3>
  <h3 align="center">part A</h3>
  
  <p align="center">
    LSH & Hyper Cube
    <br />
  </p>
</div>

## Introduction

This is the part A of algorithmic implementations. In this assignment we created the LSH algorythm and Hypercube algorythm in order to find the approximate N nearest neighbours to an image query

## Files & functions used

Below lies a concise description of the files and functions used in this part of the assignment 1.

<br><br>

```c++
mainLSH.cpp
```

It is the main source code file that includes the main() function for the LSH algorythm. It initializes the parameters given by the user.if a certain parameter left out empty (not given from the command line) it uses the default values.

<br><br>

```c++
LSH.cpp
```
It includes all the functions that are vital in the initialization and in the inserion of the images taken by the inpute file to the LSH node.

<br><br>
1.
```c++
image* file_manip(char *fname,int *image_num);
```
&emsp;Reads the given imput file or query file and creates the image array.

<br><br>
2.
```c++
void initializeLSH(int l,lshNode *lsh);
 ```
&emsp;Initializes the lsh node by creating l g functions and l hash tables.

<br><br>

3.
```c++
void generateG(g_function *gfun);
```
&emsp;Generates the approrpiate values for the g function( creates k h_functions and k r numbers r1h1(p) + r2h2(p) + · · · + rk hk (p).

<br><br>
4.
```c++
void generateH(h_function *hfun);
```
&emsp;Generates v array cordinates(the dimention is same to the image cordinates).

<br><br>
5.
```c++
HashTable htInitialize(int buckets);
```
&emsp;Initializes the hash tables that are going to be used(the function is taken from the `Hashtable.cpp` file).

<br><br>
6.
```c++
void insertToLSH(lshNode *lsh,image *v);
```
&emsp;Inserts images to the LSH node form image array.

<br><br>
7.
```c++
temp_nums computeG(g_function gfun,image *p);
```
&emsp;Calculates the value of g_function and returns a struct containing the id and index of hash table.

<br><br>
8.
```c++
int computeH(h_function hfun,image *im);
```
&emsp;Computes the value of each h_function of a certain image(according to the type given to us).

<br><br>
9.
```c++
void htInsert(HashTable ht, image *v,int index,int id);
```
&emsp;Inserts each image to the hash table(the function is taken from the `Hashtable.cpp` file).

<br><br>
10.
```c++
void query(lshNode *lsh,image *query_list,image *img_list,int n,double radius,int q,char *output,int img_num,int count);
```
&emsp;Takes the random query and calculates the N approximate neighbours,the neighbours in radius and writes the stats in the output file.



<br><br>

```c++
mainHC.cpp
```

It is the main source code file that includes the main() function for the HC algorythm. It initializes the parameters given by the user.if a certain parameter left out empty (not given from the command line) it uses the default values.

<br><br>

```c++
HyperCube.cpp
```
It includes all the functions that are vital in the initialization and in the inserion of the images taken by the inpute file to the HC node.

<br><br>
1.
```c++
image* file_manip(char *fname,int *image_num);
```
&emsp;Reads the given imput file or query file and creates the image array.

<br><br>
2.
```c++
void initializeHyperCube(int k,cubeNode *hc);
 ```
&emsp;Initializes the HC node by creating k h functions and k f functions.

<br><br>
3.
```c++
void generateH_Cube(h_function *hfun);
```
&emsp;Generates v array cordinates(the dimention is same to the image cordinates).

<br><br>
4.
```c++
HashMap hmCreate(int size);
```
&emsp;Creates a hashmap with key the number taken form the h function and value 0 or 1(the function is taken from the `HashMap.cpp` file).

<br><br>
5.
```c++
HashTable htInitialize(int buckets);
```
&emsp;Initializes the hash table that is going to be used(the function is taken from the `Hashtable.cpp` file).

<br><br>
6.
```c++
void insertToHyperCube(cubeNode *hc,image *v);
```
&emsp;Inserts images to the HC node form image array.

<br><br>
7.
```c++
int computeH_Cube(h_function hfun,image *im);
```
&emsp;Computes the value of each h_function of a certain image(according to the math type given to us).

<br><br>
8.
```c++
int computeF(HashMap f_fun,int key);
```
&emsp;Computes the value of the f functions.Each f function value is taken and bieng put to an array with size k then we convert that into a decimal number witch is the index of the hashtable

<br><br>
9.
```c++
int hmSearchOrInsert(HashMap ht,int key,int value);
```
&emsp;It searches the hash map.if key does exist then we take the value.If not we put the key int the vector as well as its value(the function is taken from the `HashMap.cpp` file).

<br><br>
10.
```c++
void query(cubeNode *hc,image *query_list,image *img_list,int n,double radius,int hammingDist,int m,int q,char *output,int img_num,int count);
```
&emsp;Takes the random query and calculates the N approximate neighbours,the neighbours in radius and writes the stats in the output file.

<br><br>
11.
```c++
void HammingDistance(cubeNode *hc,image *img_list,int *index,int hammingDist,int startFrom,vector<temp_nums> *sort_ims,int *numOfSearched,int m,int n,int *nodesSearched,int probes,int q);
```
&emsp;If searched<M we must check the neighbours according to the hamming distance.This recursive function checks neighbours with hamm dist 1 to probes until searched=M or probes = nodeschecked.

<br>

## Compilation and execution

- Using the `make LSH` command, linking and compilation of all source files and header files are performed.

- With the command `./lsh –d <input file> –q <query file> –k <int> -L <int> -ο <output file> -Ν <number of nearest> -R <radius>`, the LSH algorythm is executed.

- Using the `make HC` command, linking and compilation of all source files and header files are performed.

- With the command `./cube –d <input file> –q <query file> –k <int> -M <int> -probes <int> -ο <output file> -Ν <number of nearest> -R <radius>`, the HC algorythm is executed.
