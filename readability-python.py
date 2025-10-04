from cs50 import get_string

def main():


    input_text = get_string("Text: ")

    letters = count_letters(input_text)
    words = input_text.count(" ") + 1
    sen_list = [".", "!", "?"]
    sentence = input_text.count(".") + input_text.count("!") + input_text.count("?")

    grade =  0.0588 * (letters * 100 / words) - 0.296 * (sentence * 100 / words) - 15.8

    if grade < 1:
        print("Before Grade 1")
    elif grade > 15:
        print("Grade 16+")
    else:
        print(f"Grade {round(grade)}")

def count_letters(text):
    count = 0
    for l in text:
        if l.isalpha():
            count +=1
    return count

main()
