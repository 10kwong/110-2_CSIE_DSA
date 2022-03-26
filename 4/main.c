#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
void ERROR() {while(1){}}

#define STACK_SIZE_LIMIT 1000000
#define INPUT_LENG 10000000

typedef struct CStack
{
    long long int top; 
    long long int lim;
    char *items; 
} CStack;
CStack* CStack_init(); 
int CStack_isEmpty(CStack *); 
void CStack_free(CStack *);
int CStack_push(CStack *, char item);
char CStack_pop(CStack *);
char CStack_peek(CStack *);
void CStack_print(CStack *, int op);

typedef struct IStack
{
    long long int top; 
    long long int lim;
    long long int *items; 
} IStack;
IStack* IStack_init(); 
int IStack_isEmpty(IStack *); 
void IStack_free(IStack *);
int IStack_push(IStack *stack, long long int item);
long long int IStack_pop(IStack *stack);
long long int IStack_peek(IStack *);
void IStack_print(IStack *, int op);

void Eval(char *);
void OperatorProcess(CStack *, IStack *, char ch);
long long int Calculate(CStack *c, IStack *i);
int priority_function(char );
int isSymbol(char );

int main()
{
    char *expression = (char *) malloc(INPUT_LENG * sizeof(char));
    scanf(" %s", expression);
    Eval(expression); 
    free(expression); 
    return 0;
}

void Eval(char *ex)
{
    long long int len = strlen(ex);
    CStack *cStack = CStack_init();
    IStack *iStack = IStack_init();

    for (int i = 0 ; i < len ; i++)
    {
        if (isdigit(ex[i]))
        {
            long long int number = atoll(&ex[i]);
            while (isdigit(ex[i]))
                i++;
            i--;
            IStack_push(iStack, number);
        }
        else if (isSymbol(ex[i]))
        {
            OperatorProcess(cStack, iStack, ex[i]);
        }
    }
    CStack_free(cStack);
    IStack_free(iStack);
}

void OperatorProcess(CStack *c, IStack *i, char ch)
{
    if (CStack_isEmpty(c)){
        CStack_push(c, ch);
        return;
    }

    if (ch == '(')
    {
        CStack_push(c, ch);
    }
    else if (ch == ')')
    {
        while (CStack_peek(c) != '(')
        {
            long long int res = Calculate(c, i);
            IStack_push(i, res);
        }
        CStack_pop(c);
    }
    else if (ch == '=')
    {
        while (!CStack_isEmpty(c))
        {
            long long int res = Calculate(c, i);
            IStack_push(i, res);
        }
        printf("Print: %lld\n", IStack_peek(i));
    }
    else 
    {
        int orig = priority_function(CStack_peek(c)), upco = priority_function(ch);
        while (upco <= orig)
        {
            long long int res = Calculate(c, i);
            IStack_push(i, res);
            orig = priority_function(CStack_peek(c));
        }
        CStack_push(c, ch);
    }
}

int isSymbol(char a)
{
    return a == '+' || a == '-' || a == '*' || \
    a == '/' || a == '%' || a == '(' || a == ')' || a == '=';
}

long long int Calculate(CStack *c, IStack *i)
{
    char op = CStack_pop(c);
    long long int a = IStack_pop(i);
    long long int b = IStack_pop(i);

    if (op == '+')
        return b + a;
    else if (op == '-')
        return b - a;
    else if (op == '*') 
        return b * a;
    else if (op == '/')
        return (long long int) floorl ( b / (long double) a);
    else if (op == '%')
        return b % a;
    else ERROR();
    return 0;
}

int priority_function(char ch)
{
    if (ch == '+' || ch == '-')
        return 1;
    else if (ch == '*' || ch == '/' || ch == '%')
        return 2;
    else return 0;
}

CStack* CStack_init()
{
    CStack *aStack = (CStack *) malloc(sizeof(CStack));
    aStack->top = -1; 
    aStack->lim = STACK_SIZE_LIMIT;
    aStack->items = (char *) malloc(aStack->lim * sizeof(char));
    return aStack;
}
int CStack_isEmpty(CStack *stack)
{
    return stack->top == -1;
}
void CStack_free(CStack *stack)
{
    free(stack->items);
    free(stack);
    stack = NULL; 
}
int CStack_push(CStack *stack, char item)
{
    if (stack->lim > stack->top)
    {
        stack->items[++stack->top] = item;
        return 1;  
    }
    else return 0;
}
char CStack_pop(CStack *stack)
{
    if (!CStack_isEmpty(stack))
        return stack->items[stack->top--];
    else ERROR();
    return ' ';
}
char CStack_peek(CStack *stack)
{
    if (!CStack_isEmpty(stack))
        return stack->items[stack->top];
    else return ' ';
}

IStack* IStack_init()
{
    IStack *aStack = (IStack *) malloc(sizeof(IStack));
    aStack->top = -1; 
    aStack->lim = STACK_SIZE_LIMIT;
    aStack->items = (long long int *) malloc(aStack->lim * sizeof(long long int));
    return aStack;
}
int IStack_isEmpty(IStack *stack)
{
    return stack->top == -1;
}
void IStack_free(IStack *stack)
{
    free(stack->items);
    free(stack);
    stack = NULL; 
}
int IStack_push(IStack *stack, long long int item)
{
    if (stack->top < stack->lim)
    {
        stack->items[++stack->top] = item;
        return 1;  
    }
    else return 0;
}
long long int IStack_pop(IStack *stack)
{
    if (!IStack_isEmpty(stack))
        return stack->items[stack->top--];
    else ERROR();
    return ' ';
}
long long int IStack_peek(IStack *stack)
{
    if (!IStack_isEmpty(stack))
        return stack->items[stack->top];
    else return ' ';
}