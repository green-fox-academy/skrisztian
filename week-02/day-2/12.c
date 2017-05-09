#include <stdio.h>
#include <stdint.h>

int main() {
    uint8_t a = 60;
    uint8_t b = 0b00111100;
    uint8_t c = 0x3C;
    uint8_t d = 0x3c;

    printf("------------------\n", d);
    printf("%d\n", a);
    printf("%d\n", b);
    printf("%d\n", c);
    printf("%d\n", d);
    printf("------------------\n", d);

    // Try with %x
    printf("In HEX\n");

    // Multiply a by 2
    printf("%x\n", a*2);

    // Shift left b by 1
    printf("%x\n", b<<1);

    printf("------------------\n", d);

    // Check their values

    printf("In DEC\n");

    // Multiply a by 2
    printf("%d\n", a*2);

    // Shift left b by 1
    printf("%d\n", b<<1);
    printf("------------------\n", d);

    return 0;
}
