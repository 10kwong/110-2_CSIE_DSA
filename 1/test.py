import math

def GCD(a, b):
    ans = 1
    a, b = min(a, b), max(a, b)
    while (a != 0 and b != 0):
        if a % 2 == 0 and b % 2 == 0:
            ans = ans * 2
            a = a / 2
            b = b / 2
        elif a % 2 == 0:
            a = a / 2
        elif b % 2 == 0:
            b = b / 2
        
        if a > b:
            a, b = b, a
        b = b - a
    return a * ans

a = 77
b = 7
# print(GCD(a, b))
# print(a - b)