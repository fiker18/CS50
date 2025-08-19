#include <stdio.h>
#include <cs50.h>

int main(void)
{
    long n =get_int("Enter your credit card number: ");
    long a = n;
    int fd;
    int d;
    int count = 0;

    while (true)
    {
        n /= 10;
        count++;
        if (n < 1)
        {
            break;
        }
    }
    fd = a % 10;
    printf("%i \n", fd);

    for (int i = 0; i < count - 1; i++)
    {
        a = a / 10;
        d = a % 10;
        printf("%i \n", d);
    }
}

