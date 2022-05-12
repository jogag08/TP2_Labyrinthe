#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <vector>
#include <map>
#include <string>

#include "labo.h"

#define OPTICK_ENABLE_GPU (0)
#include "optick.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

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


#define HEAP_SIZE (UINT64_MAX * UINT64_MAX) * 9999999999
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
	
	//Trouver le nombre de pixels blanc
	for (unsigned char* i = img; i < img + imgSize; i +=3)
	{
		if ((i[0] > 100) && (i[1] > 100) && (i[2] > 100))
		{
			whitePixelsQty++;
		}
	}
	///////////////////////////////////////////////////////////////////////
	//METTRE LA VALEUR DE adj A 1 POUR MATRICE ET 2 POUR LISTE
	int adj = 1;
	///////////////////////////////////////////////////////////////////////

	//Trouver le chemin avec la matrice d'adjacence
	if (adj == 1)
	{
		AdjMatrix* graph = create_graph(whitePixelsQty);
		Vector2* pos = (Vector2*)allocate(sizeof(Vector2));
		uint64_t index = 0;
		int k = 0;

		//Boucle servant à attribuer des positions aux nodes et les ajouter au graph
		for (unsigned char* i = img; i < img + imgSize; i += 3)
		{
			if ((i[0] > 150) && (i[1] > 150) && (i[2] > 150))
			{
				pos->x = k % width;
				pos->y = k / width;
				add_node(graph, i, *pos, index);
				index++;
			}
			k++;
		}
		
		//Boucle pour ajouter les edges aux nodes
		for (unsigned int i = 0; i <= graph->len; i++)
		{
			for (unsigned int j = 0; j <= graph->len; j++)
			{
				int x1 = graph->nodes[i].position.x;
				int y1 = graph->nodes[i].position.y;
				int x2 = graph->nodes[j].position.x;
				int y2 = graph->nodes[j].position.y;

				if
					(((x2 - x1 == 1) && (y1 == y2)) ||	//voisin gauche
					((x1 == x2) && (y2 - y1 == 1)) ||	//voisin bas
					((x2 - x1 == -1) && (y1 == y2)) ||	//voisin droite
					((x1 == x2) && (y2 - y1 == -1)))	//voisin haut
				{
					add_edge(graph, i, j, 1);
				}
			}
		}

		Stack newStack = stack_init(whitePixelsQty);
		astarAdjMatrix(graph, 0, newStack.max, &newStack);
		MakePathRed(&newStack);

		printf("Loaded image with a width of %dpx, a height of %dpx and %d channels Nodes : %d\n", width, height, channels, nodesQty);

		stbi_write_bmp("new31Matrix.bmp", width, height, channels, img);
	}
	
	////////////////////////////////////////////////////////////////////////////
	else if (adj == 2)
	{
		std::vector<NodeL*> pixelList;
		std::map<std::string, NodeL*> map;
		Vector2* pos = (Vector2*)allocate(sizeof(Vector2));
		int k = 0;
		int listIdx = -1;

		//Construction d'un vecteur composé des nodes et attribuer à ces nodes une clé pour ensuite les mettres dans une map pour pouvoir les retrouver plus tard
		for (unsigned char* i = img; i < img + imgSize; i += 3)
		{
			if ((i[0] > 150) && (i[1] > 150) && (i[2] > 150))
			{
				listIdx++;
				pos->x = k % width;
				pos->y = k / width;
				NodeL* newNode = create_node(i, pos->x, pos->y, k);
				pixelList.emplace_back(newNode);
				std::string cle;
				cle += std::to_string(pos->x);
				cle += ",";
				cle += std::to_string(pos->y);
				map.emplace(cle, newNode);
				newNode->index = listIdx;
			}
			k++;
		}

		//Trouver les voisins de chaques nodes
		for (unsigned int i = 0; i < pixelList.size(); i++)
		{
			int x = pixelList.at(i)->posX;
			int y = pixelList.at(i)->posY;

			CheckKey(pixelList.at(i), x, y, &map);
		}

		Stack newStack = stack_init(whitePixelsQty);
		astarAdjList(pixelList, &newStack);
		MakePathRedList(&newStack);

		printf("Loaded image with a width of %dpx, a height of %dpx and %d channels Nodes : %d\n", width, height, channels, nodesQty);

		stbi_write_bmp("newperfect2kList.bmp", width, height, channels, img);
	}
	return 0;
}
