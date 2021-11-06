#include <stdio.h>
#include <stdlib.h>

#define TABLESIZE 5
#define PRIME 2

enum Marker
{
    EMPTY,
    USED,
    DELETED
};

typedef struct _slot
{
    int key;
    enum Marker indicator;
} HashSlot;

int HashInsert(int key, HashSlot hashTable[]);
int HashDelete(int key, HashSlot hashTable[]);

int hash1(int key);
int hash2(int key);

int main()
{
    int opt;
    int i;
    int key;
    int comparison;
    HashSlot hashTable[TABLESIZE];

    for (i = 0; i < TABLESIZE; i++)
    {
        hashTable[i].indicator = EMPTY;
        hashTable[i].key = 0;
    }

    printf("============= Hash Table ============\n");
    printf("|1. Insert a key to the hash table  |\n");
    printf("|2. Delete a key from the hash table|\n");
    printf("|3. Print the hash table            |\n");
    printf("|4. Quit                            |\n");
    printf("=====================================\n");
    printf("Enter selection: ");
    scanf("%d", &opt);
    while (opt >= 1 && opt <= 3)
    {
        switch (opt)
        {
        case 1:
            printf("Enter a key to be inserted:\n");
            scanf("%d", &key);
            comparison = HashInsert(key, hashTable);
            if (comparison < 0)
                printf("Duplicate key\n");
            else if (comparison < TABLESIZE)
                printf("Insert: %d Key Comparisons: %d\n", key, comparison);
            else
                printf("Key Comparisons: %d. Table is full.\n", comparison);
            break;
        case 2:
            printf("Enter a key to be deleted:\n");
            scanf("%d", &key);
            comparison = HashDelete(key, hashTable);
            if (comparison < 0)
                printf("%d does not exist.\n", key);
            else if (comparison <= TABLESIZE)
                printf("Delete: %d Key Comparisons: %d\n", key, comparison);
            else
                printf("Error\n");
            break;
        case 3:
            for (i = 0; i < TABLESIZE; i++)
                printf("%d: %d %c\n", i, hashTable[i].key, hashTable[i].indicator == DELETED ? '*' : ' ');
            break;
        }
        printf("Enter selection: ");
        scanf("%d", &opt);
    }
    return 0;
}

int hash1(int key)
{
    return (key % TABLESIZE);
}

int hash2(int key)
{
    return (key % PRIME) + 1;
}

int HashInsert(int key, HashSlot hashTable[])
{
    //Write your code here
    int index;
    int hashes = 0;
    int comparisons = 0;

    HashSlot *deletedAvail = NULL;

    while (comparisons < TABLESIZE)
    {
        index = hash1(key + hashes * hash2(key));
        HashSlot *slot = &hashTable[index];

        switch (slot->indicator)
        {
            case DELETED:
                //If deletedAvail is not null, do not change the value.
                //Otherwise set it as the current slot
                deletedAvail = deletedAvail ? deletedAvail : slot;
                break;
            case USED:
                comparisons++;
                if(slot->key == key)
                    return -1;
                break;
            case EMPTY:
                if(deletedAvail){
                    deletedAvail->key = key;
                    deletedAvail->indicator = USED;
                    return comparisons;
                }else{
                    slot->key = key;
                    slot->indicator = USED;
                    return comparisons;
                }
                break;
        }
        //Increment hashes for the table
        hashes++;
    }
    //Table is supposedly "full", check if there is a deleted slot
    if(deletedAvail){
        deletedAvail->key = key;
        deletedAvail->indicator = USED;
        return comparisons-1;
    }
    //Table is full
    return comparisons;
}

int HashDelete(int key, HashSlot hashTable[])
{
    //Write your code here
    int index;
    int hashes = 0;
    int comparisons = 0;

    while(comparisons<TABLESIZE){
        index = hash1(key + hashes*hash2(key));
        HashSlot *slot = &hashTable[index];

        if(slot->indicator == USED){
            comparisons++;
            if(slot->key == key){
                slot->indicator = DELETED;
                return comparisons;
            }
        }else if(slot->indicator == EMPTY){
            //DOES NOT EXIST
            return -1;
        }
        hashes++;
    }
    return -1;
}
