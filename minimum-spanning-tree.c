#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* Define Edge Type Struct That Contains the Vertices Connected and it's Associated Weight */
typedef struct edge {
	int v1;
	int v2;
	int weight;
} Edge;

/* Define Heap Type Struct Containing the Weights*/
typedef struct heap {
	Edge** edges;
	int totalElements;
	int totalSize;
} Heap;


/* Functions Declaration */
void checkNullPointer(void*);
Edge* newEdge(int, int, int);
Heap* initHeap(int);
void printHeap(Heap*);
void printEdge(Edge*);
void addElement(Heap*, Edge*);
void deleteElement(Heap**);
void freeHeap(Heap*);
void swap(Edge**, Edge**);
void heapify(Heap*, int);
void krustalAlgo(Heap**, int, int);
void initUnionFind(int**, int**, int);
void freeEdge(Edge*);
int find(int, int**);
void unionAlgo(Edge*, int**, int**);
bool connected(Edge*, int**);
inline static int getParent(int);
inline static int getRightChild(int);
inline static int getLeftChild(int);


/* Checks if the pointer passed is null */
void checkNullPointer(void* pointer) {
	if(!pointer) {
		fprintf(stderr, "Current Pointer is Null, Program is Exiting With Error!\n");
		exit(1);
	}
}

/* Create a new edge and returns a pointer to it */
Edge* newEdge(int v1, int v2, int weight) {
	
	Edge* nEdge = (Edge*) malloc(sizeof(Edge));
	checkNullPointer((void*) nEdge);

	nEdge -> v1 = v1 - 1;
	nEdge -> v2 = v2 - 1;
	nEdge -> weight = weight;

	return nEdge;
}

/* Initiates a new heap struct and returns a pointer to it */
Heap* initHeap(int totalSize) {

	Heap* nHeap = (Heap*) malloc(sizeof(Heap));
	checkNullPointer((void*) nHeap);

	nHeap -> edges = (Edge**) malloc(sizeof(Edge*) * totalSize);
	checkNullPointer((void*) nHeap -> edges);

	nHeap -> totalElements = 0;
	nHeap -> totalSize = totalSize;

	return nHeap;
}

/* Returns the value for parent node */
inline static int getParent(int currIndex) {
	return (currIndex - 1) / 2;
}

/* Returns the value for parent node */
inline static int getLeftChild(int currIndex) {
	return currIndex * 2 + 1;
}

/* Returns the value for parent node */
inline static int getRightChild(int currIndex) {
	return currIndex * 2 + 2;
}

/* Swaps the pointer of two edges */
void swap(Edge** edge1, Edge** edge2) {
	Edge* temp = *edge1;
	*edge1 = *edge2;
	*edge2 = temp;
}

/* Adds a new element to heap */
void addElement(Heap* currHeap, Edge* currEdge) {

	Edge** edges = currHeap -> edges;
	int aux;
	
	if(currHeap -> totalElements == currHeap -> totalSize) {
		fprintf(stderr, "Heap Overflow, Please Update Heap Capacity And Try Again!\n");
		exit(1);
	}

	edges[currHeap -> totalElements] = currEdge;
	currHeap -> totalElements += 1;

	aux = currHeap -> totalElements - 1;
	
	while(aux > 0 && edges[getParent(aux)] -> weight > edges[aux] -> weight) {
		swap(&edges[getParent(aux)], &edges[aux]);
		aux = getParent(aux);
	}
}

/* Deletes The First Element From Heap, Takes The Last One and Replace it, than Sort The Tree to a Heap Again */
void deleteElement(Heap** currHeap) {

	Heap* aux = *currHeap;
	Edge* lastElement;
	
	if((*currHeap) -> totalElements > 0) {

		lastElement = aux -> edges[aux -> totalElements - 1];

		swap(&aux -> edges[0], &lastElement);
		aux -> totalElements--;

		freeEdge(lastElement);
		
		heapify(aux, 0);
	}
}

/* Transforms An Array Into a Min Heap */
void heapify(Heap* currHeap, int i) {

	int left = getLeftChild(i);
	int right = getRightChild(i);
	int smallest = i;
	int totalElements = currHeap -> totalElements;
	Edge** edges = currHeap -> edges;
	
	if(left < totalElements && edges[left] -> weight < edges[smallest] -> weight)
		smallest = left;

	if(right < totalElements && edges[right] -> weight < edges[smallest] -> weight)
		smallest = right;

	if(smallest != i) {
		swap(&edges[smallest], &edges[i]);
		heapify(currHeap, smallest);
	}
}

/* Prints edge info */
void printEdge(Edge* currEdge) {
	printf("Connects Vertice %d to Vertice %d\n", currEdge -> v1, currEdge -> v2);
	printf("Weight: %d\n\n", currEdge -> weight);
}

/* Print Heap Info */
void printHeap(Heap* currHeap) {

	int i;
	
	printf("Total Elements: %d\n", currHeap -> totalElements);
	printf("Total Size: %d\n", currHeap -> totalSize);

	for(i = 0; i < currHeap -> totalElements; i++)
		printEdge(currHeap -> edges[i]);
}

/* Free memory allocated for edge struct */
void freeEdge(Edge* currEdge) {
	if(currEdge){
		free(currEdge);
	}
}

/* Free memory allocated for Heap Struct */
void freeHeap(Heap* currHeap) {
	if(currHeap -> edges)
		free(currHeap -> edges);

	if(currHeap)
		free(currHeap);
}

/* Init Arrays Used in Union Find Algorithm */
void initUnionFind(int** vertices , int** ranks, int totalVertices) {

	int i;
	
	*vertices = (int*) calloc(totalVertices, sizeof(int));
	*ranks = (int*) calloc(totalVertices, sizeof(int));

	checkNullPointer((void*) *vertices);
	checkNullPointer((void*) *ranks);

	for(i = 0; i < totalVertices; i++) {
		(*vertices)[i] = i;
		(*ranks)[i] = 1;
	}
	
}

/* UnionFind Core Operation, Find the Root Vertice Given Any Other Vertice */
int find(int root, int** vertices) {

	/* Find Root Vertice and do Path Compression Recursivilly */
	if(root != (*vertices)[root])
		(*vertices)[root] = find((*vertices)[root], vertices);
	
	return (*vertices)[root];
}

/* UnionFind Core Operation, Connects Sets of Vertices Together*/
void unionAlgo(Edge* currEdge, int** vertices, int** ranks) {

	int root1 = find(currEdge -> v1, vertices);
	int root2 = find(currEdge -> v2, vertices);

	if((*ranks)[root1] < (*ranks)[root2])
		(*vertices)[root1] = root2;
	else if((*ranks)[root1] > (*ranks)[root2])
		(*vertices)[root2] = root1;
    else {
		(*vertices)[root1] = root2;
		(*ranks)[root1] += 1;		
	}
}


/* Returns if Two Vertices are Already Connected */
bool connected(Edge* currEdge, int** vertices) {
	int u = currEdge -> v1;
	int v = currEdge -> v2;
	
	return (find(u, vertices) == find(v, vertices));
}

/* Implement Krustal Algorithm and Prints The Sum of The Minimun Spanning Tree */
void krustalAlgo(Heap** currHeap, int totalVertices, int totalEdges) {

	int sum = 0, i;
	int* vertices = NULL, *ranks = NULL;
	Heap* aux = *currHeap;

	//printf("Total Vertices: %d\n", totalVertices);
	
	initUnionFind(&vertices, &ranks, totalVertices);

	for(i = 0; i < totalEdges; i++) {

		//printf("Is Connected To MST? %c\n", (connected(aux -> edges[0], &vertices)) ? 's' : 'n');
		//printEdge(aux -> edges[i]);
		
		if(connected(aux -> edges[0], &vertices)){
			deleteElement(currHeap);
			continue;
		}

		//printEdge(aux -> edges[i]);
		
		unionAlgo(aux -> edges[0], &vertices, &ranks);
		sum += aux -> edges[0] -> weight;
		
		deleteElement(currHeap);

		/* All Vertices are Connected */
		if(ranks[find(0, &vertices)] == totalVertices){
			printf("All Nodes Are Connected!\n");
			break;
		}
	}

	/*for(int i =0; i < totalVertices; i++)
	  printf("Vertice %d: %d\nRank %d: %d\n\n", i, vertices[i], i, ranks[i]);*/

	printf("%d\n", sum);

	free(vertices);
	free(ranks);
}


int main(int argc, char* argv[]) {

	int totalEdges, totalVertices;
	int u, v, w, cont = 0;
	Heap* currHeap;
	Edge* currEdge;
	
	scanf("%d %d", &totalVertices, &totalEdges);

	currHeap = initHeap(totalEdges);
	
	while(cont < totalEdges &&!feof(stdin)) {
		scanf("%d %d %d", &u, &v, &w);
		currEdge = newEdge(u, v, w);
		addElement(currHeap, currEdge);
		cont++;
		currEdge = NULL;
	}

	//printHeap(currHeap);
	
	krustalAlgo(&currHeap, totalVertices, totalEdges);

	//printHeap(currHeap);
	freeHeap(currHeap);
	
	return 0;
}
