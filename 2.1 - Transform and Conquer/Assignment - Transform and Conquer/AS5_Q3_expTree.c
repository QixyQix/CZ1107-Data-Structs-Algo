#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define SIZE 200 //The number digit limitation

typedef struct _btnode
{
    int item;
    struct _btnode *left;
    struct _btnode *right;
} BTNode; // You should not change the definition of BTNode

typedef struct _node
{
    BTNode *item;
    struct _node *next;
} StackNode;

typedef struct _stack
{
    int size;
    StackNode *head;
} Stack;

void deleteTree(BTNode **root);

void createExpTree(BTNode **root, char *prefix);
void printTree(BTNode *node);
void printTreePostfix(BTNode *node);
double computeTree(BTNode *node);

void push(Stack *sPtr, BTNode *item);
int pop(Stack *sPtr);
BTNode *peek(Stack s);
int isEmptyStack(Stack s);

int main()
{
    char prefix[SIZE];
    BTNode *root = NULL;

    printf("Enter an prefix expression:\n");
    gets(prefix);

    createExpTree(&root, prefix);

    printf("The Infix Expression:\n");
    printTree(root);
    printf("\n");
    printf("The Postfix (Reverse Polish) Expression:\n");
    printTreePostfix(root);
    printf("\n");
    printf("Answer ");
    printf("%.2f\n", computeTree(root));
    deleteTree(&root);
    return 0;
}

void createExpTree(BTNode **root, char *prefix)
{
    //Write your code here
    //Initialize a stack
    Stack s;
    s.head = NULL;
    s.size = 0;
    Stack *sp = &s;

    //Move char pointer to the end
    char *cur = prefix;
    while (*cur != '\0')
    {
        cur++;
    }
    cur--;

    //Numbers to support number conversion
    int digitCount = 0;
    int tensMultiplier = 1;
    int currentNumber = 0;

    while (cur != (prefix - 1))
    {
        if (isdigit(*cur))
        {
            //Conversion to numbers
            currentNumber += tensMultiplier * (*cur - '0');
            tensMultiplier *= 10;
            digitCount++;
        }
        else if (*cur == ' ' && digitCount > 0)
        {
            BTNode *newNumberNode = malloc(sizeof(BTNode));
            newNumberNode->item = currentNumber;
            newNumberNode->left = NULL;
            newNumberNode->right = NULL;

            //Push it into the stack
            push(sp, newNumberNode);

            //Reset the number conversions
            digitCount = 0;
            tensMultiplier = 1;
            currentNumber = 0;
        }
        else if (*cur != ' ')
        {
            //Cur is an operator
            BTNode *newOperatorNode = malloc(sizeof(BTNode));
            newOperatorNode->item = *cur;
            newOperatorNode->left = NULL;
            newOperatorNode->right = NULL;

            if (!isEmptyStack(s))
            {
                newOperatorNode->left = peek(s);
                pop(sp);
            }
            if (!isEmptyStack(s))
            {
                newOperatorNode->right = peek(s);
                pop(sp);
            }
            //Push the newOperatorNode onto the stack
            push(sp, newOperatorNode);
            
        }
        //else cur is whitespace, discard
        cur--;
    }
    *root = peek(s);
    //TEST: + - + 1 2 * 3 4 5
    //EXPECTED: 1 + 2 - 3 * 4 + 5
}

void printTree(BTNode *node)
{
    //Write your code here
    //Infix ==> In-order traversal
    if (node == NULL)
    {
        return;
    }

    printTree(node->left);

    //print current
    if (!node->left && !node->right)
        printf("%d ", node->item); //If its a number left & right is null
    else
        printf("%c ", node->item); //Otherwise, operator always has child

    printTree(node->right);
}

void printTreePostfix(BTNode *node)
{
    //Write your code here
    //Postfix ==> Post order traversal
    if (node == NULL)
    {
        return;
    }

    printTreePostfix(node->left);
    printTreePostfix(node->right);

    //print current
    if (!node->left && !node->right)
        printf("%d ", node->item); //If its a number left & right is null
    else
        printf("%c ", node->item); //Otherwise, operator always has child
}

double computeTree(BTNode *node)
{
    //Write your code here
    if (node == NULL)
    {
        return 0.0;
    }
    else if (!node->left && !node->right)
    {
        return (double)node->item;
    }
    else
    {
        double left = computeTree(node->left);
        double right = computeTree(node->right);

        switch ((char)node->item)
        {
        case '+':
            return left + right;
        case '-':
            return left - right;
        case '/':
            return left / right;
        case '*':
            return left * right;
        }
    }
}

void push(Stack *sPtr, BTNode *item)
{
    StackNode *newNode;
    newNode = malloc(sizeof(StackNode));
    newNode->item = item;
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

BTNode *peek(Stack s)
{
    return s.head->item;
}

int isEmptyStack(Stack s)
{
    if (s.size == 0)
        return 1;
    else
        return 0;
}

void deleteTree(BTNode **root)
{
    BTNode *temp;
    if (*root != NULL)
    {
        temp = (*root)->right;
        deleteTree(&((*root)->left));
        free(*root);
        *root = NULL;
        deleteTree(&temp);
    }
}