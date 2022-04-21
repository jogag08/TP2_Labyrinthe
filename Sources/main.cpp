#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "labo.h"

#define OPTICK_ENABLE_GPU (0)
#include "optick.h"


Stack stack_init(size_t max_size) {
	Stack s;
	s.max = max_size;
	s.top = -1;
	s.elements = (void**)allocate(sizeof(void*) * s.max);
	return s;
}

void stack_push(Stack* s, void* element) {
	if (s->max != s->top + 1) {
		s->top++;
		s->elements[s->top] = element;
	}
}

void* stack_pop(Stack* s) {
	if (s->top > -1) {
		void* elem = s->elements[s->top];
		s->elements[s->top] = NULL;
		--s->top;
		return elem;
	}
	return NULL;
}
void queue_init(Queue* q) {
	q->next = q->prev = NULL;
	q->count = 0;
}
void queue_push(Queue* q, void* data) {
	QNode* n = (QNode*)allocate(sizeof(Node));
	n->data = data;
	n->prev = n->next = NULL;
	if (q->prev == NULL) {
		q->prev = q->next = n;
	}
	else {
		QNode* temp = q->next;
		q->next = temp->prev = n;
		n->next = temp;
	}
}
void* queue_pop(Queue* q) {
	if (q->prev != NULL) {
		QNode* temp = q->prev;
		if(temp->prev != NULL)
			temp->prev->next = NULL;
		q->prev = temp->prev;
		q->count++;
		return temp->data;
	}
}

#define HEAP_SIZE 1024 * 2048
static uint8_t* heap = NULL;
static size_t heap_top = 0;
void* allocate(size_t size) {
	size_t old_top = heap_top;
	heap_top += size;
	assert(heap_top <= HEAP_SIZE);
	return &heap[old_top];
}

int main(int argc, char** argv) {
	heap = (uint8_t*)malloc(HEAP_SIZE);
	assert(heap != NULL);
	OPTICK_APP("ConsoleApp");
	int count = 30;
	while (count > 0) {
		int n = 25;
		for (int i = 0; i <= n; ++i)
			printf("%lld\n", fibonacci(i));
		count--;
	}
	count = 30;
	while (count > 0) {
		int n = 25;
		for (int i = 0; i <= n; ++i)
			printf("%lld\n", fibonacci_memoization(i));
		count--;
	}
	count = 30;
	while (count > 0) {
		int n = 25;
		for (int i = 0; i <= n; ++i)
			printf("%lld\n", fibonacci_memoization_malloc(i));
		count--;
	}

	//printf("-----FIRST-----\n");
	//srand(time(NULL));
	//AdjMatrix* graph = create_graph(250);
	//count = 0;
	//for (int i = 65; i < 90; ++i) {
	//	char* temp = (char*)allocate(sizeof(char)*10);
	//	temp[0] = i;
	//	int y = 65;
	//	for (; y < 65 + count; ++y) {
	//		temp[y - 65] = (char)y;
	//	}
	//	temp[y] = '\0';
	//	Vector2 pos = { 0 };
	//	pos.x = rand() % 100;
	//	pos.y = rand() % 100;
	//	add_node(graph, temp, pos);
	//	int amnt = rand() % 5;
	//	for (int ti = 0; ti < amnt; ++ti) {
	//		int to = rand() % 250;
	//		int from = (i-65) + (count * 25);
	//		add_edge(graph, from, to, rand() % 255);
	//	}
	//
	//	if (i == 89 && count < 10) {
	//		count++;
	//		i = 65;
	//	}
	//	if (count >= 10) {
	//		break;
	//	}
	//}
	//
	//Stack s = stack_init(250);
	//Queue tq;
	//queue_init(&tq);
	//
	//build_groups(graph);
	//// Find first adjacent
	//int i = 0;
	//int y = 0;
	//for (; y < 250; ++y) {
	//	for (; i < 250; ++i) {
	//		if (graph->nodes[y].graph_group == graph->nodes[i].graph_group && &graph->nodes[y] != &graph->nodes[i])
	//			break;
	//	}
	//	if (i != 250)
	//		break;
	//}
	//
	//astar(graph, y, i, &s);
	//Node* n = (Node*)stack_pop(&s);
	//queue_push(&tq, n);
	//while (n != NULL) {
	//	printf("      %s", (char*)n->data);
	//	n = (Node*)stack_pop(&s);
	//	if (n != NULL) {
	//		queue_push(&tq, n);
	//		printf(" ->");
	//	}
	//}
	//printf("\n");
	//n = (Node*)queue_pop(&tq);
	//while (n != NULL) {
	//	printf("cost: %i | ", n->cost);
	//	n->path_from = UINT8_MAX;
	//	n->cost = UINT8_MAX;
	//	n->visited = 0;
	//	n = (Node*)queue_pop(&tq);
	//}
	//printf("\n");
	return 0;
}
