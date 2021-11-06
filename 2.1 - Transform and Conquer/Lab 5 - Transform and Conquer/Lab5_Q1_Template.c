#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define SIZE 80 //The limit of expression length

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
int precedence(char opt);

void in2Post(char *, char *);

int main()
{
    char infix[SIZE];
    char postfix[SIZE];

    printf("Enter an infix expression:\n");
    gets(infix);

    in2Post(infix, postfix);
    printf("The postfix expression is \n");
    printf("%s\n", postfix);
    return 0;
}

void in2Post(char *infix, char *postfix)
{
    // Write your code here
    //Initialize Stack
    Stack s;
    s.head = NULL;
    s.size = 0;

    Stack *sp = &s;

    //Start
    char *c = infix;
    char *p = postfix;

    while (*c != '\0')
    {
        if (!isalnum(*c))
        {
            if (*c == '(')
            {
                //Push the opening bracket into stack
                push(sp, *c);
            }
            else if (*c == ')')
            {
                //Closing bracket the items in the paranthesis
                while (!isEmptyStack(s) && peek(s) != '(')
                {
                    *p = peek(s);
                    p++;
                    pop(sp);
                }
                //Discard the opening bracket
                pop(sp);
            }
            else
            {
                while (!isEmptyStack(s) && peek(s) != '(' && precedence(*c) < precedence(peek(s)))
                {
                    *p = peek(s);
                    p++;
                    pop(sp);
                }
                push(sp, *c);
            }
        }
        else
        {
            *p = *c;
            p++;
        }
        c++;
    }
    while (!isEmptyStack(s))
    {
        *p = peek(s);
        pop(sp);
        p++;
    }

    *p = '\0';
}

int precedence(char opt)
{
    if (opt == '*' || opt == '/')
        return 1;
    else
        return 0;
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
