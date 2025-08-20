#include <cs50.h>
#include <stdio.h>

int main(void)
{
    long card;
    do
    {
        card = get_long("Enter your credit card number: ");
    }
    while (card <= 0);

    // variable a,b,c are be copy of the card
    long a = card;
    long b = card;
    long c = card;

    int last_digit;
    int digit;
    int card_length = 0;

    while (true)
    {
        card /= 10;
        card_length++;
        if (card < 1)
        {
            break;
        }
    }
    last_digit = a % 10;
    int count = card_length;
    int double_sum = 0;
    int single_sum = last_digit;

    for (int i = 0; i < card_length - 1; i++)
    {
        a = a / 10;
        digit = a % 10;

        if (card_length % 2 == 0)
        {
            if (count % 2 == 0)
            {
                if (digit >= 5)
                {
                    digit = 2 * digit - 9;
                }
                else
                {
                    digit *= 2;
                }
                double_sum += digit;
            }
            else
            {
                single_sum += digit;
            }
            count--;
        }
        if (card_length % 2 == 1)
        {
            if (count % 2 == 1)
            {
                if (digit >= 5)
                {
                    digit = 2 * digit - 9;
                }
                else
                {
                    digit *= 2;
                }
                double_sum += digit;
            }
            else
            {
                single_sum += digit;
            }
            count--;
        }
    }
    int total_sum = single_sum + double_sum;

    if (total_sum % 10 == 0)
    {
        int second_digit;
        for (int i = 0; i < card_length - 2; i++)
        {
            b = b / 10;
            second_digit = b % 10;
        }

        int first_digit;
        for (int i = 0; i < card_length - 1; i++)
        {
            c = c / 10;
            first_digit = c % 10;
        }

        if (card_length == 15)
        {
            if (first_digit == 3)
            {
                if (second_digit == 4 || second_digit == 7)
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
            if (first_digit == 5)
            {
                if (second_digit == 1 || second_digit == 2 || second_digit == 3 ||
                    second_digit == 4 || second_digit == 5)
                {
                    printf("MASTERCARD\n");
                }
                else
                {
                    printf("INVALID\n");
                }
            }
            else if (first_digit == 4)
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
            if (first_digit == 4)
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
