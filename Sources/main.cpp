#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "labo.h"

#define OPTICK_ENABLE_GPU (0)
#include "optick.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

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
	if (q->prev != NULL) 
	{
		QNode* temp = q->prev;
		if(temp->prev != NULL)
			temp->prev->next = NULL;
		q->prev = temp->prev;
		q->count++;
		return temp->data;
	}
	return NULL;
}


#define HEAP_SIZE (UINT32_MAX * UINT32_MAX) * 9999999999
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

	int width, height, channels, nodesQty, imgSize, whitePixelsQty = 0, blackPixelsQty =0;
	unsigned char* img = stbi_load("31.bmp", &width, &height, &channels, 0);
	nodesQty = width * height;
	imgSize = nodesQty * channels;
	

	if (img == NULL)
	{
		printf("Error in loading the image\n");
		exit(1);
	}

	for (unsigned char* i = img; i < img + imgSize; i +=3)
	{
		if ((i[0] > 100) && (i[1] > 100) && (i[2] > 100))
		{
			//i[0] = 255; //R
			//i[1] = 0;	//G
			//i[2] = 0;	//B
			whitePixelsQty++;
		}

		//if ((i[0] < 100) && (i[1] < 100) && (i[2] < 100))
		//{
		//	i[0] = 0;	//R
		//	i[1] = 255;	//G
		//	i[2] = 0;	//B
		//	blackPixelsQty++;
		//}
	}

	AdjMatrix* graph = create_graph(whitePixelsQty);
	Vector2* pos = (Vector2*)allocate(sizeof(Vector2));
	int index = -1;
	int k = 0;

	for (unsigned char* i = img; i < img + imgSize; i += 3)
	{
		index++;
		printf("Position:%d  ", k);
		k++;
		if ((i[0] > 150) && (i[1] > 150) && (i[2] > 150))
		{
			pos->x = index % width;
			pos->y = index / width;
			add_node(graph, i, *pos);
			printf("%d\n", index);
		}
	}
	int node = 0;
	for (unsigned int i = 0; i < whitePixelsQty; i++)
	{
		for (unsigned int j = 0; j < whitePixelsQty; j++)
		{
			int x1 = graph->nodes[i].position.x;
			int y1 = graph->nodes[i].position.y;
			int x2 = graph->nodes[j].position.x;
			int y2 = graph->nodes[j].position.y;

			if (((x2 - x1 == 1) && (y1 == y2)) || ((x1 == x2) && (y2 - y1 == 1))) 
			{
				printf("Node %d", node);
				node++;
				add_edge(graph, i, j, 1);
				printf("Node%d :: x1 :%d    y1 :%d    x2 :%d    y2 :%d\n",i, x1, y1, x2, y2);
			}
		}
	}
	
	Stack newStack = stack_init(graph->len);
	//astar(graph, 0, graph->len, &newStack);
	//MakePathRed(&newStack);

	//for (int i = 0; i < graph->len; i++)
	//{
	//	graph->nodes[i].g = 0;
	//	graph->nodes[i].b = 0;
	//}

	printf("Loaded image with a width of %dpx, a height of %dpx and %d channels Nodes : %d\n", width, height, channels, nodesQty);
	
	stbi_write_bmp("new31.bmp", width, height, channels, img);
	
	return 0;
}
