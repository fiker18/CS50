// Implements a dictionary's functionality

#include <ctype.h>
#include <stdbool.h>
#include <cs50.h>
#include <strings.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
} node;

// TODO: Choose number of buckets in hash table
const unsigned int N = 16276;

// Hash table
node *table[N];
unsigned int count = 0;

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    int hash_value = hash(word);

    if(table[hash_value] == NULL)
    {
        return false;
    }

    for(node *ptr = table[hash_value]; ptr != NULL; ptr = ptr->next)
    {
        if(strcasecmp(ptr->word, word) == 0)
        {
            return true;
        }
    }
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    if (word[1] != '\0')
    {
        if(word[2] != '\0')
        {
            return ((625 * (toupper(word[0]) - 'A'))+ (25 * (toupper(word[1]) - 'A')) + (toupper(word[2]) - 'A'));
        }
        return ((625 * (toupper(word[0]) - 'A'))+ (25 * (toupper(word[1]) - 'A')));
    }
    return ((625 * (toupper(word[0]) - 'A')));
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{

    FILE *dic = fopen(dictionary, "r");
    if( dic == NULL)
    {
        return false;
    }

    char word[LENGTH + 1] = {'\0'};
    while(fscanf(dic, "%s", word) != EOF)
    {
        node *n = malloc(sizeof(node));
        if( n == NULL)
        {
            return false;
        }
        strcpy(n->word, word);
        n->next = NULL;

        int hash_value = hash(word);
        if (table[hash_value] == NULL)
        {
            table[hash_value] = n;
            count++;
        }
        else
        {
            n->next = table[hash_value];
            table[hash_value] = n;
            count++;
        }
    }
    fclose(dic);
    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    return count;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    for(int i = 0; i < N - 1; i++)
    {
        if(table[i] != NULL)
        {
            for(node *ptr = table[i], *tmp = table[i]; ptr != NULL; ptr = tmp)
            {
                tmp = ptr->next;
                free(ptr);
            }
        }
    }
    return true;
}
