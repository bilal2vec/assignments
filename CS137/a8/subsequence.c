#include <stdio.h>
#include <stdbool.h>
#include <assert.h>

bool is_subsequence(char *s, char *t)
{
    int s_len = 0;
    while (s[s_len])
    {
        s_len += 1;
    }

    int t_len = 0;
    while (t[t_len])
    {
        t_len += 1;
    }

    int s_idx = 0;
    for (int i = 0; i < t_len; i++)
    {
        char t_current = t[i];
        char s_current = s[s_idx];

        if ((int)t_current == (int)s_current)
        {
            s_idx += 1;
        }
    }

    if (s_idx == s_len)
    {
        return true;
    }
    else
    {
        return false;
    }
}

// int main(void)
// {
//     assert(is_subsequence("area", "rectangular breadth"));
//     assert(!is_subsequence("pi", "circumference"));
//     return 0;
// }