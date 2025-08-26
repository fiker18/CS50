#include <ctype.h>
#include <stdio.h>
#include <cs50.h>
#include <string.h>

int points[] = {1, 3, 3, 2, 1, 4, 2, 4, 1, 8, 5, 1, 3, 1, 1, 3, 10, 1, 1, 1, 1, 4, 4, 8, 4, 10};
int score (string word);

int main (void)
{
    // to input for diffrent players
    // a function that will check the score of the players
    // output of who is the winner

    string input_1 = get_string("Player 1: ");
    string input_2 =get_string("Player 2: ");

    int score1 = score(input_1);
    int score2 = score(input_2);

    if ( score1 > score2 )
    {
        printf("Player 1 wins\n");
    }
    else if ( score1 < score2 )
    {
        printf("Player 2 wins\n");
    }
    else
    {
        printf("tie\n");
    }

}



int score (string word)
{
    int total_score;
    int score;
    total_score = 0;
    
    for (int i = 0, n = strlen(word); i < n; i++)
    {
        if (isupper(word[i]))
        {
            score = points[word[i] - 'A'];
        }
        else if (islower(word[i]))
        {
            score = points[word[i] - 'a'];
        }
        else
        {
            score = 0;
        }

        total_score += score;
    }
    return total_score;
}
