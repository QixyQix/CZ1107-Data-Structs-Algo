////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>

////////////////////////////////////////////////////////////////////

typedef struct _btnode
{
	int item;
	struct _btnode *left;
	struct _btnode *right;

} BTNode;

////////////////////////////////////////////////////////////////////

void mirrorTree(BTNode *node);

void printSmallerValues(BTNode *node, int m);
int smallestValue(BTNode *node);
int hasGreatGrandchild(BTNode *node);

void printTree_InOrder(BTNode *node);

////////////////////////////////////////////////////////////////////

int main(int argc, const char *argv[])
{

	int i;
	BTNode *root, *root2;
	BTNode btn[15];

	// Create the tree in Q1
	// Using manual dynamic allocation of memory for BTNodes

	root = malloc(sizeof(BTNode));
	root->item = 4;

	root->left = malloc(sizeof(BTNode));
	root->left->item = 5;

	root->right = malloc(sizeof(BTNode));
	root->right->item = 2;

	root->left->left = NULL;

	root->left->right = malloc(sizeof(BTNode));
	root->left->right->item = 6;

	root->left->right->left = NULL;
	root->left->right->right = NULL;

	root->right->left = malloc(sizeof(BTNode));
	root->right->left->item = 3;

	root->right->right = malloc(sizeof(BTNode));
	root->right->right->item = 1;

	root->right->left->left = NULL;

	root->right->left->right = NULL;

	root->right->right->left = NULL;

	root->right->right->right = NULL;

	printTree_InOrder(root);
	printf("\n");
	mirrorTree(root);
	printTree_InOrder(root);
	printf("\n\n");

	//question 2
	printf("\n input m for question 2:");
	scanf("%d", &i);
	printf("the values smaller than %d are:", i);
	printSmallerValues(root, i);
	printf("\n\n");

	//question 3
	printf("The smallest value in the tree is %d:\n", smallestValue(root));

	//question 4
	// Create a tree for Q4: Tall enough so some nodes have great-grandchildren
	// Use array of BTNodes, create tree by linking nodes together
	for (i = 0; i <= 6; i++)
	{
		btn[i].item = i;
		btn[i].left = &(btn[i * 2 + 1]);
		btn[i].right = &(btn[i * 2 + 2]);
	}

	for (i = 7; i <= 14; i++)
	{
		btn[i].item = i;
		btn[i].left = NULL;
		btn[i].right = NULL;
	}
	root2 = &btn[0];

	printf("The tree for question 4 visited by in-order is \n");
	printTree_InOrder(root2);
	printf("\nthe values stored in all nodes of the tree that have at least one great-grandchild are: ");

	hasGreatGrandchild(root2);

	return 0;
}

/////////////////////// QUESTION 1 ///////////////////////
void mirrorTree(BTNode *node)
{

	// write your code here
	//Recursive Function
	//Base Case: Node is null, reach the end of the tree
	if (node == NULL)
	{
		return;
	}
	//Iterative Step:
	//Mirror the current node first
	BTNode *temp = node->left;
	node->left = node->right;
	node->right = temp;

	//Mirror the childs next
	mirrorTree(node->left);
	mirrorTree(node->right);
}

/////////////////////// QUESTION 2 ///////////////////////
void printSmallerValues(BTNode *node, int m)
{

	// write your code here
	//Recursive Function
	//Base case: current node item is smaller than m
	if (node == NULL)
	{
		return;
	}
	if (node->item < m)
	{
		printf("%d ", node->item);
	}

	//Iterative Step:
	//Print Smaller Values of child nodes
	printSmallerValues(node->left, m);
	printSmallerValues(node->right, m);
}

/////////////////////// QUESTION 3 ///////////////////////
int smallestValue(BTNode *node)
{
	int l, r;

	// write your code here
	// Recursive Function
	// Base case: node is end of branch, left and right should be null
	if (node->left == NULL && node->right == NULL)
	{
		return node->item;
	}

	if (node->left != NULL)
		l = smallestValue(node->left);
	if (node->right != NULL)
		r = smallestValue(node->right);

	//Check: If current node item is the smallest among the rest, return current node
	if (node->item < l && node->item < r)
		return node->item;
	else if (l > r)
		return r;
	else
		return l;
}

/////////////////////// QUESTION 4 ///////////////////////
int hasGreatGrandchildOLD(BTNode *node)
{
	//*****IGNORE THIS -- IT COMPUTERS GRANDCHILD ONLY*****
	// write your code here
	// Base Case: End node
	if (node == NULL)
		return;
	//Common property: At least one left/right child's childs not NULL
	if (node->left != NULL)
	{
		if (node->left->left != NULL || node->left->right != NULL)
			printf("%d ", node->item);
	}
	else if (node->right != NULL)
	{
		if (node->right->left != NULL || node->right->right != NULL)
			printf("%d ", node->item);
	}

	//Recursively call next node
	hasGreatGrandchild(node->left);
	hasGreatGrandchild(node->right);
}

int hasGreatGrandchild(BTNode *node)
{
	//Recursive function: Compute the height of each path
	//Base Case: end of branch
	if (node == NULL)
		return 0;

	int leftRes;
	int rightRes;
	//Iterative Step: compute height of child nodes, and add 1
	leftRes = 1 + hasGreatGrandchild(node->left);
	rightRes = 1 + hasGreatGrandchild(node->right);

	//Override leftRes if rightRes is higher
	if(rightRes > leftRes)
		leftRes = rightRes;
	//Print if the largest height is at least 3
	if (leftRes >= 3)
		printf("%d ", node->item);

	return rightRes;
}

//////////////////////////////////////////////////////////////////

void printTree_InOrder(BTNode *node)
{

	if (node == NULL)
		return;
	printTree_InOrder(node->left);
	printf("%d, ", node->item);
	printTree_InOrder(node->right);
	return;
}
