#include <stdio.h>
#include <string.h>
#include <string.h>

int is_palindrom(char *a);

int main()
{
	char original_string[100], current_string[100];
	printf("Enter the string to check if it has palindromes\n");
	gets(original_string);

	for (int i = 0; i < strlen(original_string); i++) {
		for (int j = i; j < strlen(original_string); j++) {
			int l = 0;
			for (int k = i; k <= j; k++) {
                current_string[l] = original_string[k];
                l++;
			}
			current_string[l] = 0;
			if ((strlen(current_string) > 1) && is_palindrom(current_string))
                printf("%s\n", current_string);
		}
	}

    return 0;
}

int is_palindrom(char *a)
{
	char b[100];
	strcpy(b, a);
	strrev(b);

	if (strcmp(a, b) == 0)
		return 1;
	else
		return 0;
}
