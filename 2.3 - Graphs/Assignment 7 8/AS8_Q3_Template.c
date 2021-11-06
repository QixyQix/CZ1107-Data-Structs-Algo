#include <stdio.h>
#include <stdlib.h>

typedef struct _arraynode
{
	int *itemArray;
	int sizeArray;
	struct _arraynode *next;
} ArrayNode;

typedef struct _arraylist
{
	int size; //the size of a possible sequence
	ArrayNode *head;
} ArrayList;

typedef struct _listnode
{
	int item;
	struct _listnode *next;
} ListNode;

typedef struct _linkedlist
{
	int sum; //the number of possible sequences
	int size;
	ListNode *head;
} LinkedList;

// You should not change the prototypes of these functions
//////LinkedList///////////////////////////////////////////
int insertNode(LinkedList *ll, int index, int coin);
int removeNode(LinkedList *ll, int index);
ListNode *findNode(LinkedList ll, int index);
void removeAllItems(LinkedList *ll);
///////////////////////////////////////////////////
//Custom Prototypes
void insertIntoAl(ArrayList *al, LinkedList *ll);

void sumToC(LinkedList *ll, int C, ArrayList *al);
int main()
{
	int C;
	printf("Enter a number:\n");
	scanf("%d", &C);

	LinkedList ll;
	ll.head = NULL;
	ll.sum = 0;
	ll.size = 0;
	ArrayList al;
	al.head = NULL;
	al.size = 0;

	sumToC(&ll, C, &al);

	//printing all possible sequences
	ArrayNode *temp;
	int i, j;
	temp = al.head;
	for (i = 0; i < al.size; i++)
	{
		for (j = 0; j < temp->sizeArray; j++)
			printf(" %d ", temp->itemArray[j]);
		printf("\n");
		temp = temp->next;
	}

	return 0;
}

void sumToC(LinkedList *ll, int C, ArrayList *al)
{
	//Write your code here
	int baseNumber;
	for (baseNumber = 1; baseNumber <= C; baseNumber++)
	{
		//Clear the LinkedList
		removeAllItems(ll);
		//Insert the base into the linkedlist
		insertNode(ll, ll->size, baseNumber);

		int checkNumber = baseNumber+1;
		while(checkNumber < C)
		{
			//CHECK NUMBER INSERT INTO END OF LL
			insertNode(ll, ll->size, checkNumber);

			//CASE: Meets condition == C
			if (ll->sum == C)
			{
				//Insert into the AL
				insertIntoAl(al,ll);
				removeNode(ll,(ll->size)-1);
				
				checkNumber = (findNode(*ll,(ll->size)-1)->item)+1;
				removeNode(ll,(ll->size)-1);
				//insertNode(ll, ll->size, checkNumber);
				//checkNumber++;
			}
			else if(ll->sum > C){
				//CASE: Exceeds condition C, backtrack!
				if(ll->size > 2){
					//If the linkedlist size is larger than 2
					//BaseNumber + ... + (checkNumber-1) + checkNumber > C

					//Remove checkNumber
					removeNode(ll,(ll->size)-1);
					//Remove the number before the check number
					removeNode(ll,(ll->size)-1);

					//But retain the check number, still possibility the number before when added will succeed
					//It will be reinserted
				}else{
					//Otherwise, it is just Base number+check number > C
					//It is impossible for any number higher than the check number to be == C
					//As we have already exhausted our previous options
					break;
				}
				// checkNumber = (findNode(*ll,(ll->size)-1)->item)+1;
				// removeNode(ll,(ll->size)-1);
				// removeNode(ll,(ll->size)-1);
			}else{
				//Otherwise, Below condition C, proceed to add the next number

				checkNumber = (findNode(*ll,(ll->size)-1)->item)+1;
			}
		}

		//LAST CHECK: Only item inside is the sum
		if(ll->sum == C){
			insertIntoAl(al,ll);
		}
	}
}

void insertIntoAl(ArrayList *al, LinkedList *ll){
	//Convert into a array first
	int *intArray = malloc((ll->size) * sizeof(int));
	int i = 0;
	ListNode *cur = ll->head;
	while (cur != NULL)
	{
		intArray[i] = cur->item;
		cur = cur->next;
		i++;
	}

	//Create a new ArrayNode for insertion into the ArrayList
	ArrayNode *newSln = malloc(sizeof(ArrayNode));
	newSln->itemArray = intArray;
	newSln->sizeArray = ll->size;
	newSln->next = NULL;

	//Insert ArrayNode into the ArrayList
	if(al->size == 0){
		al->head = newSln;
		al->size++;
	}else{
		ArrayNode *cur = al->head;
		while(cur->next != NULL){
			cur = cur->next;
		}
		cur->next = newSln;
		al->size++;
	}
}

void printLL(LinkedList *ll){
	ListNode *cur= ll->head;
	while (cur != NULL)
	{
		printf("%d ",cur->item);
		cur = cur->next;
	}
	
}
///////////////////////////////////////////////////////
int insertNode(LinkedList *ll, int index, int value)
{

	ListNode *pre, *cur;

	if (ll == NULL || index < 0 || index > ll->size)
		return 0;

	if (index == 0)
	{
		cur = ll->head;
		ll->head = (ListNode *)malloc(sizeof(ListNode));
		ll->head->item = value;
		ll->head->next = cur;
		ll->sum += value;
		ll->size++;
		return 1;
	}

	// Find the nodes before and at the target position
	// Create a new node and reconnect the links
	if ((pre = findNode(*ll, index - 1)) != NULL)
	{
		cur = pre->next;
		pre->next = malloc(sizeof(ListNode));
		pre->next->item = value;
		pre->next->next = cur;
		ll->sum += value;
		ll->size++;
		return 1;
	}

	return 0;
}

int removeNode(LinkedList *ll, int index)
{

	ListNode *pre, *cur;

	// Highest index we can remove is size-1
	if (ll == NULL || index < 0 || index > ll->size)
		return 0;

	// If removing first node, need to update head pointer
	if (index == 0)
	{
		cur = ll->head->next;
		ll->sum -= ll->head->item;
		free(ll->head);
		ll->head = cur;
		ll->size--;
		return 1;
	}

	// Find the nodes before and after the target position
	// Free the target node and reconnect the links
	if ((pre = findNode(*ll, index - 1)) != NULL)
	{

		if (pre->next == NULL)
			return 0;

		cur = pre->next;
		ll->sum -= cur->item;
		pre->next = cur->next;
		free(cur);
		ll->size--;
		return 1;
	}

	return 0;
}

ListNode *findNode(LinkedList ll, int index)
{

	ListNode *temp;

	if (index < 0 || index >= ll.size)
		return NULL;

	temp = ll.head;

	if (temp == NULL || index < 0)
		return NULL;

	while (index > 0)
	{
		temp = temp->next;
		if (temp == NULL)
			return NULL;
		index--;
	}

	return temp;
}

void removeAllItems(LinkedList *ll)
{
	ListNode *cur = ll->head;
	ListNode *tmp;

	while (cur != NULL)
	{
		tmp = cur->next;
		free(cur);
		cur = tmp;
	}
	ll->head = NULL;
	ll->size = 0;
	ll->sum = 0;
}
