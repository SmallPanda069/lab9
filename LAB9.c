#include <stdio.h>
#include <stdlib.h>

// RecordType
struct RecordType
{
    int id;
    char name;
    int order;
};

// Fill out this structure
struct HashType
{
    struct RecordType record;
    struct HashType *next;
};

// Hash Table
struct HashTable
{
    int size;
    struct HashType **array;
};

// Create a new Hash Table
struct HashTable *createHashTable(int size)
{
    struct HashTable *hashTable = (struct HashTable *)malloc(sizeof(struct HashTable));
    if (hashTable == NULL)
    {
        printf("Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    hashTable->size = size;
    hashTable->array = (struct HashType **)malloc(sizeof(struct HashType *) * size);

    if (hashTable->array == NULL)
    {
        printf("Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < size; i++)
    {
        hashTable->array[i] = NULL;
    }

    return hashTable;
}

// Compute the hash function
int hash(int x, int size)
{
    return x % size;
}

// Insert a record into the hash table
void insert(struct HashTable *hashTable, struct RecordType record)
{
    int index = hash(record.id, hashTable->size);
    struct HashType *newNode = (struct HashType *)malloc(sizeof(struct HashType));
    if (newNode == NULL)
    {
        printf("Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    newNode->record = record;
    newNode->next = NULL;

    // If the slot is empty, insert the node
    if (hashTable->array[index] == NULL)
    {
        hashTable->array[index] = newNode;
    }
    else
    {
        // Collision handling: Append the new node at the end of the linked list
        struct HashType *tmp = hashTable->array[index];
        while (tmp->next != NULL)
        {
            tmp = tmp->next;
        }
        tmp->next = newNode;
    }
}

// Display records in the hash structure
void displayRecordsInHash(struct HashTable *hashTable)
{
    printf("\nRecords:\n");
    for (int i = 0; i < hashTable->size; ++i)
    {
        struct HashType *temp = hashTable->array[i];
        printf("Index %d -> ", i);
        while (temp != NULL)
        {
            printf("%d %c %d -> ", temp->record.id, temp->record.name, temp->record.order);
            temp = temp->next;
        }
        printf("NULL\n");
    }
    printf("\n");
}

// Parses input file to an integer array
int parseData(char *inputFileName, struct RecordType **ppData)
{
    FILE *inFile = fopen(inputFileName, "r");
    int dataSz = 0;
    int i, n;
    char c;
    struct RecordType *pRecord;
    *ppData = NULL;

    if (inFile)
    {
        fscanf(inFile, "%d\n", &dataSz);
        *ppData = (struct RecordType *)malloc(sizeof(struct RecordType) * dataSz);

        if (*ppData == NULL)
        {
            printf("Cannot allocate memory\n");
            exit(EXIT_FAILURE);
        }
        for (i = 0; i < dataSz; ++i)
        {
            pRecord = *ppData + i;
            fscanf(inFile, "%d ", &n);
            pRecord->id = n;
            fscanf(inFile, "%c ", &c);
            pRecord->name = c;
            fscanf(inFile, "%d ", &n);
            pRecord->order = n;
        }

        fclose(inFile);
    }

    return dataSz;
}

// Print the records
void printRecords(struct RecordType pData[], int dataSz)
{
    printf("\nRecords:\n");
    for (int i = 0; i < dataSz; ++i)
    {
        printf("\t%d %c %d\n", pData[i].id, pData[i].name, pData[i].order);
    }
    printf("\n\n");
}

int main(void)
{
    struct RecordType *pRecords;
    int recordSz = 0;

    recordSz = parseData("input.txt", &pRecords);
    printRecords(pRecords, recordSz);

    struct HashTable *hashTable = createHashTable(recordSz * 2); // Creating a hash table with size based on the number of records

    for (int i = 0; i < recordSz; i++)
    {
        insert(hashTable, pRecords[i]);
    }

    displayRecordsInHash(hashTable);

    // Free memory
    for (int i = 0; i < hashTable->size; i++)
    {
        struct HashType *current = hashTable->array[i];
        while (current != NULL)
        {
            struct HashType *temp = current;
            current = current->next;
            free(temp);
        }
    }
    free(hashTable->array);
    free(hashTable);
    free(pRecords);

    return 0;
}