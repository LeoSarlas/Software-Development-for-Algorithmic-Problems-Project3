#include "HashMap.h"


HashMap hmCreate(int size){ //  create/initialize the hash table
	HashMap head=new HashHeadNode;  
	head->size=size;    //store the given size of the hash table
	head->table=new Node[size];   //create a hash table of the given size
	for(int i=0;i<head->size;i++){
		head->table[i].count=0;
	}
	head->count=0;
	return head;
}

int hmSearchOrInsert(HashMap ht,int key,int value){
	int index = key % ht->size;
	int result = (index < 0) ? (index + ht->size) : index;
	index = result;
	if(ht->table[index].key.empty()){   //no collitions so just insert the node 
		ht->table[index].key.push_back(key);
		ht->table[index].value.push_back(value);
		ht->table[index].count++;
		ht->count++;
		return value;
	}else{    //if there are already key/value sets in this "bucket"
		for(int i=0;i<ht->table[index].count;i++){
			if(ht->table[index].key[i]==key){    //check if the key already exists
				return ht->table[index].value[i];
			}
		}
		//if it doesnt exist push back the key and value 
		ht->table[index].key.push_back(key);
		ht->table[index].value.push_back(value);
		ht->table[index].count++;
		ht->count++;
		return value;
	}
}