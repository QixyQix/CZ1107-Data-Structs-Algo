#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define SIZE 80 //The size of the array

enum ExpType
{
    OPT,
    OPERAND
};

typedef struct _stackNode
{
    char item;
    struct _stackNode *next;
} StackNode;

typedef struct _stack
{
    int size;
    StackNode *head;
} Stack;

void push(Stack *sPtr, char item);
int pop(Stack *sPtr);
char peek(Stack s);
int isEmptyStack(Stack s);
int precedence(char operator);

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

void in2PreLL(char *infix, LinkedList *inExpLL);

void printExpLL(LinkedList inExpLL);

int main()
{
    char infix[SIZE];

    printf("Enter an infix expression:\n");
    gets(infix);

    LinkedList inExpLL;
    inExpLL.head = NULL;
    inExpLL.size = 0;

    in2PreLL(infix, &inExpLL);

    printExpLL(inExpLL);

    removeAllNodes(&inExpLL);
    return 0;
}

void in2PreLL(char *infix, LinkedList *inExpLL)
{

    //Write your code here
    //Move our char pointer to the end of the string
    char *cur = infix;
    while (*cur != '\0')
    {
        cur++;
    }
    cur--;

    //Initialize our stack
    Stack s;
    s.head = NULL;
    s.size = 0;

    int tensMultiplier = 1;
    int digitCounter = 0;
    int currentNumber = 0;
    while (cur != (infix - 1))
    {
        if (!isdigit(*cur))
        {
            //Case not digit - an operator
            if (digitCounter > 0)
            {
                //INSERT THE NUMBER INTO THE LL
                insertNode(inExpLL, currentNumber, OPERAND);
                //Reset the variables to support number conversion
                currentNumber = 0;
                tensMultiplier = 1;
                digitCounter = 0;
            }

            //PROCESS THE OPERATOR
            if (*cur == ')')
            {
                push(&s, *cur);
            }
            else if (*cur == '(')
            {
                while (peek(s) != ')')
                {
                    insertNode(inExpLL, (s.head)->item, OPT);
                    pop(&s);
                }
                //To get rid of the closing bracket
                pop(&s);
            }
            else
            {
                //Operator +-*/
                if(isEmptyStack(s)){
                    push(&s,*cur);
                }else{
                    //While the precedence of the current is smaller than the precedence of the top of stack
                    //Pop and insert into the LL as we are supposed to process the higher precedence one first
                    while(!isEmptyStack(s) &&  precedence(*cur) < precedence(peek(s)) && peek(s) != ')'){
                        insertNode(inExpLL,peek(s),OPT);
                        pop(&s);
                    }
                    push(&s,*cur);
                }
            }
        }
        else
        {
            //It is a digit
            currentNumber += tensMultiplier * (*cur - '0');
            tensMultiplier *= 10;
            digitCounter++;
        }

        cur--;
    }

    //Outside of the loop, add any extra numbers left behind and empty the stack
    if (digitCounter > 0)
    {
        insertNode(inExpLL, currentNumber, OPERAND);
    }
    //Empty the stack into the prefix expression
    while (!isEmptyStack(s))
    {
        insertNode(inExpLL, (s.head)->item, OPT);
        pop(&s);
    }

    //TEST CASE: 99+(88-77)*(66/(55-44)+33)
    //EXPECTED OUTPUT:  +  99  *  -  88  77  +  /  66  -  55  44  33
}

int precedence(char operator){
    if(operator == '*' || operator == '/')
        return 1;
    else
        return 0;
}

void printExpLL(LinkedList inExpLL)
{
    ListNode *temp = NULL;
    temp = inExpLL.head;
    while (temp != NULL)
    {
        if (temp->type == OPERAND)
            printf(" %d ", temp->item);
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
    {
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

void push(Stack *sPtr, char item)
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

char peek(Stack s)
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
