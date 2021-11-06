#include <stdio.h>
#include <stdlib.h>

enum GraphType
{
    ADJ_MATRIX,
    ADJ_LIST
}; // Types of Graph Representation

typedef struct _listnode
{
    int vertex;
    struct _listnode *next;
} ListNode;

union GraphForm
{
    int **matrix;
    ListNode **list;
};

typedef struct _graph
{
    int V;
    int E;
    enum GraphType type;
    union GraphForm adj;
} Graph;

void printGraphMatrix(Graph);
void adjM2adjL(Graph *);
void adjL2adjM(Graph *);
void printGraphList(Graph);
void calDegreeV(Graph, int *);
void printDegreeV(int *, int);

int main()
{
    Graph g;
    int i, j;

    int *degreeV;

    printf("Enter the number of vertices:\n");
    scanf("%d", &g.V);

    g.E = 0;
    g.adj.matrix = (int **)malloc(g.V * sizeof(int *));
    for (i = 0; i < g.V; i++)
        g.adj.matrix[i] = (int *)malloc(g.V * sizeof(int));

    for (i = 0; i < g.V; i++)
        for (j = 0; j < g.V; j++)
            g.adj.matrix[i][j] = 0;
    g.type = ADJ_MATRIX;

    degreeV = (int *)malloc(g.V * sizeof(int));
    for (i = 0; i < g.V; i++)
        degreeV[i] = 0;

    int V1, V2;
    printf("Enter two vertices which are adjacent to each other:\n");
    while (scanf("%d %d", &V1, &V2) == 2)
    {
        if (V1 > 0 && V1 <= g.V && V2 > 0 && V2 <= g.V)
        {
            g.adj.matrix[V1 - 1][V2 - 1] = 1;
            g.adj.matrix[V2 - 1][V1 - 1] = 1;
            g.E++;
        }
        else
            break;
        printf("Enter two vertices which are adjacent to each other:\n");
    }

    calDegreeV(g, degreeV);

    printGraphMatrix(g);
    printDegreeV(degreeV, g.V);

    adjM2adjL(&g);

    calDegreeV(g, degreeV);

    printGraphList(g);
    printDegreeV(degreeV, g.V);

    adjL2adjM(&g);
    printGraphMatrix(g);

    return 0;
}

void printGraphMatrix(Graph g)
{
    int i, j;
    if (g.type == ADJ_LIST)
    {
        printf("Error");
        return;
    }

    for (i = 0; i < g.V; i++)
    {
        for (j = 0; j < g.V; j++)
            printf("%d\t", g.adj.matrix[i][j]);
        printf("\n");
    }
}

void printDegreeV(int *degreeV, int V)
{
    int i;
    for (i = 0; i < V; i++)
        printf("%d: %d degree\n", i + 1, degreeV[i]);
}

void printGraphList(Graph g)
{
    int i;
    ListNode *temp;

    if (g.type == ADJ_MATRIX)
    {
        printf("Error");
        return;
    }

    for (i = 0; i < g.V; i++)
    {
        printf("%d:\t", i + 1);
        temp = g.adj.list[i];
        while (temp != NULL)
        {
            printf("%d -> ", temp->vertex);
            temp = temp->next;
        }
        printf("\n");
    }
}

void adjM2adjL(Graph *g)
{
    // Question 1
    // Write your code here
    if (g->type == ADJ_LIST)
    {
        printf("Error\n");
        return;
    }

    if (g->V == 0)
    {
        printf("Empty graph\n");
        return;
    }

    //Initialize the list
    ListNode **list = (ListNode **)malloc(g->V * sizeof(ListNode *));
    int i, j;
    for (i = 0; i < g->V; i++)
    {
        list[i] = NULL;
    }

    //Convert to ADJ List -- use temp to keep track of the end of the list
    ListNode *temp = NULL;
    //Traverse the rows (from)
    for (i = 0; i < g->V; i++)
    {
        //Traverse the cols (to)
        for (j = 0; j < g->V; j++)
        {
            if (g->adj.matrix[i][j] == 1)
            {
                //We insert the vertices to the end of the LL
                if (!list[i])
                {
                    temp = malloc(sizeof(ListNode));
                    temp->next = NULL;
                    temp->vertex = j + 1;
                    list[i] = temp;
                }else{
                    temp->next = malloc(sizeof(ListNode));
                    temp->next->vertex = j+1;
                    temp->next->next = NULL;
                    temp = temp->next;
                }
            }
        }
    }

    //Cleanup
    //Free the matrix
    for(i=0;i<g->V;i++){
        free(g->adj.matrix[i]);
    }
    free(g->adj.matrix);
    //set the type
    g->type = ADJ_LIST;
    //update pts
    g->adj.list = list;
}

void adjL2adjM(Graph *g)
{
    // Question 2
    // Write your code here
    if (g->type == ADJ_MATRIX)
    {
        printf("Error\n");
        return;
    }

    if (g->V == 0)
    {
        printf("Empty graph\n");
        return;
    }

    //Initialize the matrix
    int **matrix =  (int**) malloc(sizeof(int*) * g->V);
    int i,j;
    for(i=0;i<g->V;i++){
        matrix[i] = malloc(g->V * sizeof(int));
        for(j=0;j<g->V;j++){
            matrix[i][j] = 0;
        }
    }

    //Convert the AdjList to AdjM
    for(i=0;i<g->V;i++){
        ListNode *listHead = g->adj.list[i];
        ListNode *toFree = NULL;
        while(listHead){
            toFree = listHead;
            matrix[i][(listHead->vertex)-1] = 1;
            listHead = listHead->next;
            free(toFree);
        }
    }

    //Cleaup
    free(g->adj.list);
    g->type = ADJ_MATRIX;
    g->adj.matrix = matrix;
}

void calDegreeV(Graph g, int *degreeV)
{
    // Question 3
    // Write your code here
    if (g.V == 0)
    {
        printf("Empty graph\n");
        return;
    }

    if(g.type == ADJ_LIST){
        int i;
        for(i=0;i<g.V;i++){
            ListNode *head = g.adj.list[i];
            int count = 0;
            while(head){
                count++;
                head = head->next;
            }
            degreeV[i] = count;
        }
    }else{
        int i,j;
        for(i=0;i<g.V;i++){
            int count = 0;
            for(j=0;j<g.V;j++){
                count += g.adj.matrix[i][j];
            }
            degreeV[i] = count;
        }
    }
}
