#include <stdint.h>
#include <assert.h>
#include <vector>

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

typedef struct {
	int x;
	int y;
} Vector2;

typedef struct {
	uint64_t cost;
	Vector2 position;
	uint8_t graph_group;
	void* data;
	unsigned char r;
	unsigned char g;
	unsigned char b;
	uint64_t path_from;
	uint64_t index;
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
void add_node(AdjMatrix* graph, void* data, Vector2 pos, uint64_t idx);

/*
* Ajouter un lien dans la matrice d'adjacence, d'un noeud a l'autre noeud, en specifiant le cout y etant relier.
*/
void add_edge(AdjMatrix* graph, int fromNode, int toNode, uint8_t cost);


/*
* Construire les groupes de nodes
* Utiliser OPTICK_EVENT(); pour enregistrer la fonction dans le profiler
*/

void build_groups(AdjMatrix* graph);

/*
* Aller chercher votre implémentation d'astar. Faire des test de profiling avec votre implémentation. 
* Essayer de changer le packing de la struct Node et voir si cela a un impact.
* Essayer d'utiliser la fonction avec build_groups et sans build_groups
* Utiliser OPTICK_EVENT(); pour enregistrer la fonction dans le profiler
*/

void astarAdjMatrix(AdjMatrix* graph,int startNodeIndex, int endNodeIndex, Stack* solvedPath);

double DistanceNodes(Node* fromNode, Node* toNode);
void MakePathRed(Stack* s);



//Avec liste adjacence

struct NodeL{
	void* data;
	int index;
	int r;
	int g;
	int b;
	//Vector2 pos;
	int posX;
	int posY;
	NodeL* adj[4];
	uint8_t adjCount;
	uint8_t len;
	uint64_t cost;
	uint8_t visited;
	uint64_t revPath;

//	//uint64_t cost;
//	//Vector2 position;
//	//uint8_t graph_group;
//	//unsigned char r;
//	//unsigned char g;
//	//unsigned char b;
//	//uint64_t path_from;
//	//uint64_t index;
//	//uint8_t visited;
};

typedef struct AdjList AdjList;
struct AdjList {
	NodeL* nodes;
	size_t len;
	size_t max_size;
};

AdjList* create_list(size_t max_nodes);

NodeL* create_node(void* data, int x, int y, int idx);

void add_adjacent_node(NodeL* root, NodeL* node);

void astarAdjList(std::vector<NodeL*> list, Stack* solvedPath);

double DistanceNodesL(NodeL* fromNode, NodeL* toNode);