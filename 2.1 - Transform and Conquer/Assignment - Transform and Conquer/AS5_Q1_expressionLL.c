#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define SIZE 80 //The size of the array

enum ExpType
{
    OPT,
    OPERAND
};

typedef struct _listnode
{
    int item;
    enum ExpType type;
    struct _listnode *next;
} ListNode;

typedef struct _linkedlist
{
    int size;
    ListNode *head;
} LinkedList;

void insertNode(LinkedList *llPtr, int item, enum ExpType type);
int deleteNode(LinkedList *llPtr);
void removeAllNodes(LinkedList *llPtr);
int isEmptyLinkedList(LinkedList ll);

void expressionLL(char *infix, LinkedList *inExpLL);

void printExpLL(LinkedList inExpLL, int seed);

int main()
{
    char infix[SIZE];

    printf("Enter an infix expression:\n");
    gets(infix);

    LinkedList inExpLL;
    inExpLL.head = NULL;
    inExpLL.size = 0;

    expressionLL(infix, &inExpLL);

    int seed;
    printf("Enter a seed: \n");
    scanf("%d", &seed);
    printExpLL(inExpLL, seed);

    removeAllNodes(&inExpLL);
    return 0;
}

void expressionLL(char *infix, LinkedList *inExpLL)
{
    //Write your code here
    //Empty the linkedlist
    removeAllNodes(inExpLL);

    //We will use this char ptr to iterate through the string
    char *currChar = infix;
    while (*currChar != '\0')
    {
        currChar++;
    }
    //Adjust so that currChar points to last character
    currChar--;

    int numberVal = 0;
    int tensMultiplier = 1;
    //Iterate from the back
    while (currChar != (infix - 1))
    {
        if (!isdigit(*currChar))
        {
            //Insert previous number if applicable
            if (numberVal > 0)
            {
                insertNode(inExpLL, numberVal, OPERAND);
            }
            //Operator, insert into the LL and reset tracking for number evaluation
            insertNode(inExpLL, *currChar, OPT);
            numberVal = 0;
            tensMultiplier = 1;
        }
        else
        {
            //Since it is corresponding to the ASCII table
            //When we subtract '1' from '0' we get the difference
            //Which is in ascending order, essentially getting the value
            int number = *currChar - '0';
            numberVal += tensMultiplier * number;
            tensMultiplier *= 10;
        }
        currChar--;
    }
    //Sanity check: remaining numbers behind
    if (numberVal > 0)
    {
        insertNode(inExpLL, numberVal, OPERAND);
    }
}

void printExpLL(LinkedList inExpLL, int seed)
{
    ListNode *temp = NULL;
    temp = inExpLL.head;
    while (temp != NULL)
    {
        if (temp->type == OPERAND)
            printf(" %d ", temp->item + seed);
        else
            printf(" %c ", (char)(temp->item));
        temp = temp->next;
    }
    printf("\n");
}

void insertNode(LinkedList *LLPtr, int item, enum ExpType type)
{
    ListNode *newNode;
    newNode = malloc(sizeof(ListNode));
    if (newNode == NULL)
        exit(0);

    newNode->item = item;
    newNode->type = type;
    newNode->next = LLPtr->head;
    LLPtr->head = newNode;
    LLPtr->size++;
}

int deleteNode(LinkedList *LLPtr)
{
    if (LLPtr == NULL || LLPtr->size == 0)
    { //Queue is empty or NULL pointer
        return 0;
    }
    else
    {
        ListNode *temp = LLPtr->head;
        LLPtr->head = LLPtr->head->next;

        free(temp);
        LLPtr->size--;
        return 1;
    }
}

int isEmptyLinkedList(LinkedList ll)
{
    if (ll.size == 0)
        return 1;
    else
        return 0;
}

void removeAllNodes(LinkedList *LLPtr)
{
    while (deleteNode(LLPtr))
        ;
}
