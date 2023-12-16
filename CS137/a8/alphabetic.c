// #include <stdio.h>

#include <assert.h>
#include <stdlib.h>
#include <string.h>

char *alphabetic(const char *s)
{

    if (s == NULL)
    {
        return NULL;
    }

    int len = 0;
    while (s[len])
    {
        len += 1;
    }

    if (len == 0)
    {
        char *temp = malloc(sizeof(char));
        temp[0] = '\0';
        return temp;
    }

    int alphabetical_len = 0;
    for (int i = 0; i < len; i++)
    {
        if ((((int)s[i] >= 65) && ((int)s[i] <= 90)) || (((int)s[i] >= 97) && ((int)s[i] <= 122)))
        {
            alphabetical_len += 1;
        }
    }

    char *new_string = malloc((sizeof(char) * alphabetical_len) + 1);

    int idx2 = 0;

    for (int i = 0; i < len; i++)
    {
        if ((((int)s[i] >= 65) && ((int)s[i] <= 90)) || (((int)s[i] >= 97) && ((int)s[i] <= 122)))
        {
            new_string[idx2] = s[i];
            idx2 += 1;
        }
    }
    new_string[idx2] = '\0';

    return new_string;
}

// int main(void)
// {
//     char *a;
//     a = alphabetic("Ready..123. aim... fire!");
//     assert(!strcmp(a, "Readyaimfire"));
//     free(a);
//     a = alphabetic("");
//     assert(!strcmp(a, ""));
//     free(a);
//     char *temp = NULL;
//     a = alphabetic(temp);
//     assert(a == NULL);
//     return 0;
// }