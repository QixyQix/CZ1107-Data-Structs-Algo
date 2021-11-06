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
    int **adjMatrix;
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

void removeAdjVertex(int **matrix, int from, int to);
void insertAdjVertex(int **matrix, int from, int to);
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
    g.adjMatrix = (int **)malloc(g.V * sizeof(int *));
    int i;
    for (i = 0; i < g.V; i++)
    {
        g.adjMatrix[i] = malloc(g.V * sizeof(int));
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

        int aStudentIndex = (studentNo * 2) - 1;
        int bStudentIndex = (studentNo * 2);

        //Process projects first
        //Project index start from offet (2*no of students)
        int j;
        for (j = 0; j < noPrefProj; j++)
        {
            int projNo;
            scanf("%d", &projNo);
            //Adjust proj No
            projNo += (2 * Std);
            //Link proj -> studentA
            insertAdjVertex(g.adjMatrix, projNo, aStudentIndex);
        }

        //Link studentX(a) -> studentX(b)
        insertAdjVertex(g.adjMatrix, aStudentIndex, bStudentIndex);

        //Process mentor next
        for (j = 0; j < noPrefMent; j++)
        {
            int mentNo;
            scanf("%d", &mentNo);
            //Adjust mentor no, after studentNodes and the project
            mentNo += (2 * Std) + Prj;
            //Link studentX(b) -> mentor
            insertAdjVertex(g.adjMatrix,bStudentIndex, mentNo);
        }
    }

    //Connect source to projects
    for (i = 1; i <= Prj; i++)
    {
        int projVertex = (Std * 2) + i;
        insertAdjVertex(g.adjMatrix, 0, projVertex);
    }
    //Connect mentors to the drain
    for (i = 1; i <= Mtr; i++)
    {
        int mtrVertex = (Std * 2) + Prj + i;
        insertAdjVertex(g.adjMatrix, mtrVertex, g.V - 1);
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

    //Initalize queue for BFS
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
        for (i = 0; i < g.V; i++)
        {
            path[i] = -1;
        }

        enqueue(&q, 0);
        path[0] = -2;

        while (!isEmptyQueue(q))
        {
            int visiting = getFront(q);
            dequeue(&q);

            for (i = 0; i < g.V; i++)
            {
                if (g.adjMatrix[visiting][i] == 1 && path[i] == -1)
                {
                    path[i] = visiting;
                    enqueue(&q, i);
                }
            }
        }
        //Check for path
        int child = g.V - 1;
        if (path[child] == -1)
        {
            break;
        }
        else
        {
            while (path[child] != -2)
            {
                int parent = path[child];
                removeAdjVertex(g.adjMatrix, parent, child);
                insertAdjVertex(g.adjMatrix, child, parent);
                child = parent;
            }
            matches++;
            free(path);
        }
    }
    //Write your code
    return matches;
}

void removeAdjVertex(int **matrix, int from, int to)
{
    matrix[from][to] = 0;
}
void insertAdjVertex(int **matrix, int from, int to)
{
    matrix[from][to] = 1;
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