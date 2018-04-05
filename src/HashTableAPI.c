#include "HashTableAPI.h"

HTable *createTable(size_t size, int (*hashFunction)(size_t tableSize, char * key),void (*destroyData)(void *data),void (*printData)(void *toBePrinted))
{

    int i;
    HTable * temp = malloc(sizeof(HTable));

    if(temp == NULL)
    {
        return NULL;
    }

    temp->table = malloc(sizeof(*temp->table) * size);

    for(i = 0; i < size; i++)
    {
        temp->table[i] = NULL;
    }

    temp->size = size;
    temp->destroyData = destroyData;
    temp->hashFunction = hashFunction;
    temp->printData = printData;

    return temp;

}

Node *createNode(char * key, void *data)
{

    Node * temp = malloc(sizeof(Node));
    temp->key = malloc(sizeof(key)*2);
    temp->data = malloc(sizeof(data)*2);

    if(temp == NULL)
    {
        return NULL;
    }


    strcpy(temp->key, key);
    strcpy(temp->data, data);
    temp->next = NULL;

    return temp;
}

void destroyTable(HTable *hashTable)
{

    Node * temp = NULL;

    while(temp != NULL)
    {
        removeData(hashTable, temp->key);
        temp = temp->next;
    }

    free(hashTable);

}

void insertData(HTable *hashTable, char * key, void *data)
{

    if(hashTable != NULL)
    {
        int count = 0;
        count = hashTable->hashFunction(hashTable->size, key);
        Node * temp = createNode(key, data);
        temp->next = hashTable->table[count];
        hashTable->table[count] = temp;
    }
}


void removeData(HTable *hashTable, char * key)
{

    if (hashTable != NULL)
    {
        int tempKey = 0;
        tempKey = hashTable->hashFunction(hashTable->size, key);
        Node * temp = hashTable->table[tempKey];

        while(temp != NULL)
        {
            if(strcmp(temp->key, key) == 0) 
            {
                if(temp->next != NULL)
                {
                    temp->next = temp->next->next;
                }

                free(temp);
                hashTable->table[tempKey] = NULL;
                break;
            }

            temp = temp->next;
        }
    }
}

void *lookupData(HTable *hashTable, char * key)
{
    if (hashTable == NULL)
    {
        return NULL;
    }

    int tempKey = 0;
    
    tempKey = hashTable->hashFunction(hashTable->size, key);
    Node * temp = hashTable->table[tempKey];

    if(temp == NULL || key == NULL || temp->data == NULL)
    {

        return NULL;
    }

    while(temp != NULL)
    {

        if(strcmp(temp->key, key) == 0)
        {
            return temp->data;
            break;
        }

        temp = temp->next;

    }

    return NULL;
}

