#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max number of candidates
#define MAX 9

// preferences[i][j] is number of voters who prefer i over j
int preferences[MAX][MAX];

// locked[i][j] means i is locked in over j
bool locked[MAX][MAX];

// Each pair has a winner, loser
typedef struct
{
    int winner;
    int loser;
} pair;

// Array of candidates
string candidates[MAX];
pair pairs[MAX * (MAX - 1) / 2];

int pair_count;
int candidate_count;

// Function prototypes
bool vote(int rank, string name, int ranks[]);
void record_preferences(int ranks[]);
void add_pairs(void);
void sort_pairs(void);
void lock_pairs(void);
void print_winner(void);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: tideman [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX)
    {
        printf("Maximum number of candidates is %i\n", MAX);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i] = argv[i + 1];
    }

    // Clear graph of locked in pairs
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            locked[i][j] = false;
        }
    }
    //clear graph of preferences
    for(int i = 0; i < candidate_count; i++)
    {
        for(int j = 0; j < candidate_count; j++)
        {
            preferences[i][j] = 0;
        }
    }

    pair_count = 0;
    int voter_count = get_int("Number of voters: ");

    // Query for votes
    for (int i = 0; i < voter_count; i++)
    {
        // ranks[i] is voter's ith preference
        int ranks[candidate_count];

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            if (!vote(j, name, ranks))
            {
                printf("Invalid vote.\n");
                return 3;
            }
        }

        record_preferences(ranks);

        printf("\n");
    }

    add_pairs();
    sort_pairs();
    lock_pairs();
    print_winner();
    return 0;
}

// Update ranks given a new vote
bool vote(int rank, string name, int ranks[])
{
    for(int i = 0; i < candidate_count; i++)
    {
        if(strcmp(name, candidates[i]) == 0)
        {
            ranks[rank] = i;
            return true;
        }
    }
    return false;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{

    // k is the voter choice from 0 to candiate_count
    for(int k = 0; k < candidate_count; k++)
    {
        // i is the candiates that is voted placed number
        for(int i = 0; i < candidate_count; i++)
        {
            if(ranks[k] == i)
            {
                for(int l = 1; l < candidate_count; l++)
                {
                    for(int j = 0; j < candidate_count; j++)
                    {
                        if(ranks[k+l] == j)
                        {
                            preferences[i][j]++;
                        }
                    }
                }
            }
        }
    }
    return;
}

// Record pairs of candidates where one is preferred over the other
void lock_pairs(void)
{
    for(int i = 0; i < pair_count; i++)
    {
        if (!check_cycle(pairs[i].winner, pairs[i].loser))
        {
            locked[pairs[i].winner][ pairs[i].loser] = true;
        }
    }
    return;
}

bool check_cycle(int winner, int loser)
{
    if(locked[loser][winner])
    {
        return true;
    }
    else
    {
        for(int i = 0; i < candidate_count; i++)
        {
            if(locked[loser][i])
            {
                if(check_cycle(winner,i))
                {
                    return true;
                }
            }
        }
        return false;
    }
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    // sorts the pairs in descending order
    pair temp;
    for(int i = 0; i < pair_count - 1; i++)
    {
        for(int j = i + 1; j < pair_count; j++)
        {
            if(preferences[pairs[i].winner][pairs[i].loser] < preferences[pairs[j].winner][pairs[j].loser])
            {
                temp.winner = pairs[i].winner;
                temp.loser = pairs[i].loser;

                pairs[i].winner = pairs[j].winner;
                pairs[i].loser = pairs[j].loser;

                pairs[j].winner = temp.winner;
                pairs[j].loser = temp.loser;
            }
        }
    }
    return;
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    int check[pair_count];
    int temp;

    for(int i = 0; i < pair_count; i++)
    {
        check[i] = 0;
    }

    for(int k = 0; k < pair_count; k++)
    {
       locked[pairs[k].winner][pairs[k].loser] = true;

        check[pairs[k].loser]++;

        //sort check
        for(int i = 1; i < candidate_count - 1; i++)
        {
            if(check[0] > check[i])
            {
                temp = check[0];
                check[0] = check[i];
                check[i] = temp;
            }
        }
        // check if is there is no source candidate
        if(check[0] != 0)
        {
            locked[pairs[k].winner][pairs[k].loser] = false;
            check[pairs[k].loser]--;
        }
    }
    return;
}

// Print the winner of the election
void print_winner(void)
{
    //make sure the variable check is zero
    int check[pair_count];
    for(int i = 0; i < pair_count; i++)
    {
        check[i] = 0;
    }

    //counts if a candidate has no arrows pointing at it
    for(int i = 0; i < pair_count; i++)
    {
        for(int j = 0; j < pair_count; j++)
        {
            if(locked[i][j] == false)
            {
                check[j]++;
            }
        }
    }

    //prints out the winner
    for(int i = 0; i < pair_count; i++)
    {
        if(check[i] == pair_count)
        {
            printf("%s\n", candidates[i]);
        }
    }
    return;
}
