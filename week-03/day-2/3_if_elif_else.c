#include <stdio.h>
#include <stdlib.h>

// Define the TOTORO macro which holds a number
#define TOTORO 19

int main()
{
    // Use the #if, #elif, #else macros
    // If the TOTORO macro is greater than 3 print out "Greater than 3"
    #if TOTORO > 3
    printf("Totoro class member count is greather than 3\n");

    // If the TOTORO macro is lower than 3 print out "Lower than 3"
    #elif TOTORO < 3
    printf("Totoro class member count is smaller than 3\n");

    // Else print out "TOTORO is 3"
    #else
    printf("Totoro class member count is 3\n");
    #endif // TOTORO

    return 0;
}
