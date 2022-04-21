#include "labo.h"
#include <math.h>
#include <stdlib.h>

/*
* Programmer l'algorithme de fibonacci.
* Faire attention lorsque vous utilisez un type pour une variable qui représente un nombre, int ne dépasse pas 2^31-1 ou 2,147,483,647
*/
long long fibonacci(int n)
{
	if (n == 0)
	{
		return 0;
	}
	else if (n == 1)
	{
		return 1;
	}

	return fibonacci(n - 1) + fibonacci(n - 2);
}

/*
* Programmer l'algorithme de fibonacci avec la memoization en utilisant une variable static fib_cache, implémenter dans la fonction. Son type sera long long [].
* Utiliser OPTICK_EVENT(); pour enregistrer la fonction dans le profiler
* Faire attention lorsque vous utilisez un type pour une variable qui représente un nombre, int ne dépasse pas 2^31-1 ou 2,147,483,647
*/
long long fibonacci_memoization(int n)
{
	static long long fib_cache[75] = { 0 };

	if (n == 0)
	{
		return 0;
	}
	else if (n == 1)
	{
		return 1;
	}
	else if (fib_cache[n] != 0)
	{
		return fib_cache[n];
	}

	return fib_cache[n] = fibonacci_memoization(n - 1) + fibonacci_memoization(n - 2);
}

/*
* Programmer l'algorithme de fibonacci avec la memoization en utilisant une variable static fib_cache, implémenter dans la fonction. Son type sera long long**.
* Allouer de la memoire pour fib_cache en utilisant allocate. Après avoir trouver un resultat, pour le mettre dans fib_cache, allouer un int* avec malloc puis ajouter le a fib_cache.
* Utiliser OPTICK_EVENT(); pour enregistrer la fonction dans le profiler
* Faire attention lorsque vous utilisez un type pour une variable qui représente un nombre, int ne dépasse pas 2^31-1 ou 2,147,483,647.
*/
 long long fibonacci_memoization_malloc(int n)
{
	static long long** fib_cache = NULL;

	if (fib_cache == NULL)
	{
		fib_cache = (long long**)allocate(sizeof(long long*) * 75);

		for (int i = 0; i < 75; i++)
		{
			fib_cache[i] = NULL;
		}
	}

	if (n == 0)
	{
		return 0;
	}
	else if (n == 1)
	{
		return 1;
	}
	else
	{
		if (fib_cache[n] == NULL)
		{
			fib_cache[n] = (long long*)malloc(sizeof(long long*));
			*fib_cache[n] = fibonacci_memoization_malloc(n - 1) + fibonacci_memoization_malloc(n - 2);
		}
		return *fib_cache[n];
	}
}


AdjMatrix* create_graph(size_t max_nodes) {
	AdjMatrix* graph = (AdjMatrix*)allocate(sizeof(AdjMatrix));
	graph->len = 0;
	graph->max_size = max_nodes;
	graph->nodes = (Node*)allocate(sizeof(Node) * max_nodes);
	for (int i = 0; i < max_nodes; ++i) {
		Node* n = &graph->nodes[i];
		n->cost = UINT8_MAX;
		n->graph_group = UINT8_MAX;
		n->visited = 0;
		n->data = "NONE";
		n->path_from = UINT8_MAX;
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

void add_node(AdjMatrix* graph, void* data, Vector2 pos) {
	Node* n = &graph->nodes[graph->len++];
	n->data = data;
	n->position = pos;
}

void add_edge(AdjMatrix* graph, int fromNode, int toNode, uint8_t cost)
{
	graph->adjGraph[fromNode][toNode] = cost;
}