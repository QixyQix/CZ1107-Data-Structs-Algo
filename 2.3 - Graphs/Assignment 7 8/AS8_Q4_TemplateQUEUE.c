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
    ListNode **list;
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
int reverseAndCheckPath(int *path, Graph g);
void printGraphList(Graph g);

int main()
{
    int Prj, Std, Mtr; //Number of Projects, Students and Mentors
    int maxMatch;
    scanf("%d %d %d", &Std, &Prj, &Mtr);

    //Build graph
    Graph g;
    //Write your code
    //Number of nodes ==> stud*2 + noOfProj + noOfMentor + source&drain(2)
    g.E = 0;
    g.V = (Std * 2) + Prj + Mtr + 2;
    g.list = (ListNode **)malloc(g.V * sizeof(ListNode *));
    int i;
    for (i = 0; i < g.V; i++)
    {
        g.list[i] = NULL;
    }

    //Graph Legend
    //0 - Source
    //1,2 - Student 1 nodes a and b
    //2,3 - Student 2 nodes a and b
    //....
    //(2*noOfStudents)+1 - Proj 1
    //...
    //(2*noOfStudents)+noOfProjects+1 - Mentor 1
    //...
    // V - Drain
    int studentNo;
    for (studentNo = 1; studentNo <= Std; studentNo++)
    {
        int noPrefProj;
        int noPrefMent;
        scanf("%d", &noPrefProj);
        scanf("%d", &noPrefMent);

        //Process projects first
        //Project index start from offet (2*no of students)
        int j;
        for (j = 0; j < noPrefProj; j++)
        {
            int projNo;
            scanf("%d", &projNo);
            //Adjust proj No
            projNo += (2 * Std);
            //Link to studentX(a)
            insertAdjVertex(&g.list[projNo], (studentNo * 2) - 1);
        }

        //Link studentX(a) to studentX(b)
        insertAdjVertex(&g.list[(studentNo * 2) - 1], (studentNo * 2));

        //Process mentor next
        for (j = 0; j < noPrefMent; j++)
        {
            int mentNo;
            scanf("%d", &mentNo);
            //Adjust mentor no, after studentNodes and the project
            mentNo += (2 * Std) + Prj;
            //Link to studentX(b)
            insertAdjVertex(&g.list[(studentNo * 2)], mentNo);
        }
    }

    //Connect source to projects
    for (i = 1; i <= Prj; i++)
    {
        int projVertex = (Std * 2) + i;
        insertAdjVertex(&g.list[0], projVertex);
    }
    //Connect mentors to the drain
    for (i = 1; i <= Mtr; i++)
    {
        int mtrVertex = (Std * 2) + Prj + i;
        insertAdjVertex(&g.list[mtrVertex], (g.V) - 1);
    }

    //apply Ford Fulkerson algorithm
    //use DFS or BFS to find a path
    maxMatch = matching(g);
    printf("%d\n", maxMatch);
    return 0;
}

int matching(Graph g)
{
    int matches = 0;

    //Initalize stack for DFS
    Queue q;
    q.head = NULL;
    q.tail = 0;
    q.size = 0;

    //Continuously perform BFS
    while (1)
    {
        removeAllItemsFromQueue(&q);
        int *path = malloc(g.V * sizeof(int));
        int i;
        for(i=0;i<g.V;i++){
            path[i]=-1;
        }

        enqueue(&q,0);
        path[0]=-2;

        while(!isEmptyQueue(q)){
            int visiting = getFront(q);
            dequeue(&q);

            ListNode *currNode = g.list[visiting];
            while(currNode!=NULL){
                int nodeToVisit = currNode->vertex;
                if(nodeToVisit == g.V-1){
                    matches++;
                    path[nodeToVisit] = visiting;
                    removeAllItemsFromQueue(&q);
                }
                if(path[nodeToVisit] == -1){
                    path[nodeToVisit] = visiting;
                    enqueue(&q,nodeToVisit);
                }
                currNode = currNode->next;
            }
        }
        if(!reverseAndCheckPath(path, g)){
            return matches;
        }
        free(path);
    }
    //Write your code
    return matches;
}

int reverseAndCheckPath(int *path, Graph g)
{
    //Returns 1 if there is a path, the items are reversed
    //Returns 0 if there is no path and the items are NOT reversed

    //Check for path. There will be no path if the destination parent is the default
    //Our drain is always in the last vertex
    if (path[(g.V) - 1] == -1)
    {
        return 0;
    }
    //There is a path, reverse the vertices
    int child = (g.V) - 1;

    while (child != 0)
    {
        int parent = path[child];

        //Remove child from parent's adjacency list
        removeAdjVertex(&g.list[parent], child);
        //Insert prent into child's adjacency list
        insertAdjVertex(&g.list[child], parent);

        child = parent;
    }
    return 1;
}

void printGraphList(Graph g)
{
    int i;
    ListNode *temp;

    for (i = 0; i < g.V; i++)
    {
        printf("%d:\t", i);
        temp = g.list[i];
        while (temp != NULL)
        {
            printf("%d -> ", temp->vertex);
            temp = temp->next;
        }
        printf("\n");
    }
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