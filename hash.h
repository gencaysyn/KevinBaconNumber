#ifndef MY_HASH_H
#define MY_HASH_H

#include<stdlib.h>
#include<stdio.h>
#include<string.h>

int m = 350000;
int R = 349999;

struct Node{
	int key;
	int idx;
	char val[100];
};

struct Hash{
	int count;
	struct Node *node;
};

int h(int key,int i){
	int res = key % m;

	unsigned int tmp =  i*(R-(key % R));
/*	if( i*(R-(key % R)) < 0)
		printf("%lld \n", tmp);*/
	res += tmp % (m/2);
	return res % m;
}

struct Hash createHash(){
	struct Hash hash;
	hash.count = 0;
	hash.node = (struct Node*) malloc((m)*sizeof(struct Node));
	int i;
	for(i=0;i<m;i++){
		hash.node[i].key = -1;
	}
	return hash;
}

int generator(char str[100]){
	int i;
	int key = 0;
	for(i = 0; i<strlen(str); i++){
		key += str[i] * 26 *i;
	}
	return key;
}

void insertElement(struct Hash *hash, char str[100], int position){
	int i;
	int index;
	struct Node node;
	strcpy(node.val,str); 
	
	node.key = generator(str);
	i = -1;
	
	do{
		index = h(node.key,++i);
		
	}while(index >= 0 && (i < m) && (hash->node[index].key != -1));

	if(index >= 0 && i != m){
		hash->node[index].key = node.key;
		hash->node[index].idx = position;
		strcpy(hash->node[index].val,str); 
		hash->count++;
	}else{
		printf("%d %d %d %d\n", index, i, node.key, h(node.key,i));
		printf("Failed to add element! \"%s\" \n", str);
	}
	
}

int search(struct Hash hash, char str[100]){
	int i;
	int index;
	struct Node node;
	strcpy(node.val,str); 
	node.key = generator(str);
	
	i = -1;
	do{
		index = h(node.key,++i);
	}while(index >= 0 && (hash.node[index].key != -1) && strcmp(hash.node[index].val, str) != 0);

	if(index >= 0 && strcmp(hash.node[index].val, str) == 0){
		return index;
	}
	return -1;
}
	
void printTable(struct Hash table){
	int i=0;
	printf("  TABLE\n");
	while(i < m){
		for(i = 0; i<m; i++){
			if(table.node[i].key != -1){
				printf("%d %d %s\n",i,table.node[i].key,table.node[i].val);
			}
		}
	}
}
#endif 
