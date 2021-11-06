#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define SIZE 80 //The limit of expression length

typedef struct _stackNode{
    char item;
    struct _stackNode *next;
}StackNode;

typedef struct _stack{
   int size;
   StackNode *head;
}Stack;

void push(Stack *sPtr, char item);
int pop(Stack *sPtr);
char peek(Stack s);
int isEmptyStack(Stack s);

void in2Pre(char*, char*);

int main()
{
    char infix[SIZE];
    char prefix[SIZE];

    printf("Enter an infix expression:\n");
    gets(infix);

    in2Pre(infix,prefix);
    printf("The prefix expression is \n");
    printf("%s\n",prefix);

    return 0;
}

void in2Pre(char *infix, char *prefix)
{
    // Write your code here
    Stack s;
    s.head = NULL;
    s.size = 0;

    char *cur = infix;
    while(*cur != '\0')
        cur++;

    cur--;

    char *curPre = prefix;

    while(cur != infix-1){
        if(isalnum(*cur)){
            *curPre = *cur;
            curPre++;
        }else if(*cur == ')'){
            push(&s, *cur);
        }
        else if(*cur == '('){
            while(!isEmptyStack(s) && peek(s) != ')'){
                *curPre = peek(s);
                pop(&s);
                curPre++;
            }
            pop(&s);
        }else{
            while(!isEmptyStack(s) && peek(s) != ')' && precedence(*cur) < precedence(peek(s))){
                *curPre = peek(s);
                curPre++;
                pop(&s);
            }
            push(&s, *cur);
        }
        cur--;
    }

    while(!isEmptyStack(s)){
        *curPre = peek(s);
        curPre++;
        pop(&s);
    }

    *curPre = '\0';

    //NOTE: FOR PREFIX WE NEED TO REVERSE THE STRING
    int stringLength = curPre-1 - prefix;
    int i;
    int loopsNeeded = stringLength/2;
    for(i=0;i<loopsNeeded;i++){
        char temp = *(prefix+i);
        *(prefix+i) = *(curPre-1-i);
        *(curPre-1-i) = temp;
    }
}

int precedence(char cur){
    switch(cur){
        case '+':
        case '-':
            return 0;
        case '*':
        case '/':
            return 1;
    }

}

void push(Stack *sPtr, char item){
    StackNode *newNode;
    newNode = malloc(sizeof(StackNode));
    newNode->item = item;
    newNode->next = sPtr->head;
    sPtr->head = newNode;
    sPtr->size++;
}

int pop(Stack *sPtr){
    if(sPtr == NULL || sPtr->head == NULL){
        return 0;
    }
    else{
       StackNode *temp = sPtr->head;
       sPtr->head = sPtr->head->next;
       free(temp);
       sPtr->size--;
       return 1;
    }
}

char peek(Stack s){
    return s.head->item;
}

int isEmptyStack(Stack s){
     if(s.size == 0) return 1;
     else return 0;
}
