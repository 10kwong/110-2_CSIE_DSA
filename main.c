#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// define number of DIGIT_LIMIT_LIMIT
// 257 digits at most according to the description, set it to 1024 for safety
#define DIGIT_LIMIT 1024

// define a Data structure with struct which can store Big integer using array, 
// it's digits (the size of array) represents the number of digits of the integer
// digits should keep track of the integer throughout different arithmetic operations like division.
typedef struct BigInt{
    // keep track of the number of digits
    int digits;
    // limit of digits 
    int digit_limit;
    // save integer digit number in each of the slot 
    // note that the number is saved in reverse order
    // Ex: Number 7053211 in the array is like {1, 1, 2, 3, 5, 0, 7}
    int *ptr;
} BigInt;


// To initialize the BigInt data structure
// input: char pointer that saved an integer (supposedly large), no characters input is allowed
// output: a BigInt pointer which data values have been initialized.
// usage: BigInt_initialize(NULL) or BigInt_initialize("123456")
// result: digits will be 0 if input is NULL, number of digits of the input number otherwise.
//         digit_limit is the limit size for BigInt 
//         ptr, a pointer which will point to an integer array with size of digit_limit
BigInt* BigInt_initialize(char *a_tmp);

// free memery and delete all datum inside BigInt
void BigInt_destruct(BigInt *a);

// print out the status of BigInt in character form
// use variable mode to control the display of contents
// mode == 1: display status of BigInt
// mode == 0: print out the value of BigInt 
// mode == -1: print out the status of BigInt pointer array 
void BigInt_print(BigInt *, int mode);

// replace the value of from by to
void BigInt_replace(BigInt *to, BigInt *from);

// make a copy of a BigInt
// Input: BigInt pointer 
// Output: BigInt pointer that stored same value as the input
BigInt* BigInt_copy(BigInt *obj);

// compare two BigInt 
// return 1 if a is greater than b,
// 0 if a equals to b,
// -1 if b is greater than a
int BigInt_Comparator(BigInt *a, BigInt *b);

// this function will divide BigInt by 2
void BigInt_DivideByTwo(BigInt *);
// this function will multiple BigInt by 2
void BigInt_MultipleByTwo(BigInt *);

// subtraction between two BigInts
// input: Pointer of two BigInt entry a, b where value of a is greater than b
// output: A BigInt pointer that saved the result of digit of a array - digit of b array
BigInt* BigInt_Subtraction(BigInt *a, BigInt *b);

// function that reverse a string 
char* string_reverse(char *);

// helper function that return the integer in character array form 
// return Null if the input is Null 
char* BigInt_getArray(BigInt *);

// return the integer but in reverse form, Null will be returned if the input is Null
char* BigInt_getValue(BigInt *);

// compare two BigInt, return the one that is larger 
BigInt* BigInt_min(BigInt *, BigInt *);

// compare two BigInt, return the one that is smaller
BigInt* BigInt_max(BigInt *, BigInt *);

// multiple a BigInt by 2^k for some positive integer k
BigInt* BigInt_multiplicationByTwoPowerK(BigInt *, int k);

// check if BigInt is even or not
// input: BigInt pointer
// output: 1 if BigInt is even, 0 if BigInt is odd
int BigInt_isEven(BigInt *);

// swap two BigInt
void BigInt_swap(BigInt *a, BigInt *b);

// function that check whether the integer of BigInt is 0
// return 1 if BigInt is zero, -1 if BigInt ptr is Null, 0 otherwise
int BigInt_isZero(BigInt *);

// function that check whether the integer of BigInt is 1
// return 1 if BigInt is one, -1 if BigInt ptr is Null, 0 otherwise
int BigInt_isOne(BigInt *);

// Binary algorithm 
BigInt* BinaryAlgorithmForGCD(BigInt *a, BigInt *b);

int main()
{
    char tmp[DIGIT_LIMIT];
    scanf(" %s", tmp); 
    BigInt *a = BigInt_initialize(tmp); 
    scanf(" %s", tmp); 
    BigInt *b = BigInt_initialize(tmp);

    BigInt *res = BinaryAlgorithmForGCD(a, b);
    BigInt_print(res, 0);
    
    //  while (!BigInt_isZero(a)) {
    //     BigInt_print(a, 0);
    //     printf("\n");
    //     BigInt_DivideByTwo(a);
    // }
    // BigInt_print(a, 1);

    // int k = 5;
    // BigInt_multiplicationByTwoPowerK(a, k);

    // BigInt_print(a, 1);
    // BigInt_print(b, 1);
    // a =BigInt_Subtraction(a, b);
    // BigInt_print(a, 1);

    BigInt_destruct(a);
    BigInt_destruct(b);
    BigInt_destruct(res);
    
    return 0;
}

BigInt* BinaryAlgorithmForGCD(BigInt *a, BigInt *b)
{
    if (BigInt_isOne(a) || BigInt_isOne(b)) return BigInt_initialize("1");

    int k = 0;
    BigInt *n = BigInt_min(a, b);
    BigInt *m = BigInt_max(a, b);

    while (!BigInt_isZero(n) && !BigInt_isZero(m))
    {
        if (BigInt_isEven(n) && BigInt_isEven(m))
        {
            k++;
            BigInt_DivideByTwo(n);
            BigInt_DivideByTwo(m);
            // printf("Both divided by two\n");
            // BigInt_print(m, 0);
            // printf("\n");
            // BigInt_print(n, 0);
            // printf("\n---\n");
        }
        else if (BigInt_isEven(n))
        {
            BigInt_DivideByTwo(n);
            // printf("n divided by two\n");
            // BigInt_print(n, 0);
            // printf("\n---\n");
        }
        else if (BigInt_isEven(m))
        {
            BigInt_DivideByTwo(m);
            // printf("m divided by two\n");
            // BigInt_print(m, 0);
            // printf("\n---\n");
        }

        if (BigInt_Comparator(n, m) == 1) // if n is greater than m
            BigInt_swap(n, m);
        m = BigInt_Subtraction(m, n);
        // BigInt_print(m, 0);
        // printf("\n");
        // BigInt_print(n, 0);
        // printf("\n===\n");
    }

    BigInt_destruct(m);

    return BigInt_multiplicationByTwoPowerK(n, k);
}

BigInt* BigInt_initialize(char *a_tmp)
{
    BigInt *a = (BigInt *) malloc(sizeof(BigInt));
    a->digit_limit = DIGIT_LIMIT;
    a->ptr = malloc(a->digit_limit * sizeof(int));
    if (a_tmp == NULL)
    {
        a->digits = 0;
    }
    else 
    {
        a->digits = strlen(a_tmp);
        for (int i = 0; i < a->digits; i++)
        {
            a->ptr[i] = a_tmp[a->digits - 1 - i] - '0';
        }
        a->ptr[a->digits] = '\0';
    }
    return a;
}

void BigInt_destruct(BigInt *a)
{
    free(a->ptr);
    a->ptr = NULL;
    a->digits = 0;
    free(a);
    a = NULL;
    return;
}

void BigInt_replace(BigInt *to, BigInt *from)
{
    to->digits = from->digits;
    for (int i = 0 ; i < from->digits; i++)
        to->ptr[i] = from->ptr[i];
}

BigInt* BigInt_copy(BigInt *obj)
{
    BigInt *res = BigInt_initialize(BigInt_getValue(obj));
    return res;
}

void BigInt_print(BigInt *input, int mode)
{
    if (mode == 0 || mode == 1)
    {
        printf("%s", BigInt_getValue(input));
    }
    else if (mode == -1)
    {
        printf("%s", BigInt_getArray(input));
    }

    if (mode == 1)
    {
        printf("\n===\nPrint Status\n");
        printf("Digits: %d", input->digits);
        printf("\n===");
    }
    if (mode != 0) printf("\n");
}

int BigInt_Comparator(BigInt *a, BigInt *b)
{
    if (a->digits > b->digits) // if digits of a is more than b
        return 1;
    else if (a->digits < b->digits)
        return -1;
    else // digits of a equals to b, now we compare digit pairwise
    {
        int s = a->digits - 1;
        while (s >= 0)
        {
            if (a->ptr[s] > b->ptr[s])
                return 1;
            else if (a->ptr[s] < b->ptr[s])
                return -1;
            s--;
        }
    }
    return 0;
}

void BigInt_DivideByTwo(BigInt *input)
{
    if (input->ptr == NULL) 
    {
        printf("WRONG ANSWER\n");
        return;
    }
    // else if (BigInt_isOne(input)) 
    // {
    //     BigInt_destruct(input);
    //     input = BigInt_initialize("0");
    //     return;
    // }

    const int TWO = 2;
    float *complement = calloc(input->digit_limit, sizeof(float));
    for (int i = input->digits - 1; i >= 0 ; i--)
    {
        complement[i] = input->ptr[i] % TWO * 10 / TWO;
        input->ptr[i] /= TWO;
    }

    // add remainder back after divided by two 
    // invalid if the number is between 0 to 9
    if (input->digits > 1)
    {
        for (int i = input->digits - 2; i >= 0 ; i--)
        {
            input->ptr[i] += complement[i+1];
        }

        // check whether, and how many digits of the head of the number is 0
        //  and eliminate them all by updating digits
        while (input->ptr[input->digits - 1] == 0 && input->digits > 1)
        {
            input->digits--;
        }
    }

    // // check whether, and how many digits of the head of the number is 0
    // // and eliminate them all by updating digits
    // while (input->ptr[input->digits - 1] == 0)
    // {
    //     input->digits--;
    //     if (input->digits == 0)
    //     {
    //         input->digits = 1;
    //         break;
    //     }
    // }

    free(complement);
    complement = NULL;
}

void BigInt_MultipleByTwo(BigInt *input)
{
    const int TWO = 2;
    float *complement = calloc(input->digit_limit, sizeof(float));
    for (int i = 0; i < input->digits; i++)
    {
        input->ptr[i] *= TWO;
        complement[i] = (int) input->ptr[i] / 10;
        input->ptr[i] %= 10;
    }

    for (int i = 0; i < input->digits && i < input->digit_limit; i++)
    {
        input->ptr[i+1] += complement[i];
    }

    if (complement[input->digits-1] != 0)
        input->digits++;

    free(complement);
    complement = NULL;
}

BigInt* BigInt_Subtraction(BigInt *a, BigInt *b)
{
    char res_string[DIGIT_LIMIT] = {0};
    if (BigInt_Comparator(a, b) == -1) // if b is greater than a
        BigInt_swap(a, b);

    BigInt *big = BigInt_copy(a);
    BigInt *small = BigInt_copy(b);
    for (int i = 0 ; i < small->digits ; i++)
    {
        int diff = big->ptr[i] - small->ptr[i]; 
        if (diff < 0 && i < big->digits - 1)
        {
            diff += 10;
            big->ptr[i+1] -= 1;
        }
        res_string[i] = diff + '0';
    }

    for (int i = small->digits ; i < big->digits ; i++)
    {
        int num = big->ptr[i];
        if (num < 0 && i < big->digits - 1)
        {
            num += 10;
            big->ptr[i+1] -= 1;
        }
        res_string[i] = num + '0';
    }

    BigInt *res = BigInt_initialize(string_reverse(res_string));
    // check whether, and how many digits of the head of the number is 0
    // and eliminate them all by updating digits
    while (res->ptr[res->digits - 1] == 0 && res->digits > 1)
    {
        res->digits--;
    }
    BigInt_destruct(big);
    BigInt_destruct(small);
    return res;
}

char* string_reverse(char *string)
{
    char *reverse_string = malloc(sizeof(char) * strlen(string));
    for (int i = 0 ; i < strlen(string); i++)
        reverse_string[i] = string[strlen(string) - 1 - i];
    return reverse_string;
}

BigInt* BigInt_min(BigInt *a, BigInt *b)
{
    BigInt *res = NULL;
    if (BigInt_Comparator(a, b) == 1)
        res = BigInt_copy(b);
    else res = BigInt_copy(a);
    return res;
}

BigInt* BigInt_max(BigInt *a, BigInt *b)
{
    BigInt *res = NULL;
    if (BigInt_Comparator(a, b) == 1)
        res = BigInt_copy(a);
    else res = BigInt_copy(b);
    return res;
}

void BigInt_swap(BigInt *a, BigInt *b)
{
    BigInt tmp = *a;
    *a = *b;
    *b = tmp;
    return; 
}

char* BigInt_getArray(BigInt *entry)
{
    if (entry->ptr == NULL) return NULL;
    char* res = calloc(DIGIT_LIMIT, sizeof(char));
    for (int i = 0 ; i < entry->digits; i++)
        res[i] = entry->ptr[i] + '0';
    return res;
}

char* BigInt_getValue(BigInt *entry)
{
    if (entry->ptr == NULL) return NULL;
    char* res = calloc(DIGIT_LIMIT, sizeof(char));
    for (int i = 0 ; i < entry->digits; i++)
        res[i] = entry->ptr[entry->digits - 1 - i] + '0';
    return res;
    // return string_reverse(BigInt_getArray(entry));
}

int BigInt_isZero(BigInt *entry)
{
    if (entry == NULL) return -1;
    return ((entry->digits == 1) && (entry->ptr[entry->digits-1] == 0));
}

int BigInt_isOne(BigInt *entry)
{
    if (entry == NULL) return -1;
    return ((entry->digits == 1) && (entry->ptr[entry->digits-1] == 1));
}

BigInt* BigInt_multiplicationByTwoPowerK(BigInt *n, int k)
{
    if (k < 0)
    {
        while (k < 0)
        {
            BigInt_DivideByTwo(n);
            k++;
        }
    }
    else if (k > 0)
    {
        while (k >0)
        {
            BigInt_MultipleByTwo(n);
            k--;
        }
    }
    return n;   // note that k will equal to 0 at this point
}

int BigInt_isEven(BigInt *input)
{
    return (input->ptr[0] % 2 == 0);
}