#include <stdio.h>
#include <stdlib.h>

void dualSearch(int[], int, int, int[]);

int main()
{
    int i, size, K;
    int *A;
    int index[2] = {-1, -1};

    //Search key
    printf("Enter Search Key K: \n");
    scanf("%d", &K);

    //create an array
    printf("Enter Array size: \n");
    scanf("%d", &size);
    A = (int *)malloc(sizeof(int) * size);

    printf("Enter %d data: \n", size);
    for (i = 0; i < size; i++)
    {
        scanf("%d", &A[i]);
    }

    dualSearch(A, size, K, index);

    if (index[0] != -1)
    {
        printf("Indexes in array whose data adds to sum %d: \n", K);
        printf("%d %d\n", index[0], index[1]);
    }
    else
    {
        printf("Not found");
    }

    free(A);
    return 0;
}

void dualSearch(int A[], int size, int K, int dualIndex[])
{
    // Write your code here
    int i;
    //Nested for loop
    for (i = 0; i < size; i++)
    {
        int numA = A[i];
        //Check if 2 of current number is the sum
        if ((numA * 2) == K)
        {
            dualIndex[0] = i;
            dualIndex[1] = i;
            return;
        }
        int j;
        //Otherwise, iterate through the whole LL again to find another appropriate member
        for (j = 0; j < size; j++)
        {
            int numB = A[j];
            if ((numA + numB) == K)
            {
                dualIndex[0] = i;
                dualIndex[1] = j;
                return;
            }
        }
    }
}
