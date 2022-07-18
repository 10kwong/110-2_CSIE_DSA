#include <stdio.h>
#include <string.h>
#include <stdlib.h>
char get_char(int i, int len, char* s);
// Return longest prefix palindrome
int KMP_FailureFunction(char *string);

char* s;
char* rs;
unsigned long len;
int main()
{
    s = (char *) calloc(20000001, sizeof(char));
    rs = (char *) calloc(20000001, sizeof(char));
    scanf(" %s", s);
    len = strlen(s);
    for (int i = len-1; i >= 0; i--)
        rs[len - 1 - i] = s[i]; 
    rs[len] = '\0';

    // Find longest prefix palindrome of both r and rs 
    int F1 = KMP_FailureFunction(s);  
    int F2 = KMP_FailureFunction(rs);
    if (F1 == len) printf("%d\n%s\n", 0, s);
    else if (F1 > F2)
    {
        int diff = len - F1;
        printf("%d\n", diff);
        for (int i = 0; i < diff; i++)
            printf("%c", rs[i]);
        printf("%s", s);
        printf("\n");
    }
    else if (F2 > F1)
    {
        int diff = len - F2;
        printf("%d\n", diff);
        for (int i = 0; i < diff; i++)
            printf("%c", s[i]);
        printf("%s", rs);
        printf("\n");
    }
    else // if F2 == F1 
    {
        int diff = len - F1;
        printf("%d\n", diff);
        for (int i = 0; i < diff; i++)
            printf("%c", rs[i]);
        printf("%s", s);
        printf("\n");
        for (int i = 0; i < diff; i++)
            printf("%c", s[i]);
        printf("%s", rs);
        printf("\n");
    }

    free(s);
    free(rs);
    return 0;
}
char get_char(int i, int len, char* s)
{
    if (i < len)
        return s[i];
    else if (i == len)
        return ' ';
    else if (i <= 2 * len)
        return s[2 * len - i];
    else return ' ';
}

int KMP_FailureFunction(char *string)
{
    int M = strlen(string);
    // printf("%s\n", string);
    int F[2 * M + 1];
    F[0] = 0;
    int i = 1;
    while (i <= 2 * M) {
        int j = F[i-1];
        while (j > 0 && get_char(i, M, string) != get_char(j, M, string))
            j = F[j-1];
        if (get_char(i, M, string) == get_char(j, M, string))
            j++;
        F[i] = j;
        i++;
    }
    // for (int i = 0; i <= 2*M; i++)
    //     printf("%d_", F[i]);
    // printf("\n");
    return F[2 * M];
}