/*
 * enee140_hashtable.h
 *
 *  Header file for a hash table library.
 */
#ifndef ENEE140_HASHTABLE_H_
#define ENEE140_HASHTABLE_H_
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