#include <stdint.h>
#include <assert.h>

void* allocate(size_t size);

typedef struct {
	void** elements;
	int top;
	size_t max;
} Stack;

Stack stack_init(size_t max_size);
void stack_push(Stack* s, void* element);
void* stack_pop(Stack* s);

typedef struct QNode QNode;
typedef struct QNode {
	void* data;
	QNode* prev;
	QNode* next;
	size_t count;
}Queue;

void queue_init(Queue* q);
void queue_push(Queue* q, void* data);
void* queue_pop(Queue* q);

/*
* Programmer l'algorithme de fibonacci.
* Faire attention lorsque vous utilisez un type pour une variable qui représente un nombre, int ne dépasse pas 2^31-1 ou 2,147,483,647
*/
long long fibonacci(int n);

/*
* Programmer l'algorithme de fibonacci avec la memoization en utilisant une variable static fib_cache, implémenter dans la fonction. Son type sera long long [].
* Utiliser OPTICK_EVENT(); pour enregistrer la fonction dans le profiler
* Faire attention lorsque vous utilisez un type pour une variable qui représente un nombre, int ne dépasse pas 2^31-1 ou 2,147,483,647
*/
long long fibonacci_memoization(int n);

/*
* Programmer l'algorithme de fibonacci avec la memoization en utilisant une variable static fib_cache, implémenter dans la fonction. Son type sera long long**.
* Allouer de la memoire pour fib_cache en utilisant allocate. Après avoir trouver un resultat, pour le mettre dans fib_cache, allouer un int* avec malloc puis ajouter le a fib_cache.
* Utiliser OPTICK_EVENT(); pour enregistrer la fonction dans le profiler
* Faire attention lorsque vous utilisez un type pour une variable qui représente un nombre, int ne dépasse pas 2^31-1 ou 2,147,483,647.
*/
long long fibonacci_memoization_malloc(int n);



typedef struct {
	double x;
	double y;
} Vector2;

typedef struct {
	uint8_t cost;
	Vector2 position;
	uint8_t graph_group;
	void* data;
	uint8_t path_from;
	uint8_t visited;
} Node;
typedef struct AdjMatrix AdjMatrix;

struct AdjMatrix {
	int** adjGraph;// Essentiellement: int[][]
	Node* nodes;
	size_t len;
	size_t max_size;
};


AdjMatrix* create_graph(size_t max_nodes);

/*
* Creer un node, lui attribuer le data et l'ajouter dans la matrice d'adjacence.
*/
void add_node(AdjMatrix* graph, void* data, Vector2 pos);

/*
* Ajouter un lien dans la matrice d'adjacence, d'un noeud a l'autre noeud, en specifiant le cout y etant relier.
*/
void add_edge(AdjMatrix* graph, int fromNode, int toNode, uint8_t cost);


/*
* Construire les groupes de nodes
* Utiliser OPTICK_EVENT(); pour enregistrer la fonction dans le profiler
*/

//void build_groups(AdjMatrix* graph);

/*
* Aller chercher votre implémentation d'astar. Faire des test de profiling avec votre implémentation. 
* Essayer de changer le packing de la struct Node et voir si cela a un impact.
* Essayer d'utiliser la fonction avec build_groups et sans build_groups
* Utiliser OPTICK_EVENT(); pour enregistrer la fonction dans le profiler
*/

//void astar(AdjMatrix* graph,int startNodeIndex, int endNodeIndex, Stack* solvedPath);