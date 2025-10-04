from cs50 import get_int

while True:
    Height = get_int("Height: ")
    if 1 <= Height <= 8:
        break

for i in range(Height):
    for j in range(Height - 1 - i):
        print(" ", end="")
    print("#" * (i+1), end="")
    print("  ", end="")
    print("#" * (i+1))
