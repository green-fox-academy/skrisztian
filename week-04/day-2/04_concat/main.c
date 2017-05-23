#include <stdio.h>
#include <stdlib.h>

/*
TODO: Implement a function that concatenates 2 strings (dest and source).
The return value should be the concatenated strings.
*/

int concat_strings(char *string1, const char *string2)
{
    // Check the size of the strings, assuming we got
    // null terminated strings
    int size1 = 0;
    int size2 = 0;

    while (string1[size1] != '\0') {
        size1++;
    }

    while (string2[size2] != '\0') {
        size2++;
    }

    // Add string2 to the end of string1 and the null terminator
    for (int i = size1; i < size1 + size2; i++) {
        string1[i] = string2[i - size1];
    }

    string1[size1 + size2] = '\0';

    return 0;
}

int main()
{
    char dest[256] = {"Hello "};
    char source[] = {"world!"};

    printf("dest: %s\n", dest);
    printf("source: %s\n", source);

    concat_strings(dest, source);
    printf("Concatenated string: %s\n", dest);

    return 0;
}
