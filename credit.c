#include <cs50.h>
#include <stdio.h>


int main(void)
{
    long n;
    do
    {
        n = get_long("Enter your credit card number: ");
    }
    while (n <= 0);

    long a = n;
    long b = n;
    long c = n;
    int fd;
    int d;
    int card_length = 0;

    while (true)
    {
        n /= 10;
        card_length++;
        if (n < 1)
        {
            break;
        }
    }
    fd = a % 10;
    int count = card_length;
    int double_sum = 0;
    int single_sum = fd;

    for (int i = 0; i < card_length - 1; i++)
    {
        a = a / 10;
        d = a % 10;

        if (card_length % 2 == 0)
        {
            if (count % 2 == 0)
            {
                if (d>=5)
                {
                    d = 2 * d - 9;
                }
                else
                {
                    d = 2 * d;
                }
                double_sum += d;
            }
            else
            {
                single_sum += d;
            }
            count--;
        }
        if (card_length % 2 == 1)
        {
            if (count % 2 == 1)
            {
                if (d>=5)
                {
                    d = 2 * d - 9;
                }
                else
                {
                    d = 2 * d;
                }
                double_sum += d;
            }
            else
            {
                single_sum += d;
            }
            count--;
        }
    }
    int total_sum = single_sum + double_sum;
    int sd;

    if (total_sum % 10 == 0)
    {
        for (int i = 0; i < card_length - 2; i++)
        {
        b = b / 10;
        sd = b % 10;
        }

        for (int i = 0; i < card_length - 1; i++)
        {
        c = c / 10;
        d = c % 10;
        }

        if (card_length == 15)
        {
            if (d == 3)
            {
                if ( sd == 4 || sd == 7)
                {
                    printf("AMEX\n");
                }
                else
                {
                    printf("INVALID\n");
                }
            }
            else
            {
                printf("INVALID\n");
            }
        }
        else if (card_length == 16)
        {
            if (d == 5)
            {
                if ( sd == 1 || sd == 2 || sd == 3 || sd == 4 || sd == 5)
                {
                    printf("MASTERCARD\n");
                }
                else
                {
                    printf("INVALID\n");
                }
            }
            else if (d == 4)
            {
                printf("VISA\n");
            }
            else
            {
                printf("INVALID\n");
            }
        }
        else if (card_length == 13)
        {
            if (d == 4)
            {
                printf("VISA\n");
            }
            else
            {
                printf("INVALID\n");
            }
        }
        else
        {
            printf("INVALID\n");
        }
    }
    else
    {
        printf("INVALID\n");
    }
}
