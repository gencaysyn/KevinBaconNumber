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
void  addElement(node**,node*); // Linkli listeye eleman eklemek için kullanýlýr listeyi ve eklenecek deðeri alýr.
void  printList (node*); // Listeyi yazdýrýr.
void  freeList  (char* ,node**); // Listeyi serbest býrakýr.
Info readFile(char* ); // Dosyayý okur ve verileri yapýya kaydeder.
Info initialize(int , int); // Film ve artistlerin saklanacaðý veriyi ilklendirir.
void deInitialize(Info*, int, int); // Film ve artistlerin saklandýðý yapýyý serbest býrakýr.
void bfs(Info, char[100], char[100]); // BFS iþleminin yapýldýðý kod kaynak ve hedef deðerlerini alýr ve sonucu yazdýrýr.
int getArtId(Info , char [100]); // Hash map'te isimle yapýlan aramanýn sonucunu döndürür.

node *filmRoot[MAX_FILM] ; // Filmlerin saklandýðý yapý.
node *artistRoot[MAX_ARTIST]; // Artistlerin saklandýðý yapý.

int main(){

	// Filmlerin ve Artistlerin isimlerinin tutulacaðý yapýnýn ilklendirilmesi
	int i;
	for(i = 0; i < MAX_ARTIST; i++){
		if(i < MAX_FILM)
			filmRoot[i] = NULL;
		artistRoot[i] = NULL;
	}
	
	//Dosya Okuma ve verilerin kaydedilmesi iþlemi
	Info info = readFile("input-mpaa.txt");
	
	char art1[100],art2[100];
	printf("   Kevin Bacon Number\n");
	printf("Please enter the first artist's name:");
	gets(art1);
	printf("Please enter the second artist's name:");
	gets(art2);

	bfs(info, art1, art2);
	
	deInitialize(&info, MAX_FILM, MAX_ARTIST);
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
	if(root == NULL){
		printf("list is empty!");
	}
	else{
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
	else{
		node *prev = *rootPtr;
		while(*rootPtr != NULL){
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
		if(last == '\n')str[strlen(str) -1] = '\0'; // Satýr sonlarýnýn stringe dahil edilmesine karþý önlem.
		
		char *each =  strtok(str,delim); // Veri '/' iþaretine göre elemanlara ayrýlýyor ilk eleman film diðer elemanlar is oyuncularý temsil ediyor.
		strcpy(info.films[info.filmCount],each);
		fRoot = (&filmRoot[info.filmCount]);
		addElement(fRoot,createNode(info.filmCount)); // Filmi linkli listeye ekleyen kýsým.
		each =  strtok(NULL,delim);
		int result;
		char buffer[100];
	
		// Filmi ekledikten sonra oyuncularý tekrar tekrar eklememek için önce bir kontrolden geçiyor yoksa ekleniyor.
		// Satýrda eleman kalmayýncaya dek bu iþlem devam ediyor.
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
	int result = search(info.artists,word);
	if(result == -1){
		printf("'%s' Artist Not Found!",word);
		exit(404);
	}
	return info.artists.node[result].idx;
}

void bfs(Info info, char source[100], char dest[100]){
	int visFilms[MAX_FILM];
	int visArtist[MAX_ARTIST];
	struct queue *qA = createQueue(); // Oyuncularýn tutulduðu kuyruk
	struct queue *qF = createQueue(); // Filmlerin tutulduðu kuyruk
	int element;
	int i;
	if(!strcmp(source,dest)){
		printf("%s\n",source);
		printf("Bacon Number is 0\n");
		return;
	}
	
	for(i=0;i<MAX_ARTIST;i++){ // Visited dizilerinin ilklendirilmesi.
		if(i<MAX_FILM){
			visFilms[i] = -1;
		}
		visArtist[i] = -1;
	}
	
	int s = getArtId(info,source);
	int d = getArtId(info,dest);
	visArtist[s] = s;
	node *iter = artistRoot[s]->next;
	
	while(iter != NULL){		
		enqueue(qF,iter->val);
		visFilms[iter->val] = s;
		iter = iter->next;
	}

	int counter = 1;
	while(counter < 7){ // Kevin Bacon sayýsýnýn sýnýrý.
		// Önce Filmler kuyruktan çekiliyor ve komþularý oyuncu kuyruðuna eklendikten sonra 
		// visited dizisinde hangi oyuncu tarafýndan iþaretlendiði tutularak iþaretleniyor.
		while(!isEmpty(qF)){
			element = dequeue(qF);
			iter = filmRoot[element]->next;
			while(iter!= NULL){
				if(visArtist[info.artists.node[iter->val].idx] == -1){
					enqueue(qA,info.artists.node[iter->val].idx);
					visArtist[info.artists.node[iter->val].idx] = element;
				}

				if(info.artists.node[iter->val].idx == d){	//Oyuncu bulunduðu zaman Deðer buraya düþer ve tersine giderek yazdýrma iþlemi yapýlýr
					printf("Bacon Number is %d\n",counter);
					
					int index = visArtist[d];
					int fac = 0;
					
					printf("Artist: %s\n", info.artists.node[artistRoot[d][0].val].val);	
					while(index != s){
						if(fac == 0){
							fac = 1;
							printf("Film: %s\n",info.films[index]);
							index = visFilms[index];
						}else{
							fac = 0;
							printf("Artist: %s\n", info.artists.node[artistRoot[index][0].val].val);
							index = visArtist[index];
						}
					}
					if(fac == 0)
						printf("Film: %s\n", info.films[visFilms[index]]);
					printf("Artist: %s\n",info.artists.node[artistRoot[s][0].val].val);
					
					return;
				}
				iter = iter->next;
			}
		}
		// Yukarda yapýlan iþlem tersine Artistler için yapýlýyor tek tek kuyruktan çekilip
		// Komþularý kaydediliyor ve kendisi hangi filmden geldiði bilgisiyle iþarteleniyor
		while(!isEmpty(qA)){
			element = dequeue(qA);
			iter = artistRoot[element]->next;
			while(iter != NULL){
				if(visFilms[iter->val] == -1){
					enqueue(qF,iter->val);
					visFilms[iter->val] = element;
				}
				iter = iter->next;
			}
		}
		counter++;
	}
}


