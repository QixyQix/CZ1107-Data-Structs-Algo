#include <stdio.h>
#include <stdlib.h>

typedef struct _listnode
{
	int vertex;
	struct _listnode *next;
} ListNode;
typedef ListNode StackNode;

typedef struct _graph
{
	int V;
	int E;
	int **matrix;
} Graph;

typedef ListNode QueueNode;

typedef struct _queue
{
	int size;
	QueueNode *head;
	QueueNode *tail;
} Queue;

typedef struct _stack
{
	int size;
	StackNode *head;
} Stack;

void insertAdjVertex(ListNode **AdjList, int vertex);
void removeAdjVertex(ListNode **AdjList, int vertex);
int matching(Graph g);

void enqueue(Queue *qPtr, int item);
int dequeue(Queue *qPtr);
int getFront(Queue q);
int isEmptyQueue(Queue q);
void removeAllItemsFromQueue(Queue *qPtr);
void printQ(QueueNode *cur);
//////STACK///////////////////////////////////////////
void push(Stack *sPtr, int vertex);
int pop(Stack *sPtr);
int peek(Stack s);
int isEmptyStack(Stack s);
void removeAllItemsFromStack(Stack *sPtr);
//////////////////////////////////
void printMatrix(int **Matrix, int v);

int main()
{
	int Prj, Std, Mtr; //Project, Student and Mentor;
	int maxMatch;
	int i, j;
	scanf("%d %d %d", &Std, &Prj, &Mtr);

	int np, nm; //number of projects and number of mentors

	//build graph
	Graph g;
	g.V = Prj + (Std * 2) + Mtr + 2;

	g.matrix = (int **) malloc(g.V * sizeof(int*));
	for(i=0; i<g.V; i++){
        g.matrix[i] = malloc(g.V *sizeof(int));
	}

	for (i = 0; i < g.V; i++)
	{ //init matrix
		for (j = 0; j < g.V; j++)
		{
			g.matrix[i][j] = 0;
		}
	}

	int sPrj, sMtr, projID, mtrID;
	for (i = 1; i <= Std; i++)
	{
		scanf(" %d", &sPrj);
		scanf(" %d", &sMtr);
		for (j = 0; j < sPrj; j++)
		{
			scanf(" %d", &projID);
			g.matrix[projID][sPrj + i] = 1;
		}
		for (j = 0; j < sMtr; j++)
		{
			scanf(" %d", &mtrID);
			g.matrix[Prj + Std + i][mtrID + Prj + Std + Std] = 1;
		}
	}
	//Write your code

	printMatrix(g.matrix, g.V);

	//apply Ford Fulkerson algorithm
	// use DFS or BFS to find a path
	maxMatch = matching(g);
	printf("%d\n", maxMatch);
	return 0;
}

void printMatrix(int **Matrix, int v){
	int i;
	for(i=0;i<v;i++){
		int * arrHead = Matrix[0];
		int j;
		for(j=0;j<v;j++){
			printf("%d ",arrHead[j]);
		}
		printf("\n");
	}
	
}

int matching(Graph g)
{
	//Write your code
	Stack s;
	s.head = NULL;
	s.size = 0;

	int i, count = 0, parent, child;
	int temp;

	while (1)
	{
		int *visited = malloc(g.V * sizeof(int)); //to mark visited and path taken
		for (i = 0; i < g.V; i++)
		{ //init visited array with -1
			visited[i] = -1;
		}
		push(&s, 0);
		visited[0] = -5;

		while(!isEmptyStack(s)){
			int current = peek(s);
			pop(&s);

			for (i = 0; i < g.V; i++)
			{
				if (g.matrix[current][i] == 1 && visited[i] == -1)
				{
					visited[i] = current;
					push(&s, i);
				}
			}
		}

		//Check your path
		child = (g.V)-1;
		if(visited[child] == -1){
			break;
		}

		while(visited[child] != -5){
			parent = visited[child];
			g.matrix[parent][child] = 0;
			g.matrix[child][parent] = 1;
		}

		count++;
		free(visited);
	}
	return count;
}

void removeAdjVertex(ListNode **AdjList, int vertex)
{
	ListNode *temp, *preTemp;
	if (*AdjList != NULL)
	{
		if ((*AdjList)->vertex == vertex)
		{ //first node
			temp = *AdjList;
			*AdjList = (*AdjList)->next;
			free(temp);
			return;
		}
		preTemp = *AdjList;
		temp = (*AdjList)->next;
		while (temp != NULL && temp->vertex != vertex)
		{
			preTemp = temp;
			temp = temp->next;
		}
		preTemp->next = temp->next;
		free(temp);
	}
}
void insertAdjVertex(ListNode **AdjList, int vertex)
{
	ListNode *temp;
	if (*AdjList == NULL)
	{
		*AdjList = (ListNode *)malloc(sizeof(ListNode));
		(*AdjList)->vertex = vertex;
		(*AdjList)->next = NULL;
	}
	else
	{
		temp = (ListNode *)malloc(sizeof(ListNode));
		temp->vertex = vertex;
		temp->next = *AdjList;
		*AdjList = temp;
	}
}
void enqueue(Queue *qPtr, int vertex)
{
	QueueNode *newNode;
	newNode = malloc(sizeof(QueueNode));
	if (newNode == NULL)
		exit(0);

	newNode->vertex = vertex;
	newNode->next = NULL;

	if (isEmptyQueue(*qPtr))
		qPtr->head = newNode;
	else
		qPtr->tail->next = newNode;

	qPtr->tail = newNode;
	qPtr->size++;
}

int dequeue(Queue *qPtr)
{
	if (qPtr == NULL || qPtr->head == NULL)
	{ //Queue is empty or NULL pointer
		return 0;
	}
	else
	{
		QueueNode *temp = qPtr->head;
		qPtr->head = qPtr->head->next;
		if (qPtr->head == NULL) //Queue is emptied
			qPtr->tail = NULL;

		free(temp);
		qPtr->size--;
		return 1;
	}
}

int getFront(Queue q)
{
	return q.head->vertex;
}

int isEmptyQueue(Queue q)
{
	if (q.size == 0)
		return 1;
	else
		return 0;
}

void removeAllItemsFromQueue(Queue *qPtr)
{
	while (dequeue(qPtr))
		;
}

void printQ(QueueNode *cur)
{
	if (cur == NULL)
		printf("Empty");

	while (cur != NULL)
	{
		printf("%d ", cur->vertex);
		cur = cur->next;
	}
	printf("\n");
}

void push(Stack *sPtr, int vertex)
{
	StackNode *newNode;
	newNode = malloc(sizeof(StackNode));
	newNode->vertex = vertex;
	newNode->next = sPtr->head;
	sPtr->head = newNode;
	sPtr->size++;
}

int pop(Stack *sPtr)
{
	if (sPtr == NULL || sPtr->head == NULL)
	{
		return 0;
	}
	else
	{
		StackNode *temp = sPtr->head;
		sPtr->head = sPtr->head->next;
		free(temp);
		sPtr->size--;
		return 1;
	}
}

int isEmptyStack(Stack s)
{
	if (s.size == 0)
		return 1;
	else
		return 0;
}

int peek(Stack s)
{
	return s.head->vertex;
}

void removeAllItemsFromStack(Stack *sPtr)
{
	while (pop(sPtr))
		;
}
