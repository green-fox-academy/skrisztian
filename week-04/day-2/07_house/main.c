#include <stdio.h>

// TODO:
// Create a struct that represents a House
// It should store:
//  - The address of the house
//  - The price in EUR
//  - The number of rooms
//  - The area of the house in square meters


struct real_estate {
    char address[256];
    long price;
    int rooms;
    int area;
};

// TODO:
// The market price of houses is 400 EUR / square meters
// Create a function that takes a pointer to a house and decides if it's worth to buy
// (if the price is lower than the calculated price from it's area)

int if_buy(struct real_estate *house, int price)
{
    if ((house->price / house->area) < price)
        return 1;
    else
        return 0;
}

// TODO:
// Create a function that takes an array of houses (and it's length), and counts the
// houses that are worth to buy.

int count_buys(struct real_estate *houses, int size, int price)
{
    int count = 0;
    for (int i = 0; i < size; i++) {
        count += if_buy(&houses[i], price);
    }

    return count;
}

int main()
{
    struct real_estate houses[] = {
        {"Budapest", 50000, 10, 200},
        {"Szeged", 10000, 2, 40},
        {"Eger", 150000, 3, 120}
    };

    int size = sizeof(houses) / sizeof(houses[0]);

    printf("Houses to buy: %d\n", count_buys(houses, size, 400));

    return 0;
}
