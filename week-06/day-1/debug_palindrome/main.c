#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>

void  palindrome(char *input);

int main()
{
	char input[100];
	printf("please enter a word: \n");
	scanf("%s", input);
    palindrome(input);
    printf("\n");
	palindrome_simple(input);
	return 0;
}

void palindrome(char *input)
{
	printf("%s", input);
	char *pointer;
	for (int i = strlen(input) - 1; i >= 0; i--) {
		pointer = &input[i];
		printf("%c", *pointer);
	}
}

void palindrome_simple(char *input)
{
	printf("%s", input);
	for (int i = strlen(input) - 1; i >= 0; i--) {
		printf("%c", input[i]);
	}
}
