#include <cs50.h>
#include <stdio.h>

void print_hash(int b);
int main(void)
{
    int n;
    do
    {
        n = get_int("Height: ");
    }
    while (n < 1 || n > 8);

    int a = n;
    int b = 1;
    for (int row = 0; row < n; row++)
    {
        for (int j = 0; j < a - 1; j++)
        {
            printf(" ");
        }
        print_hash(b);
        for (int l = 0; l < 2; l++)
        {
            printf(" ");
        }
        print_hash(b);
        a--;
        b++;
        printf("\n");
    }
}
void print_hash(int b)
{
    for (int k = 0; k < b; k++)
    {
        printf("#");
    }
}
