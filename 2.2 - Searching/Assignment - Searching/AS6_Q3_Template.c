#include <stdio.h>
#include <stdlib.h>

#define TABLESIZE 5
#define PRIME 3

enum Marker
{
    EMPTY,
    USED
};

typedef struct _slot
{
    int key;
    enum Marker indicator;
    int next;
} HashSlot;

int HashInsert(int key, HashSlot hashTable[]);
int HashFind(int key, HashSlot hashTable[]);

int hash(int key)
{
    return (key % TABLESIZE);
}

int main()
{
    int opt;
    int i;
    int key;
    int index;
    HashSlot hashTable[TABLESIZE];

    for (i = 0; i < TABLESIZE; i++)
    {
        hashTable[i].next = -1;
        hashTable[i].key = 0;
        hashTable[i].indicator = EMPTY;
    }

    printf("============= Hash Table ============\n");
    printf("|1. Insert a key to the hash table  |\n");
    printf("|2. Search a key in the hash table  |\n");
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
            index = HashInsert(key, hashTable);
            if (index < 0)
                printf("Duplicate key\n");
            else if (index < TABLESIZE)
                printf("Insert %d at index %d\n", key, index);
            else
                printf("Table is full.\n");
            break;
        case 2:
            printf("Enter a key for searching in the HashTable:\n");
            scanf("%d", &key);
            index = HashFind(key, hashTable);

            if (index != -1)
                printf("%d is found at index %d.\n", key, index);
            else
                printf("%d is not found.\n", key);
            break;

        case 3:
            printf("index:\t key \t next\n");
            for (i = 0; i < TABLESIZE; i++)
                printf("%d\t%d\t%d\n", i, hashTable[i].key, hashTable[i].next);
            break;
        }
        printf("Enter selection: ");
        scanf("%d", &opt);
    }
    return 0;
}

int HashInsert(int key, HashSlot hashTable[])
{
    int index = hash(key);
    HashSlot *slot = &hashTable[index];

    if(slot->indicator == EMPTY){
        //1st hash slot is empty, just insert
        slot->indicator = USED;
        slot->key = key;
        return index;
    }

    //Preliminary check for duplicate
    if(slot->key == key)
            return -1;

    //Otherwise, occupied. Find the last slot in the chain
    int lastIndex = index;
    while(slot->next != -1){
        lastIndex = slot->next;
        slot=&hashTable[slot->next];
        //Check for duplicate
        if(slot->key == key)
            return -1;
    }

    //Find next available slot
    int i;
    int newIndex = -1;
    for(i = 1; i<TABLESIZE;i++){
        int effectiveIndex = lastIndex+i;
        if(effectiveIndex >= TABLESIZE){
            effectiveIndex -= TABLESIZE;
        }
        if(hashTable[effectiveIndex].indicator == EMPTY){
            newIndex = effectiveIndex;
            break;
        }
    }

    if(newIndex >= 0){
        hashTable[newIndex].key = key;
        hashTable[newIndex].indicator = USED;
        hashTable[lastIndex].next = newIndex;
        return newIndex;
    }else{
        return TABLESIZE;
    }
    
}

int HashFind(int key, HashSlot hashTable[])
{
    // Write your code here
    int index = hash(key);

    HashSlot *slot;
    do{
        slot = &hashTable[index];
        if(slot->indicator == EMPTY){
            //Key not found
            return -1;
        }else if(slot->key == key){
            return index;
        }

        index = slot->next;
    }while (index != -1);
    return -1;
}
