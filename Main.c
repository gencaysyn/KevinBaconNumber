#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include"hash.h"
#include"queue.h"
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
//int isContains(char**,int,char*);
void bfs(Info, char[100], char[100]);
int getArtId(Info , char [100]);

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
			printf("film[%d] %s: ",i,info.films[i]);
			printList(filmRoot[i]);
		}
		if(artistRoot[i] != NULL){
			printf("artist[%d] %s : ",i, info.artists.node[artistRoot[i][0].val].val);	
			printList(artistRoot[i]);	
		}
		
	}*/
	bfs(info, "Pitt, Brad","Pitt, Brad");
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
				printf("CallocFilms Error!");
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

int getArtId(Info info, char word[100]){
	return info.artists.node[search(info.artists,word)].idx;
}

void bfs(Info info, char source[100], char dest[100]){
	int visFilms[MAX_FILM];
	int visArtist[MAX_ARTIST];
	struct queue *qA = createQueue();
	struct queue *qF = createQueue();
	int element;
	int i;
	printf("s:%s, d:%s\n",source,dest);
	if(!strcmp(source,dest)){
		printf("%s\n",source);
		printf("Bacon Number is 0\n");
		return;
	}
	
	for(i=0;i<MAX_ARTIST;i++){
		if(i<MAX_FILM){
			visFilms[i] = 0;
		}
		visArtist[i] = 0;
	}
	
	int s = getArtId(info,source);
	int d = getArtId(info,dest);
	visArtist[s] = 1;
	
	node *iter = artistRoot[s]->next;
	
	while(iter != NULL){		
		enqueue(qF,iter->val);
		visFilms[iter->val] = 1;
		iter = iter->next;
	}
	
	
	printf("\ns:%d, d:%d\n",s,d);
	int counter = 1;
	while(1){
		while(!isEmpty(qF)){
			element = dequeue(qF);
			iter = filmRoot[element]->next;
			while(iter!= NULL){
				if(visArtist[info.artists.node[iter->val].idx] == 0){
					enqueue(qA,info.artists.node[iter->val].idx);
					visArtist[info.artists.node[iter->val].idx] = counter;
				}

				if(info.artists.node[iter->val].idx == d){
					printf("Bulduk!");
					printf("counter:%d",counter);
					exit(1);
				}
				iter = iter->next;
			}
		}
		while(!isEmpty(qA)){
			element = dequeue(qA);
			iter = artistRoot[element]->next;
			while(iter != NULL){
				if(visFilms[iter->val] == 0){
					enqueue(qF,iter->val);
					visFilms[iter->val] = counter;
				}
				iter = iter->next;
			}
		}
		counter++;
	}
}


