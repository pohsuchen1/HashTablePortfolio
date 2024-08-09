/*
 * enee140_lookup.c
 *
 *  Read the input line‐by‐line, store the words in a
 *  hash table, and allow the user to look up several words.
 */
#include "enee140_hashtable.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#define MAX_LINE 1000

int next_word(const char line[], char word[], int size);
int prompt_and_check(int max_option);
void lowercase_string(char word[]);

int main(int argc, char *argv[]) {
    char word[MAX_STRING], line[MAX_LINE];
    FILE *file;
    // Check command‐line arguments
    if (argc < 2) {
        fprintf(stderr, "Usage: %s filename \n", argv[0]);
        return -1;
    }

    file = fopen(argv[1], "r");
    if (file == NULL) {
        printf("Could not open the %s file.\n", argv[1]);
        exit(-1);
    }
    // Initialize the hashtable
    hash_initialize();

    // Read file line‐by‐line
    while (fgets(line, MAX_LINE, file) != NULL) {
        // Extract each word from line and add it to the hash table
        while (next_word(line, word, MAX_STRING) != 0) {

            lowercase_string(word);

            if (hash_lookup(word) == 0) //if word is not present in hashtable
            {
                hash_put(word, 1); //Places word in hashtable with value of 1
            }
            else
            {
                int a = hash_get(word) + 1;
                hash_put(word, a); //Increments value of word in hashtable by 1
            }
        }
    }
    // Print menu and implement project functionality
    int choice = 0;
    while (choice != 3)
    {
        printf("Welcome to the ENEE140 word lookup program!\n"
               "   1. Print hash table statistics\n"
               "   2. Look up a word\n"
               "   3. Exit\n"
               "   4. Remove a word\n");
        printf("Enter your choice: (1-4):\n");
        scanf("%d", &choice); //Stores user chouce in variable choice

        if (choice < 1 || choice > 4) {
            printf("Sorry that is not a valid option \n Enter your choice (1-4):\n"); // reprompt user for choice
            continue; //resets the next iteration of the loop
        }


        if(choice == 4)
        {
            printf("Enter a word:\n");
            char inputr[100];
            scanf("%99s", inputr); //Takes user input and stores it
            lowercase_string(inputr);

            if (hash_lookup(inputr) == 1) //checks if the input is in the hashtable
            {
                if(hash_remove(inputr) == 1)
                {
                    printf("Word was removed successfully\n");
                }
                else
                {
                    printf("word was not removed\n");
                }
            }
            else
            {
                printf("Word is not in hashtable\n");
            }
        }
        if (choice == 1) {
            printf("Load factor: %.3f\n", hash_load_factor()); //prints load_factor
            printf("Max collision length: %d\n", hash_max_collision_chain());
            printf("Average collision length: %.3f\n", hash_avg_collision_chain());
        }
        if (choice == 2)
        {
            printf("Enter a word:\n");
            char input[100];
            scanf("%99s", input); //Takes user input and stores it

            lowercase_string(input);
            if (hash_lookup(input) == 1) //checks if the input is in the hashtable
            {
                printf("This word has occured %d times\n", hash_get(input));
            }
            else //word is not in the hashtable
            {
                printf("The word was not found\n");
            }
        }
    }

    if(choice == 3)
    {
        return 0; //exits program if input is 3
    }

    return 0;
}

void lowercase_string(char word[])
{
    int i;
    for(i=0; i < sizeof(word); i++) //cycles through entire string
    {
        if(word[i] >= 'A' && word[i] <= 'Z') // checks if each letter is uppercase
        {
            word[i] = word[i] + 32; //changes Uppercase letter to lowercase
        }
    }
}

int next_word(const char line[], char word[], int size)
{
    static int position = 0;
    int word_position = 0;

    while(line[position] != '\0') //when character is not at the end of the line
    {

        char letter = line[position];

        if(!isspace(letter) && !ispunct(letter)) //checks if letter is punctuation or a space
        {
            word[word_position] = letter; //sets word character to the letter in the line
            position++;
            word_position++;

            // Check if the word has reached the specified size
            if (word_position == size - 1)
            {
                word[word_position] = '\0';
                return 1;
            }
        }
        else
        {
            if(word_position > 0) //if word has characters in it
            {
                word[word_position] = '\0';
                return 1; //shows that a word has been entered
            }
            else
            {
                position++; //moves along the line
            }
        }
    }
    position = 0; //resets the position of the string at the start of a new line
    return 0;
}