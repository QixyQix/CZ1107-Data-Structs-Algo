#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define SIZE 80 //The limit of expression length

typedef struct _stackNode
{
    double item;
    struct _stackNode *next;
} StackNode;

typedef struct _stack
{
    int size;
    StackNode *head;
} Stack;

void push(Stack *sPtr, double item);
int pop(Stack *sPtr);
double peek(Stack s);
int isEmptyStack(Stack s);

double exePostfix(char *);

int main()
{
    char postfix[SIZE];

    printf("Enter a postfix expression:\n");
    gets(postfix);

    printf("The answer is %.2lf.\n", exePostfix(postfix));

    return 0;
}

double exePostfix(char* postfix)
{
    //NOTE: The question states that it you may assume each operand is a single digit
	// Write your code here
    Stack s;
    s.head = NULL;
    s.size = 0;

    char *cur = postfix;
    while(*cur != '\0'){
        if(isdigit(*cur)){
            //Always push operands
            push(&s,(double) *cur-'0');
        }else{
            //Is an operator
            double operandA = peek(s);
            pop(&s);
            double operandB = peek(s);
            pop(&s);
            double result = 0;

            switch(*cur){
                case '-':
                    //AB- ==> A-B note A is operandB, and B is operandA
                    result = operandB - operandA;
                    break;
                case '+':
                    result = operandB + operandA;
                    break;
                case '*':
                    result = operandB * operandA;
                    break;
                case '/':
                    result = operandB / operandA;
                    break;
            }
            push(&s, result);
        }
        cur++;
    }

    return peek(s);
}

void push(Stack *sPtr, double item)
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

double peek(Stack s)
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
