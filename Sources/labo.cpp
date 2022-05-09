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

void astar(AdjMatrix* graph, int startNodeIndex, int endNodeIndex, Stack* solvedPath)
{
	//if (graph->nodes[startNodeIndex].graph_group != graph->nodes[endNodeIndex].graph_group)
	//{
	//	return;
	//}

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

	for (int i = 0; i < graph->len; i++)
	{
		printf("%d", graph->nodes[i].index);
	}

	while (currNode != NULL)
	{
		currNode->visited = 1;

		for (int i = 0; i < graph->len; i++)
		{
			for (int j = 0; j < graph->len; j++)
			{
				if (graph->adjGraph[i][j] != 0 && currNode == &graph->nodes[i])
				{
					if ((graph->nodes[j].visited != 1 && graph->nodes[j].cost == UINT64_MAX)  || (graph->nodes[j].cost > graph->nodes[i].cost + graph->adjGraph[i][j]))
					{
						graph->nodes[j].cost = currNode->cost + graph->adjGraph[i][j] + DistanceNodes(&graph->nodes[j], &graph->nodes[endNodeIndex - 1]);
						graph->nodes[j].path_from = graph->nodes[i].index;
						queue_push(q, &graph->nodes[j]);
					}
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
	while (s->top != -1)
	{
		Node* newNode = (Node*)stack_pop(s);
		unsigned char* newPixel = (unsigned char*)newNode->data;
		newPixel[0] = 255;
		newPixel[1] = 0;
		newPixel[2] = 0;
	}
}