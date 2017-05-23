#include <stdio.h>
#include <stdlib.h>

// TODO:
// Create a function which receives a string as parameter, and returns
// the numbers of characters in the string.
// Don't use the strlen() function!

int count_chars(const char *string)
{
    int count;
    for (count = 0; string[count] != '\0'; count++);

    return count;
}


int main()
{
    char text[255] = {"Hello hali"};
    printf("%s\t%d", text, count_chars(text));

    return 0;
}
