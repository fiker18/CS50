#include <cs50.h>
#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

int count_letters(string text);
int count_words(string text);
int count_sentences(string text);

int main(void)
{
    string input_text = get_string("Text: ");

    int letters = count_letters(input_text);
    double words = count_words(input_text);
    int sentences = count_sentences(input_text);

    double grade = 0.0588 * (letters * 100 / words) - 0.296 * (sentences * 100 / words) - 15.8;

    if (grade < 1)
    {
        printf("Before Grade 1\n");
    }
    else if (grade > 15)
    {
        printf("Grade 16+\n");
    }
    else
    {
        printf("Grade %i\n", (int) round(grade));
    }
}

int count_letters(string text)
{
    int letter = 0;
    for (int i = 0, num_char = strlen(text); i < num_char; i++)
    {
        if (isalpha(text[i]))
        {
            letter++;
        }
    }
    return letter;
}

int count_words(string text)
{
    int word = 1;
    for (int i = 0, num_char = strlen(text); i < num_char; i++)
    {
        if (text[i] == ' ')
        {
            word++;
        }
    }
    return word;
}

int count_sentences(string text)
{
    int sentence = 0;
    for (int i = 0, num_char = strlen(text); i < num_char; i++)
    {
        if (text[i] == '.')
        {
            sentence++;
        }
        else if (text[i] == '!')
        {
            sentence++;
        }
        else if (text[i] == '?')
        {
            sentence++;
        }
    }
    return sentence;
}
