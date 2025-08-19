#include <cs50.h>
#include <stdio.h>

int main(void)
{
    int owed;
    do
    {
        owed = get_int("Change owed: ");
    }
    while (owed < 0);

    int q = 0;
    int d = 0;
    int n = 0;
    int p = 0;

    while (owed >= 25)
    {
        q++;
        owed -= 25;
    }
    while (owed >= 10)
    {
        d++;
        owed -= 10;
    }
    while (owed >= 5)
    {
        n++;
        owed -= 5;
    }
    while (owed >= 1)
    {
        p++;
        owed -= 1;
    }

    int coins = q + d + n + p;
    printf("%i \n", coins);
}
