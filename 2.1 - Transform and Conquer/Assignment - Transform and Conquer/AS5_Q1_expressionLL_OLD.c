//I did not notice that the question's linkedlist implementation appends items to the start of the LL
//Hence under original conditions, the algo is right, but does not fit the LL's implementation

//In this version, I have modified the LL implementation to fit the algorithm

//TODO: MODIFY THE LL IMPLEMENTATION

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

    //To count the number of digits in the current number
    int digitCounter = 0;

    //Iterate through the string
    while (*infix != '\0')
    {
        if (!isdigit(*infix))
        {
            printf("OPERAND FOUND: %c | DIGIT COUNT: %d\n",*infix,digitCounter);
            
            //Case: hit an operand, store the number
            if (digitCounter > 0)
            {
                //Calculate the number
                int i;
                int value = 0;
                int tensMultilier = 1;

                for (i = 1; i <= digitCounter; i++)
                {
                    //Since it is corresponding to the ASCII table
                    //When we subtract '1' from '0' we get the difference
                    //Which is in ascending order, essentially getting the value
                    int number = *(infix-i) - '0';
                    printf("NUMBER FOUND: %d\n",number);
                    value += tensMultilier * number;
                    tensMultilier *= 10;
                }
                //Insert the number
                printf("NUMBER INSERTED: %d\n",value);
                insertNode(inExpLL, value, OPERAND);
            }

            //Insert the operand
            printf("OPERATOR INSERTED: %c\n",*infix);
            insertNode(inExpLL, *infix, OPT);
            //reset the digit counter
            digitCounter = 0;
        }
        else
        {
            digitCounter++;
        }
        *infix++;
    }

    //Possibility end still has a number
    if (digitCounter > 0)
    {
        //Calculate the number
        int i;
        int value = 0;
        int tensMultilier = 1;

        for (i = 1; i <= digitCounter; i++)
        {
            //Since it is corresponding to the ASCII table
            //When we subtract '1' from '0' we get the difference
            //Which is in ascending order, essentially getting the value
            int number = (*infix - i) - '0';
            value += tensMultilier * number;
            tensMultilier *= 10;
        }
        //Insert the number
        insertNode(inExpLL, value, OPERAND);
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
