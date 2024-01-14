#include <cstdlib> 
#include <string>
#include <vector>
#include<iostream>
#include<fstream>
#include<cmath>
#include<climits>
#include <algorithm>
#include <cstring>
#include <ctime>

using namespace std;

struct Node{
	vector<int> key;  // the key of the map 
	vector<int> value;  // the value that correspons to the key 
	int count;
};  

struct HashHeadNode{
	int size;   // the size of the hashTable
	int count;    //how many keys/values are in the HashTable
	Node *table;   //the HashTable
};
    
typedef HashHeadNode *HashMap;  

HashMap hmCreate(int size);

int hmSearchOrInsert(HashMap ht,int key,int value);