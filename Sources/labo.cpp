#include "labo.h"
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
AdjMatrix* create_graph(size_t max_nodes) {
	AdjMatrix* graph = (AdjMatrix*)allocate(sizeof(AdjMatrix));
	graph->len = 0;
	graph->max_size = max_nodes;
	graph->nodes = (Node*)allocate(sizeof(Node) * max_nodes);
	for (int i = 0; i < max_nodes; ++i) {
		Node* n = &graph->nodes[i];
		n->cost = UINT64_MAX;
		n->graph_group = UINT8_MAX;
		n->visited = 0;
		n->data = "NONE";
		n->path_from = UINT64_MAX;
		n->index = NULL;
	}
	graph->adjGraph = (int**)allocate(sizeof(int*) * max_nodes);
	for (int i = 0; i < max_nodes; ++i) {
		graph->adjGraph[i] = (int*)allocate(sizeof(int) * max_nodes);
		for (int y = 0; y < max_nodes; ++y) {
			graph->adjGraph[i][y] = 0;
		}
	}
	return graph;
}

/*
* Creer un node, lui attribuer le data et l'ajouter dans la matrice d'adjacence.
*/
void add_node(AdjMatrix* graph, void* data, Vector2 pos, uint64_t idx) {
	Node* n = &graph->nodes[graph->len++];
	n->data = data;
	n->position = pos;
	n->index = idx;
}

/*
* Ajouter un lien dans la matrice d'adjacence, d'un noeud a l'autre noeud, en specifiant le cout y etant relier.
*/
void add_edge(AdjMatrix* graph, int fromNode, int toNode, uint8_t cost)
{
	graph->adjGraph[fromNode][toNode] = cost;
}

/*
* Construire les groupes de nodes
* Utiliser OPTICK_EVENT(); pour enregistrer la fonction dans le profiler
*/
void build_groups(AdjMatrix* graph)
{
	int currNodePos = 0;
	int groupNb = 0;

	while (currNodePos != graph->len)
	{
		Node* currNode = &graph->nodes[currNodePos];
		if (currNode->graph_group == UINT8_MAX)
		{
			currNode->graph_group = groupNb;
			groupNb++;
		}

		for (int j = 0; j < graph->len; j++)
		{
			if (graph->adjGraph[currNodePos][j] != 0)
			{
				graph->nodes[j].graph_group = currNode->graph_group;
			}
		}
		currNodePos++;
	}
}

/*
* Aller chercher votre implémentation d'astar. Faire des test de profiling avec votre implémentation.
* Essayer de changer le packing de la struct Node et voir si cela a un impact.
* Essayer d'utiliser la fonction avec build_groups et sans build_groups
* Utiliser OPTICK_EVENT(); pour enregistrer la fonction dans le profiler
*/

void astarAdjMatrix(AdjMatrix* graph, int startNodeIndex, int endNodeIndex, Stack* solvedPath)
{
	OPTICK_EVENT("astarAdjMatrix");
	
	//Vider la stack si elle n'est pas vide
	while (solvedPath->top != -1)
	{
		stack_pop(solvedPath);
	}

	for (int i = 0; i < graph->len; i++)
	{
		graph->nodes[i].visited = 0;
	}

	graph->nodes[startNodeIndex].cost = 0;

	Node* currNode = &(graph->nodes[startNodeIndex]);
	Queue* q = (Queue*)allocate(sizeof(Queue));
	queue_init(q);

	while (currNode != NULL)
	{
		currNode->visited = 1;

		for (int j = 0; j < graph->len; j++)
		{
			if (graph->adjGraph[currNode->index][j] != 0 && currNode == &graph->nodes[currNode->index])
			{
				if ((graph->nodes[j].visited != 1 && graph->nodes[j].cost == UINT64_MAX)  || (graph->nodes[j].cost > graph->nodes[currNode->index].cost + graph->adjGraph[currNode->index][j]))
				{
					graph->nodes[j].cost = currNode->cost + graph->adjGraph[currNode->index][j] + DistanceNodes(&graph->nodes[j], &graph->nodes[endNodeIndex - 1]);
					graph->nodes[j].path_from = graph->nodes[currNode->index].index;
					queue_push(q, &graph->nodes[j]);
				}
			}
		}
		currNode = (Node*)queue_pop(q);
	}
	currNode = &graph->nodes[endNodeIndex - 1];
	stack_push(solvedPath, currNode);
	while (currNode != &graph->nodes[startNodeIndex])
	{
		currNode = &graph->nodes[currNode->path_from];
		stack_push(solvedPath, currNode);
	}
}

double DistanceNodes(Node* fromNode, Node* toNode)
{
	double base = fabs(fromNode->position.x - toNode->position.x);
	double hauteur = fabs(fromNode->position.y - toNode->position.y);
	double hypo = sqrt((pow(base, 2) + pow(hauteur, 2)));

	return hypo;
}

void MakePathRed(Stack* s)
{
	OPTICK_EVENT("MakePathRed");
	
	while (s->top != -1)
	{
		Node* newNode = (Node*)stack_pop(s);
		unsigned char* newPixel = (unsigned char*)newNode->data;
		newPixel[0] = 255;
		newPixel[1] = 0;
		newPixel[2] = 0;
	}
}



//Liste adjacence

AdjList* create_list(size_t max_nodes)
{
	AdjList* list = (AdjList*)allocate(sizeof(AdjList));
	list->len = 0;
	list->max_size = 0;
	list->nodes = (NodeL*)allocate(sizeof(NodeL) * max_nodes);
	return list;
}

NodeL* create_node(void* data, int x, int y, int idx)
{
	NodeL* newNode = (NodeL*)allocate(sizeof(NodeL));
	newNode->data = data;
	newNode->visited = 0;
	newNode->len = 0;
	newNode->revPath = UINT64_MAX;
	newNode->cost = UINT64_MAX;
	newNode->posX = x;
	newNode->posY = y;
	newNode->index = idx;
	for (int i = 0; i < 4; i++)
	{
		newNode->adj[i] = NULL;
	}
	return newNode;
}

void add_adjacent_node(NodeL* root, NodeL* node)
{
	root->adj[root->len++] = node;
}

double DistanceNodesL(NodeL* fromNode, NodeL* toNode)
{
	OPTICK_EVENT("DistanceNodesL");
	
	double base = fabs(fromNode->posX - toNode->posX);
	double hauteur = fabs(fromNode->posY - toNode->posY);
	double hypo = sqrt((pow(base, 2) + pow(hauteur, 2)));

	return hypo;
}

void astarAdjList(std::vector<NodeL*> list, Stack* solvedPath)
{
	OPTICK_EVENT("astarAdjList");
	
	//Vider la stack si elle n'est pas vide
	while (solvedPath->top != -1)
	{
		stack_pop(solvedPath);
	}

	//S'assurer que tous les nodes.visited sont ŕ 0
	for (int i = 0; i < list.size() - 1; i++)
	{
		list.at(i)->visited = 0;
	}

	list.at(0)->cost = 0;

	NodeL* currNode = list.at(0);
	Queue* q = (Queue*)allocate(sizeof(Queue));
	queue_init(q);

	while (currNode != NULL)
	{
		currNode->visited = 1;

		int currNodeIdx = currNode->index;

		for (int j = 0; j < currNode->len; j++)
		{
			NodeL* adjacent = currNode->adj[j];
			if (adjacent->cost == UINT64_MAX || adjacent->cost > currNode->cost + 1 + DistanceNodesL(adjacent, list.at(list.size() - 1)))
			{
				adjacent->cost = currNode->cost + 1 + DistanceNodesL(adjacent, list.at(list.size() - 1));
				adjacent->revPath = currNodeIdx;

				if (adjacent->visited == 0)
				{
					adjacent->visited = 1;
					queue_push(q, adjacent);
				}
			}
		}
		currNode = (NodeL*)queue_pop(q);
	}

	currNode = list.at(list.size() - 1);
	stack_push(solvedPath, currNode);
	while (currNode != list.at(0))
	{
		currNode = list.at(currNode->revPath);
		stack_push(solvedPath, currNode);
	}
}

void MakePathRedList(Stack* s)
{
	OPTICK_EVENT("MakePathRedList");
	while (s->top != -1)
	{
		NodeL* newNode = (NodeL*)stack_pop(s);
		unsigned char* newPixel = (unsigned char*)newNode->data;
		newPixel[0] = 255;
		newPixel[1] = 0;
		newPixel[2] = 0;
	}
}

void CheckKey(NodeL* currNode, int currX, int currY, std::map<std::string, NodeL*> *map)
{
	//Initialisation de chaque clé qui vont prendre comme valeurs les positions de chaque côté du node courant pour savoir si elles existent dans la map, si elles existent, on ajoute un voisin
	std::string cle1, cle2, cle3, cle4;
	int yHaut = currY - 1;
	int yBas = currY + 1;
	int xGauche = currX - 1;
	int xDroite = currX + 1;
	
	cle1 += std::to_string(currX); //x
	cle1 += ",";
	cle1 += std::to_string(yHaut); //y
	
	cle2 += std::to_string(currX); //x
	cle2 += ",";
	cle2 += std::to_string(yBas); //y
	
	cle3 += std::to_string(xGauche); //x
	cle3 += ",";
	cle3 += std::to_string(currY); //y
	
	cle4 += std::to_string(xDroite); //x
	cle4 += ",";
	cle4 += std::to_string(currY); //y

	if (map->count(cle1) == 1)
	{
		NodeL* mapNode = map->at(cle1);
		add_adjacent_node(currNode, mapNode);
	}
	if (map->count(cle2) == 1)
	{
		NodeL* mapNode = map->at(cle2);
		add_adjacent_node(currNode, mapNode);
	}
	if (map->count(cle3) == 1)
	{
		NodeL* mapNode = map->at(cle3);
		add_adjacent_node(currNode, mapNode);
	}
	if (map->count(cle4) == 1)
	{
		NodeL* mapNode = map->at(cle4);
		add_adjacent_node(currNode, mapNode);
	}
}