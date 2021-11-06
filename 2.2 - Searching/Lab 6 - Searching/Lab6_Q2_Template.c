#include <stdio.h>
#include <stdlib.h>

void quickSort(int A[], int low, int high);
int partition (int A[], int low, int high);

void dualSortedSearch (int[],int, int, int[]);

int main()
{
    int i,size,K;
    int* A;
    int index[2] = {-1,-1};

    //Search key
    printf("Enter Search Key K: \n");
    scanf("%d",&K);

    //create an array
    printf("Enter Array size: \n");
    scanf("%d",&size);
    A=(int *)malloc(sizeof(int)*size);

    printf("Enter %d data: \n", size);
    for(i=0; i<size; i++)
    {
        scanf("%d",&A[i]);
    }

    quickSort(A,0,size-1);
    dualSortedSearch(A,size,K,index);

    if(index[0]!=-1)
        printf("%d %d\n",index[0], index[1]);
    else
        printf("Not found");

    free(A);
    return 0;
}

void dualSortedSearch(int A[], int size, int K, int dualIndex[])
{
   // Write your code here
   int i,j,temp;
   //Initialize i to beginning index of array, j to last index of array
   i=0;
   j=size-1;

   //As long as If i becomes larger than j then we have exhausted all options
   //Since the LL is sorted. Values at index j always > values at index i
   while(i<j){
       int sum = A[i] + A[j];
       
       if(sum == K){
           //Case: Sum matches
           dualIndex[0] = i;
           dualIndex[1] = j;
           return;
       }else if(sum > K){
           //Sum is larger than K, move j index down to get a smaller large value
           j--;
       }else{
           //Sum is smaller than K, move i index up to get a larger small value
           i++;
       }
   }
}

int partition (int A[], int low, int high)
{
    int pivot = A[high];
    int i = (low - 1);

    int temp;
    for (int j = low; j <= high- 1; j++)
    {
        if (A[j] < pivot)
        {
            i++;

            temp = A[i];
            A[i] = A[j];
            A[j] = temp;
        }
    }

    temp = A[i+1];
    A[i+1]= A[high];
    A[high] = temp;
    return (i + 1);
}

void quickSort(int A[], int low, int high)
{
    if (low < high)
    {
        int pivot = partition(A, low, high);

        quickSort(A, low, pivot - 1);
        quickSort(A, pivot + 1, high);
    }
}
