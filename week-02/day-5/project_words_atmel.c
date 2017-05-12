#include <avr/io.h>			// This header contains the definitions for the io registers
#include <stdint.h>
#include <stdio.h>
#define F_CPU	16000000	// This definition tells to _delay_ms() that the CPU runs at 16MHz
#include <util/delay.h>		// This header contains the _delay_ms() function
#include <string.h>

void check_palindrome(void);
void palindrome(void);
void anagram(void);

void UART_Init()
{
	// Set the baud rate to 9600
	// The CPU clock frequency is 16MHz
	UBRR0H = 103 >> 8;
	UBRR0L = 103;

	// Frame format: 8N1

	// Enable TX and RX
	UCSR0B |= (1<<TXEN0);
	UCSR0B |= (1<<RXEN0);
}

void LED_init() {
	// Set PORTB5 to output
	DDRB |= 1 << DDRB5;
}

void UART_SendCharacter(char character)
{
	// Wait for empty buffer
	while (!( UCSR0A & (1<<UDRE0)));

	// Put data to buffer
	UDR0 = character;
	PORTB |= 1 << PORTB5;
	_delay_ms(2);
	PORTB &= ~(1 << PORTB5);
}

char UART_GetCharacter()
{
	// Wait for data received
	while (!(UCSR0A & (1<<RXC0)));
	
	// Get data from buffer
	return UDR0;
	PORTB |= 1 << PORTB5;
	_delay_ms(2);
	PORTB &= ~(1 << PORTB5);
}

/* Counts the number of letters
 * in a word
 */
uint8_t count_letters(char word_to_count[]) {

    uint8_t count = 0;

    while (word_to_count[count] != 0) {
        count++;
    }

    return count;
}

/* Puts the letters in a word
 * into alphabetical order
 */
void sort_word(char* word_to_sort) {

    uint8_t letter_count = count_letters(word_to_sort);
    uint8_t swapped = 1;

    while (swapped == 1) {
        swapped = 0;
        for(uint8_t i=0; i<letter_count-1; i++) {
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




int main(void)
{
	char buffer[255];

	//Init
	UART_Init();
	LED_init();

	// Setting up STDIO input and output buffer
	//----- START OF STDIO IO BUFFER SETUP
	FILE UART_output = FDEV_SETUP_STREAM(UART_SendCharacter, NULL, _FDEV_SETUP_WRITE);
	stdout = &UART_output;
	FILE UART_input = FDEV_SETUP_STREAM(NULL, UART_GetCharacter, _FDEV_SETUP_READ);
	stdin = &UART_input;
	//----- END OF STDIO IO BUFFER SETUP

	// wait for an enter -- Linux
	gets(buffer);

	// Loop that runs forever
	while (1) {

		// Choose game type
	    puts("Command options:");
	    puts(" -anagram");
	    puts(" -palindrome");
	    puts(" -check_palindrome");
	    puts(" -exit");
	    puts("");
	    puts("Enter command:");
	    gets(buffer);
	    puts("");

 	    if (strcmp(buffer, "-anagram") == 0) {
 			anagram();
			puts("");
		} else if (strcmp(buffer, "-palindrome") == 0) {
			palindrome();
			puts("");
		} else if (strcmp(buffer, "-check_palindrome") == 0) {
		    check_palindrome();
		    puts("");
		} else if (strcmp(buffer, "-exit") == 0) {
		    break;
	    }
   }
   return 0;
}

/* Checks if two words
 * are a palindrome of each other
 */
void check_palindrome(void) {

    char word1[255];
    char word2[255];
    uint8_t is_anagram = 0;
    uint8_t letter_count = 0;

    // Read in two words
    puts("Enter first word:");
    gets(word1);
	puts(word1);
    puts("Enter second word:");
    gets(word2);
    puts(word2);

    // Count the number of letters in word1
    letter_count = count_letters(word1);

    // Compare the two words
    for (uint8_t i=0; i<letter_count; i++) {
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
}

/* Creates a palindrome
 * from a word
 */
void palindrome(void) {

    char word[255];
    int letter_count = 0;

    // Read in the word
    puts("Enter word:");
    gets(word);
	puts(word);

    // Count the number of letters in the word
    letter_count = count_letters(word);

    // Print word
    printf("%s", word);
	
    //Print reversed word
    for (int i=letter_count-1; i>=0; i--) {
	    printf("%c", word[i]);
    }
	printf("\n");
}

/* Checks if two words are
 * anagrams of each other
 */
void anagram(void) {

	char word1[255];
	char word2[255];
	uint8_t is_anagram = 0;

	// Read in two words
	puts("Enter first word:");
	gets(word1);
	puts(word1);
	puts("Enter second word:");
	gets(word2);
	puts(word2);

	// Count the number of letters in the words
	uint8_t letter_count_1 = count_letters(word1);
	uint8_t letter_count_2 = count_letters(word2);

	// Only look for anagram if the two words
	// have the same length
	if (letter_count_1 == letter_count_2) {

		// Sort words
		sort_word(word1);
		sort_word(word2);

		// Compare words
		for (uint8_t i=0; i<letter_count_1; i++) {
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
