#include "Hashtable.h"

using namespace std;



HashTable htInitialize(int buckets) {//init hashtable
	HashTable ht=new hashtable_head;
	ht->table=new hashtable_node[buckets];
	ht->buckets=buckets;
  	for (int i=0;i<buckets;i++){
		ht->table[i].key=i;
		ht->table[i].num_img=0;
 	}
	ht->numberOfVectors=0; //num of images in hash table
	return ht;
}

void htInsert(HashTable ht, image *v,int index,int id){		//insert images to the hash table as well as the id of each image
	ht->table[index].labels.push_back(v);
	ht->table[index].id.push_back(id);
	ht->table[index].num_img++;
  	ht->numberOfVectors++;
}

