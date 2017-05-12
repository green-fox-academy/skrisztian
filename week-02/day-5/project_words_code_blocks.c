#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

/* Checks if two words
 * are a palindrome of each other
 */
int check_palindrome(void) {

    char word1[255];
    char word2[255];
    int is_anagram = 0;
    char letter;
    int letter_count = 0;

    // Read in two words
    puts("Enter first word:");
    gets(word1);
    puts("Enter second word:");
    gets(word2);

    // Count the number of letters in word1
    letter_count = count_letters(word1);

    // Compare the two words
    for (int i=0; i<letter_count; i++) {
        if (word1[i] == word2[letter_count-(i+1)]) {
            is_anagram = 1;
        } else {
            is_anagram = 0;
            break;
        }
    }

    // Print test result
    if (is_anagram) {
        puts("true");
    } else {
        puts("false");
    }

    return 0;
}

/* Creates a palindrome
 * from a word
 */
int palindrome(void) {

    char word[255];
    char p_word[255];
    int letter_count = 0;

    // Read in the word
    puts("Enter word:");
    gets(word);

    // Count the number of letters in the word
    while (word[letter_count] != 0) {
        letter_count++;
    }

    // Print word
    printf("%s", word);

    //Print reversed word
    for (int i=letter_count-1; i>=0; i--) {
        printf("%c", word[i]);
    }
    printf("\n");
    return 0;
}

/* Counts the number of letters
 * in a word
 */
int count_letters(char word_to_count[]) {

    int count = 0;

    while (word_to_count[count] != 0) {
        count++;
    }

    return count;
}

/* Puts the letters in a word
 * into alphabetical order
 */
void sort_word(char* word_to_sort) {

    int letter_count = count_letters(word_to_sort);
    int swapped = 1;

    while (swapped == 1) {
        swapped = 0;
        for(int i=0; i<letter_count-1; i++) {
            char letter;
            // Sort two adjacent letters
            if (word_to_sort[i] > word_to_sort[i+1]) {
                letter = word_to_sort[i];
                word_to_sort[i] = word_to_sort[i+1];
                word_to_sort[i+1] = letter;
                swapped = 1;
            }
        }
    }
}


int anagram(void) {

    char word1[255];
    char word2[255];
    int is_anagram = 0;

    // Read in two words
    puts("Enter first word:");
    gets(word1);
    puts("Enter second word:");
    gets(word2);

    // Count the number of letters in the words
    int letter_count_1 = count_letters(word1);
    int letter_count_2 = count_letters(word2);

    // Only look for anagram if the two words
    // have the same length
    if (letter_count_1 == letter_count_2) {

        // Sort words
        sort_word(word1);
        sort_word(word2);

        // Compare words
        for (int i=0; i<letter_count_1; i++) {
            if (word1[i] == word2[i]) {
                is_anagram = 1;
            } else {
                is_anagram = 0;
                break;
            }
        }
    }

    // Print result
    if (is_anagram) {
        puts("true");
    } else {
        puts("false");
    }
}


int main()
{
    char command[255];

    while(1) {

        puts("Command options:");
        puts(" -anagram");
        puts(" -palindrome");
        puts(" -check_palindrome");
        puts(" -exit");
        puts("");
        puts("Enter command:");
        gets(command);
        puts("");

        if (strcmp(command, "-anagram") == 0) {
            anagram();
            puts("");
        } else if (strcmp(command, "-palindrome") == 0) {
            palindrome();
            puts("");
        } else if (strcmp(command, "-check_palindrome") == 0) {
            check_palindrome();
            puts("");
        } else if (strcmp(command, "-exit") == 0) {
            break;
        }
    }
    return 0;
}
