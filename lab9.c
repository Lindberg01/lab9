/*
COPYRIGHT Lindberg Gay
COP 3502C   4/11/2024
*/

#include <stdio.h>
#include <stdlib.h>

#define HASH_SIZE 23 // Defining your hash table size


struct RecordType
{
    int     id;
    char    name;
    int     order; 
};

// Defining a node for linked list
struct Node
{
    struct RecordType record;
    struct Node* next;
};

// Defining the hash table array
struct HashType
{
    struct Node* head; // Pointer to the head of the linked list
};

// Compute the hash function
int hash(int x)
{
    return x % HASH_SIZE; // Basic modulo hashing for simplicity
}

// Function to insert a record into the hash table
void insertRecord(struct HashType* hashTable, struct RecordType record)
{
    // Calculate the index using the hash function
    int index = hash(record.id); 

    // Create a new node
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    if (newNode == NULL)
    {
        printf("Memory allocation failed.\n");
        exit(EXIT_FAILURE);
    }
    newNode->record = record;
    newNode->next = NULL;

    // If the slot is empty, insert as the head of the list
    if (hashTable[index].head == NULL)
    {
        hashTable[index].head = newNode;
    }
    else
    {
        // Otherwise, insert at the beginning of the list
        newNode->next = hashTable[index].head;
        hashTable[index].head = newNode;
    }
}

// Function to display records in the hash structure
void displayRecordsInHash(struct HashType *hashTable, int hashSz)
{
    printf("Records in the hash table:\n");
    for (int i = 0; i < hashSz; ++i)
    {
        if (hashTable[i].head != NULL) // Skip empty indices
        {
            printf("Index %d -> ", i);
            struct Node* current = hashTable[i].head;
            while (current != NULL)
            {
                printf("%d, %c, %d", current->record.id, current->record.name, current->record.order);
                if (current->next != NULL) {
                    printf(" -> ");
                } else {
                    printf("\n");
                }
                current = current->next;
            }
        }
    }
}

// Function to parse input file to an integer array of records
int parseData(char* inputFileName, struct RecordType** ppData)
{
    FILE* inFile = fopen(inputFileName, "r");
    int dataSz = 0;
    int i, n;
    char c;
    struct RecordType *pRecord;
    *ppData = NULL;

    if (inFile)
    {
        fscanf(inFile, "%d\n", &dataSz);
        *ppData = (struct RecordType*)malloc(sizeof(struct RecordType) * dataSz);
        if (*ppData == NULL)
        {
            printf("Cannot allocate memory\n");
            exit(-1);
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

int main(void)
{
    struct RecordType *pRecords;
    int recordSz = 0;

    recordSz = parseData("input.txt", &pRecords);

    // Initialize the hash table array
    struct HashType hashTable[HASH_SIZE] = {NULL};

    // Insert records into the hash table
    for (int i = 0; i < recordSz; ++i)
    {
        insertRecord(hashTable, pRecords[i]);
    }

    // Display records in the hash table
    displayRecordsInHash(hashTable, HASH_SIZE);

    // Free dynamically allocated memory
    for (int i = 0; i < HASH_SIZE; ++i)
    {
        struct Node* current = hashTable[i].head;
        while (current != NULL)
        {
            struct Node* temp = current;
            current = current->next;
            free(temp);
        }
    }

    free(pRecords);

    return 0;
}
