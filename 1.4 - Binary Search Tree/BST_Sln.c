//////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>

///////////////////////////////////////////////////////////////////////

typedef struct _btnode
{
	int item;
	struct _btnode *left;
	struct _btnode *right;
} BTNode;

///////////////////////////////////////////////////////////////////////

void insertBSTNode(BTNode **node, int value);
void printBSTInOrder(BTNode *node);
int isBST(BTNode *node, int min, int max);
BTNode *removeBSTNode(BTNode *node, int value);
BTNode *findMin(BTNode *p);

///////////////////////////////////////////////////////////////////////

int main()
{
	int i = 0;

	BTNode *root = NULL;

	//question 1
	do
	{
		printf("input a value you want to insert(-1 to quit):");

		scanf("%d", &i);
		if (i != -1)
			insertBSTNode(&root, i);
	} while (i != -1);

	//question 2
	printf("\n");
	printBSTInOrder(root);

	//question 3
	if (isBST(root, -1000000, 1000000) == 1)
		printf("It is a BST!\n");
	else
		printf("It is not a BST!\n");

	//question 4
	do
	{
		printf("\ninput a value you want to remove(-1 to quit):");
		scanf("%d", &i);
		if (i != -1)
		{
			root = removeBSTNode(root, i);
			printBSTInOrder(root);
		}
	} while (i != -1);

	return 0;
}

//////////////////////////////////////////////////////////////////////

void insertBSTNode(BTNode **node, int value)
{
	// write your code here
	//Recursive function
	//Base case: tree is empty/final location to be inserted
	if (*node == NULL)
	{
		*node = malloc(sizeof(BTNode));
		(*node)->item = value;
		(*node)->left = NULL;
		(*node)->right = NULL;
	}
	else
	{
		//Iterative step: call the function recursively
		//to insert the node in the appropriate location
		if ((*node)->item == value)
		{
			//BST cannot have duplicate items
			printf("Node not inserted - Duplicate item\n");
		}
		else if ((*node)->item > value)
		{
			//Value is smaller then current node, should be inserted on the left
			insertBSTNode(&((*node)->left), value);
		}
		else
		{
			//Value is larger than current node, should be inserted on the right
			insertBSTNode(&((*node)->right), value);
		}
	}
}

//////////////////////////////////////////////////////////////////////

void printBSTInOrder(BTNode *node)
{
	// write your code here
	//Recursive Function
	//Base case: node is null
	if (node == NULL)
		return;

	//Print values smaller than current node first
	printBSTInOrder(node->left);
	//Print current node item
	printf("%d, ", node->item);
	//Print values larger than current node
	printBSTInOrder(node->right);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int isBST(BTNode *node, int min, int max) // the item stored in node has to be smaller than max and larger than min
{
	// write your code here
	//Recursive Function
	//Base case: node is null
	if (node == NULL)
		return 1;
	//Base case: node's value should be in range (min, max)
	if (node->item >= max || node->item <= min)
		return 0;

	//Iterative step: recursively call the function to check if left and right subtrees are BST
	//Left side: inherits the min range from parent. Should be smaller than value of current node
	int leftResult = isBST(node->left, min, node->item);
	//Right side: inherits the max range from parent. Should be larger than value of current node
	int rightResult = isBST(node->right, node->item, max);
	//Both left and right side must be BST. If either result is 0, returns 0
	return leftResult && rightResult;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

BTNode *removeBSTNode(BTNode *node, int value)
{
	// write your code here
	//Base case: node is null, value does not exist
	if (node == NULL)
	{
		printf("Can't find the value!\n");
		return NULL;
	}

	if (value < node->item)
	{
		//Value is smaller than the current node item
		node->left = removeBSTNode(node->left, value);
	}
	else if (value > node->item)
	{
		//Value is larer than the current node item
		node->right = removeBSTNode(node->right, value);
	}
	else
	{
		//Node is found (not more than, not less than, means its equal)
		if (node->left != NULL && node->right != NULL)
		{
			//Node has 2 children, find & replace with successor
			//(next value larger than but smaller than current value)
			BTNode *replacementNode;
			replacementNode = findMin(node->right);
			node->item = replacementNode->item;
			//Remove the replacement node
			node->right = removeBSTNode(node->right, replacementNode->item);
		}
		else
		{
			//Node has no children or only one child
			BTNode *nodeToRemove = node;
			nodeToRemove = node;
			if (node->left != NULL)
			{
				//Set the left node to be returned later
				node = node->left;
			}
			else
			{
				//Set the right node to be returned later
				node = node->right;
			}
			//Remove the node
			free(nodeToRemove);
		}
	}
	return node;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

BTNode *findMin(BTNode *p)
{
	// write your code here
	//Recursive function
	if (p->left != NULL)
	{
		//There exists an item smaller than p, call function recursively
		return findMin(p->left);
	}
	else
	{
		//There is no item smaller than p, return p
		return p;
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
