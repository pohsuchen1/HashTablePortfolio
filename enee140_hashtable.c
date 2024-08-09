/*
 * enee140_hashtable.h
 *
 *  Header file for a hash table library.
 */
#ifndef ENEE140_HASHTABLE_H_
#define ENEE140_HASHTABLE_H_

#include <stdio.h>
/*
 * Define the parameters of a hashtable that will store English words.
 * Provision the capacity of the hashtable considering the fact that
 * Shakespeare's works include about 30,000 unique words and that the
 * lookup performance tends to decrease with a load factor > 0.7.
 */
#define NBUCKETS        50021   // prime number, for better hash uniformity
#define MAX_STRING      80
/* Function prototypes for the public hashtable API. */
void hash_initialize();
float hash_load_factor();
int hash_put(char key[], int value);
int hash_get(char key[]);
int hash_lookup(char key[]);
int hash_probe(char key[]);
int hash_max_collision_chain();
float hash_avg_collision_chain();
int hash_remove(char key[]);
#endif /* ENEE140_HASHTABLE_H_ */

/*
 * enee140_hashtable.c
 *
 *  Implementation of the hash table operations.
 */
#include "enee140_hashtable.h"
#include <string.h>
/*
 * Define the bucket and hashtable data types.
 */
typedef struct _bucket
{
    char key[MAX_STRING];
    int value;
}
Bucket;
typedef Bucket Hashtable[NBUCKETS];
/*
 * Hashtable main storage
 */
static Hashtable my_hash_table;
static int used_buckets = 0;
static int max_collision = 0;
static int avg_collision = 0;
static int collisions = 0;
/*
 * Internal library functions
 */
unsigned hash_function(char s[]) {
    unsigned hashval, i;
    for (hashval = 0, i = 0; s[i] != '\0'; i++) {
        hashval += s[i] + 31 * hashval;
    }
    return hashval % NBUCKETS;

}
/*
 * Main hashtable API
 */
float hash_load_factor()
{
    return (float)used_buckets/NBUCKETS;
}
/*
 * Initialize the hash table by clearing its content.
 */
void hash_initialize()
{
    int i;
    for (i = 0; i < NBUCKETS; i++)
    {
        strcpy(my_hash_table[i].key, ""); //Sets each key to empty
        my_hash_table[i].value = 0; //Sets each value to zero
    }
    used_buckets = 0; //Sets used buckets to zero
}
/*
 * Insert a <key, value> pair in the hash table.
 * Return 1 if the insert was successful, and 0 if
 * the key could not be inserted because the hash
 * table if full.
 */
int hash_put(char key[], int value)
{
    unsigned int index = hash_function(key); //gets initial index to place word in
    int loop = 0;
    int collision_length = 0;

    if(strcmp(key, my_hash_table[index].key) == 0)
    {
        my_hash_table[index].value++;
        return 1;
    }

    while(strncmp(my_hash_table[index].key, "", 1) != 0) // if index is not empty it increments the index by 1
    {
        collision_length++;
        index++;
        if(strcmp(key, my_hash_table[index].key) == 0)
        {
            my_hash_table[index].value++;
            return 1;
        }
        if(index == NBUCKETS) //if index exceeds the NBUCKETS it resets to zero
        {
            index = 0;
            loop++;
            if(loop == 2) //If loop is 2 that means it has gone through the loop fully without finding an empty bucket and returns zero
            {
                return 0;
            }
        }
    }
    strcpy(my_hash_table[index].key, key); //Stores key into hash table
    my_hash_table[index].value = value; //Stores value into hash table

    if(collision_length > max_collision)
    {
        max_collision = collision_length;
    }
    if(collision_length > 0)
    {
        avg_collision = avg_collision + collision_length;
        collisions++;
    }
    used_buckets++; //Increments used buckets.
    return 1; //shows has hashtable has been succesfully updated
}
/*
 * If key is stored in the hashtable, return the corresponding
 * value. Otherwise, return 0.
 */
int hash_get(char key[])
{
    if(hash_lookup(key) == 1) //if key is in the hashtable
    {
        int a = my_hash_table[hash_probe(key)].value; //finds value in bucket
        return a;
    }
    else
    {
        return 0;
    }
}
/*
 * Returns 1 if key is stored in the hashtable, and 0 otherwise.
 */
int hash_lookup(char key[])
{
    unsigned int index = hash_function(key);

    if(strcmp(key, my_hash_table[index].key) == 0) //if the two strings are equal
    {
        return 1;
    }
    else
    {
        while(strcmp(key, my_hash_table[index].key) != 0) //if strings are not equal
        {
            index++; //increases index by 1
            if(strncmp(my_hash_table[index].key, "", 1) == 0) //when they key is not in the hash table
            {
                return 0;
            }
            if(index == NBUCKETS + 1) //starts at the begining of the hashtable
            {
                index = 0;
            }
        }
        return 1;
    }
}
int hash_probe(char key[]) //goes through the hashtable to see if key is in hashtable
{
    unsigned int index = hash_function(key);
    int loop = 0;
    while(strcmp(key, my_hash_table[index].key) != 0)
    {
        index++;
        if(index == NBUCKETS + 1)
        {
            index = 0;
            loop++;
        }
        if(loop == 2)
        {
            return 0;
        }

    }
    return index;

}

int hash_max_collision_chain()
{
    return max_collision;
}
float hash_avg_collision_chain()
{
    return (float)avg_collision/collisions;
}
int hash_remove(char key[])
{
    unsigned int index = hash_function(key);
    int removed = 0;

    if(strcmp(key, my_hash_table[index].key) == 0)
    {
        strcpy(my_hash_table[index].key, "");
        my_hash_table[index].value = 0;
        used_buckets--;
        removed++;
    }
    else
    {
        int indexp = hash_probe(key);
        if(indexp > 0)
        {
            strcpy(my_hash_table[indexp].key, "");
            my_hash_table[indexp].value = 0;
            removed++;
            used_buckets--;
            index = indexp;
        }
        else
        {
            return 0;
        }
    }

    if(removed > 0)
    {
        while (strcmp(my_hash_table[index + 1].key, "") != 0) {
            index++;
            int value = my_hash_table[index].value;
            char key[MAX_STRING];
            stpcpy(key, my_hash_table[index].key);
            hash_remove(key);
            hash_put(key, value);
        }
    }
    else
    {
        return 0;
    }
    return 1;
}