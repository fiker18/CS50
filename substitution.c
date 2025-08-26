#include <cs50.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>

bool check_repetition(string text);
bool check_alphabetical(string text);

int main(int argc, string argv[])
{
    if (argc == 2 && check_alphabetical(argv[1]) && strlen(argv[1]) == 26 &&
        check_repetition(argv[1]))
    {
        string input_text = get_string("plaintext: ");
        char characters;
        printf("ciphertext: ");

        for (int i = 0, n = strlen(input_text); i < n; i++)
        {
            if (isupper(input_text[i]) != 0)
            {
                characters = toupper(argv[1][input_text[i] - 'A']);
                printf("%c", characters);
            }
            else if (islower(input_text[i]) != 0)
            {
                characters = tolower(argv[1][input_text[i] - 'a']);
                printf("%c", characters);
            }
            else
            {
                characters = input_text[i];
                printf("%c", characters);
            }
        }
        printf("\n");
        return 0;
    }
    else if (argc > 2 || argc == 1 || check_alphabetical(argv[1]) == false)
    {
        printf("Usage: %s key\n", argv[0]);
        return 1;
    }
    else if (strlen(argv[1]) > 26 || strlen(argv[1]) < 26)
    {
        printf("Key must contain 26 characters.\n");
        return 1;
    }
    else if (check_repetition(argv[1]) == false)
    {
        printf("Repitited character in the key!");
        return 1;
    }
}

bool check_repetition(string text)
{
    for (int i = 0, n = strlen(text); i < n; i++)
    {
        text[i] = toupper(text[i]);
    }

    for (int i = 0, n = strlen(text); i < n; i++)
    {
        for (int j = 0; j < i; j++)
        {
            if (text[j] == text[i])
            {
                return false;
            }
        }
    }
    return true;
}

bool check_alphabetical(string text)
{
    for (int i = 0, n = strlen(text); i < n; i++)
    {
        if (isalpha(text[i]) == 0)
        {
            return false;
        }
    }
    return true;
}
