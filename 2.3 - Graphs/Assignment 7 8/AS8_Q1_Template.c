#include <stdio.h>
#include <stdlib.h>

typedef struct _listnode
{
    int vertex;
    struct _listnode *next;
} ListNode;

typedef struct _graph
{
    int V;
    int E;
    ListNode **list;
} Graph;

typedef ListNode QueueNode;

typedef struct _queue
{
    int size;
    QueueNode *head;
    QueueNode *tail;
} Queue;

int BFS(Graph G, int v, int w);

void printGraphList(Graph);

// You should not change the prototypes of these functions
void enqueue(Queue *qPtr, int item);
int dequeue(Queue *qPtr);
int getFront(Queue q);
int isEmptyQueue(Queue q);
void removeAllItemsFromQueue(Queue *qPtr);

int main()
{
    Graph g;
    int i, j;
    ListNode *temp;

    //Get the numberof vertices in the graph
    printf("Enter the number of vertices:\n");
    scanf("%d", &g.V);

    //Instantiate the graph, such that all vertices are NULL
    g.list = (ListNode **)malloc(g.V * sizeof(ListNode *));
    for (i = 0; i < g.V; i++)
        g.list[i] = NULL;

    printf("Enter two vertices which are adjacent to each other:\n");
    while (scanf("%d %d", &i, &j) == 2)
    {
        //Adjust the index such that it can be entered into the graph
        i = i - 1;

        //Initialie a new LL if the slot is NULL (i.e. vertice not defined yet)
        if (g.list[i] == NULL)
        {
            g.list[i] = (ListNode *)malloc(sizeof(ListNode));
            g.list[i]->vertex = j;
            g.list[i]->next = NULL;
        }
        else
        {
            //Insert the new link at the front of the linkedlist
            temp = (ListNode *)malloc(sizeof(ListNode));
            temp->next = g.list[i];
            temp->vertex = j;
            g.list[i] = temp;
        }
        g.E++;
        printf("Enter two vertices which are adjacent to each other:\n");
    }
    scanf("%*c");
    printf("Enter two vertices for finding their shortest distance:\n");
    scanf("%d %d", &i, &j);

    int d = BFS(g, i, j);

    if (d == -1)
        printf("%d and %d are unconnected.\n", i, j);
    else
        printf("The shortest distance is %d\n", d);
    printGraphList(g);
    return 0;
}

int BFS(Graph g, int v, int w)
{
    // Write your code here
    int distance = 0;

    //Initialize the queue
    Queue q;
    q.head = NULL;
    q.size = 0;
    q.tail = NULL;

    //Initialize array to store parent; the parent is previous node visited before reaching the current node.
    int *pathParent = malloc((g.V)*sizeof(int));
    int i;
    for(i=0;i<g.V;i++){
        pathParent[i] = -1;
    }

    //Start from v
    enqueue(&q,v);
    pathParent[v-1] = -2;

    while(!isEmptyQueue(q)){
        int current = q.head->vertex;
        dequeue(&q);
        //Enqueue all of current's links
        ListNode *nodeToQueue = g.list[current-1];
        while(nodeToQueue!=NULL){
            int vertex = nodeToQueue->vertex;

            //Enqueue -1 denotes it is not visited, and set the parent path to be the current node
            if (pathParent[vertex-1] == -1){
                pathParent[vertex-1] = current;
                enqueue(&q,vertex);
            }
            nodeToQueue = nodeToQueue->next;
        }
    }

    //Calculate the path from the destination back to the current
    if(pathParent[w-1] == -1){
        return -1;
    }else{
        int distance = 1;
        int index = w-1;
        while(pathParent[index] != v){
            index = pathParent[index] -1;
            distance++;
        }
        return distance;
    }
}

void printGraphList(Graph g)
{
    int i;
    ListNode *temp;

    for (i = 0; i < g.V; i++)
    {
        printf("%d:\t", i + 1);
        temp = g.list[i];
        while (temp != NULL)
        {
            printf("%d -> ", temp->vertex);
            temp = temp->next;
        }
        printf("\n");
    }
}

void enqueue(Queue *qPtr, int item)
{
    QueueNode *newNode;
    newNode = malloc(sizeof(QueueNode));
    if (newNode == NULL)
        exit(0);

    newNode->vertex = item;
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
