import csv
import sys


def main():

    # Check for command-line usage
    if len(sys.argv) != 3:
        print("Error CLA Incorrect")
        sys.exit(1)

    # Read database file into a variable:
    database = open(sys.argv[1], "r")
    reader = csv.DictReader(database)

    # Read DNA sequence file into a variable
    dna = open(sys.argv[2], "r")
    read = dna.read()
    len_str = len(reader.fieldnames)
    # Find longest match of each STR in DNA sequence
    number = [0]
    for i in range(1, len_str):
        number.insert(i, longest_match(read, reader.fieldnames[i]))

    # Check database for matching profiles
    for row in reader:
        for i in range(1, len_str):
            if row[reader.fieldnames[i]] != str(number[i]):
                break
        else:
                print(row[reader.fieldnames[0]])
                database.close()
                dna.close()
                return
    print("No match")
    database.close()
    dna.close()
    return

def longest_match(sequence, subsequence):
    """Returns length of longest run of subsequence in sequence."""

    # Initialize variables
    longest_run = 0
    subsequence_length = len(subsequence)
    sequence_length = len(sequence)

    # Check each character in sequence for most consecutive runs of subsequence
    for i in range(sequence_length):

        # Initialize count of consecutive runs
        count = 0

        # Check for a subsequence match in a "substring" (a subset of characters) within sequence
        # If a match, move substring to next potential match in sequence
        # Continue moving substring and checking for matches until out of consecutive matches
        while True:

            # Adjust substring start and end
            start = i + count * subsequence_length
            end = start + subsequence_length

            # If there is a match in the substring
            if sequence[start:end] == subsequence:
                count += 1

            # If there is no match in the substring
            else:
                break

        # Update most consecutive matches found
        longest_run = max(longest_run, count)

    # After checking for runs at each character in seqeuence, return longest run found
    return longest_run


main()
