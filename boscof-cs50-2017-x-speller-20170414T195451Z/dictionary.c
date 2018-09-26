/**
 * Implements a dictionary's functionality.
 */

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <math.h>
#include <stdbool.h>

#include "dictionary.h"

//define the size of the hashtable
#define BUCKETS 100000

/**
 * Returns true if word is in dictionary else false.
 */
 
 // declare a linked list
 typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

//declare a hashtable
node* hashtable[BUCKETS]={NULL};

//declaring a new variable to count the number of words in the dictionary
int count=0;

//Shift-Add-XOR hash
unsigned int hash_function(const char *word)
{
    unsigned int h = 0;
    for (int i=0, n=strlen(word);i<n;i++)
    {
        h ^= (h << 5) + (h >> 2) + word[i];
        h = h % BUCKETS;
    }
    return h;
}

bool check(const char *word)
{
    //store temporary word
    char word_decap[LENGTH+1];
    
    //declare length of string
    int j=strlen(word);
    
    //standardise the word's capitalisation so that each letter is lowercase
    for (int i=0;i<j;i++)
    {
        if (isalpha(word[i]))
        {
            word_decap[i]=tolower(word[i]);
        }
        else
        {
            word_decap[i]=word[i];
        }
    }
    word_decap[j]='\0';
    
    //find the index the word should belong to within the array
    unsigned int index = hash_function(word_decap);
    
    //if the index is empty, return false
    if (hashtable[index] ==NULL)
    {
        return false;
    }
    
    //otherwise declare a pointer to the head of the linked list
    node *cursor = hashtable[index];
    
    //search the linked list to see if the word is in the loaded dictionary
    while ( cursor != NULL)
    {
        //check if word matches with a word contained within the dictionary
        if (strcasecmp(cursor->word, word_decap)==0)
        {
            return true;
        }
        //go to the next value in the link
        else
        {
            cursor = cursor->next;
        }
    }
    //if word does not exist within the loaded dictionary return false
    return false;
}

/**
 * Loads dictionary into memory. Returns true if successful else false.
 */
bool load(const char *dictionary)
{
    // open the dictionary file to read
    FILE* file = fopen(dictionary, "r");
    if (file == NULL)
    {
        printf("Could not open %s\n", dictionary);
        return false;
    }
    
    // declaring a new array to store the words in
    char word[LENGTH + 1];
    
    while (fscanf(file, "%s\n", word) != EOF)
    {
        //counting the numbers of words in the dictionary
        count++;
        
        //allocate some memory for the new node
        node *new_node = malloc(sizeof(node));
        
        //copy the string into the predefined array
        strcpy(new_node->word, word);
        
        //run each word through the hash function to obtain an indexed value
        unsigned int index = hash_function(word);
        
        //link the new node with any existing node(s) and expand the linked list
        new_node->next = hashtable[index];
        hashtable[index] = new_node;
    }
    
    //close the file
    fclose(file);
        
    //return true upon success
    return true;
}

/**
 * Returns number of words in dictionary if loaded else 0 if not yet loaded.
 */
unsigned int size(void)
{
    // returns the number of words that are contained within the loaded dictionary
    if (count>0)
    {
        return count;
    }
    // returns 0 if the dictionary is either empty or unloaded
    else
    {
        return 0;
    }
}

/**
 * Unloads dictionary from memory. Returns true if successful else false.
 */
bool unload(void)
{
    //loop through the hashtable
    for (int index=0;index < BUCKETS;index++)
    {
        //free any linked lists that might exist
        while (hashtable[index] != NULL)
        {
            node *cursor=hashtable[index];
            hashtable[index] = cursor->next;
            free(cursor);
        }
    }
    //if successful, return true
    return true;
}
