from cs50 import get_string

while True:
    card = get_string("Number: ")
    if int(card) >= 0:
        break


length = len(card)
double_sum = 0
single_sum = 0

for i in (card[length-2::-2]):
    if int(i) >= 5:
        double_sum += 2 * int(i) - 9
    else:
        double_sum += 2 * int(i)

for i in (card[::-2]):
    single_sum += int(i)

total_sum = single_sum + double_sum

if total_sum % 10 != 0:a
    print("INVALID")
else:
    if length == 15 and card[0] == "3" and (card[1] == "4" or card[1] == "7"):
        print("AMEX")
    elif length == 16 and card[0] == "5" and (card[1] == "1" or card[1] == "2" or card[1] == "3" or card[1] == "4" or card[1] == "5"):
        print("MASTERCARD")
    elif (length == 16 or length == 13) and card[0] == "4":
        print("VISA")
    else:
        print("INVALID")


