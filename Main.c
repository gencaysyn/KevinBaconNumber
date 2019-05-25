#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include"hash.h"

#define MAX_FILM 15000
#define MAX_ARTIST 180000
struct info{
	char **films;
	struct Hash artists;
	int filmCount; 
	int artCount;
};
typedef struct info Info;
typedef struct s
{
	int val;
	struct s *next;
}node;

node* createNode(int);
void  addElement(node**,node*);
void  printList (node*);
void  freeList  (char* ,node**);
Info readFile(char* );
Info initialize(int , int);
void deInitialize(Info*, int, int);
int isContains(char**,int,char*);

node *filmRoot[MAX_FILM] ;
node *artistRoot[MAX_ARTIST];

int main(){
	
	int i;
	for(i = 0; i < MAX_ARTIST; i++){
		if(i < MAX_FILM)
			filmRoot[i] = NULL;
		artistRoot[i] = NULL;
	}
	Info info = readFile("input-mpaa.txt");
	//printTable(info.artists);
	/*for(i = 0; i < MAX_ARTIST; i++){
		if(i < MAX_FILM && strlen(info.films[i]) > 0){
			printf("film %s: ",info.films[i]);
			printList(filmRoot[i]);
		}
		if(artistRoot[i] != NULL){
			printf("artist %s : ", info.artists.node[artistRoot[i][0].val].val);	
			printList(artistRoot[i]);	
		}
		
	}*/

	return(0);
}

node* createNode(int val)
{
	node *tmp; 
	tmp = (node*)malloc(sizeof(node));
	if(tmp == NULL)
	{
		printf("bellek ayrilamadi!\n");
		exit(1);
	}
	tmp->val  = val;
	tmp->next = NULL;
	return tmp;
}

void addElement(node **rootPtr, node *newNode)
{
	if(*rootPtr == NULL)
	{
		*rootPtr = newNode;
	}
	else
	{
		node *i = (*rootPtr);
		while(i->next != NULL){
			i = i->next;	
		}
		i->next = newNode;
	}
}

void printList(node *root)
{
	if(root == NULL)
	{
		printf("list is empty!");
	}
	else
	{
		node *i = root;
		while(i != NULL)
		{
			printf("%d ", i->val);
			i = i->next;
		}
	}
	printf("\n");
}

void freeList(char *name, node **rootPtr)
{
	if(*rootPtr == NULL)
		printf(" %s listesi zaten bos!\n", name);
	else
	{
		node *prev = *rootPtr;
		while(*rootPtr != NULL)
		{
			*rootPtr = (*rootPtr)->next;
			free(prev);
			prev = *rootPtr;	
		}
		printf(" %s listesi bosaltildi!\n", name);	
	}	
	
}

Info initialize(int maxFilm, int maxArtist){
	Info info;
	info.artCount = 0;
	info.filmCount = 0;

	info.artists = createHash();	
	info.films = (char**) malloc(maxFilm*sizeof(char*));
	if(info.films == NULL)
			printf("Malloc Error!");
	
	int i = 0;
	for(;i<maxArtist;i++){
		if(i<maxFilm){
			info.films[i] = (char*) calloc(100, sizeof(char));
			if(info.films[i] == NULL)
				printf("MallocFilms Error!");
		}
	}
	return info;
}

void deInitialize(Info *info, int maxFilm, int maxArtist){
	int i = 0;
	for(;i<maxArtist;i++){
		if(i<maxFilm){
			free(info->films[i]);
		}
	}
	free(info->films);
}

int find(char** array ,int N,char* word){
	int i = 0;
	while(i<N && strcmp(array[i],word))
		i++;
	if(i == N)
		return -1;
	return i-1;
}

Info readFile(char* fileName){
	FILE * fp;
	Info info = initialize(MAX_FILM,MAX_ARTIST);
	fp = fopen (fileName, "r");

	if(!fp)
		printf("%s File not found!",fileName);
	
	char *delim = "/";
	char str[3800];
	node **fRoot, **aRoot;
	while(!feof(fp)){
		
		fgets(str,sizeof(str),fp);
		char last = str[strlen(str) -1];
		if(last == '\n')str[strlen(str) -1] = '\0';
		
		char *each =  strtok(str,delim);
		strcpy(info.films[info.filmCount],each);
		fRoot = (&filmRoot[info.filmCount]);
		addElement(fRoot,createNode(info.filmCount));
		each =  strtok(NULL,delim);
		int result;
		char buffer[100];
	
		while(each != NULL){
			strcpy(buffer,each);
			result = search(info.artists,buffer);
			aRoot = (&artistRoot[info.artCount]);
			if(result == -1){
				insertElement(&info.artists, buffer, info.artCount);
				int id = search(info.artists,buffer);
				addElement(fRoot,createNode(id));
				addElement(aRoot,createNode(id));
				addElement(aRoot,createNode(info.filmCount));
				info.artCount++;
			}else{
				aRoot = (&artistRoot[info.artists.node[result].idx]);
				addElement(aRoot,createNode(info.filmCount));
				addElement(fRoot,createNode(result));
			}				
			each =  strtok(NULL,delim);
		}
		info.filmCount++;
	}
	fclose(fp);
	return info;
}



