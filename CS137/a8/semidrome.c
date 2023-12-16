#include <stdbool.h>
#include <string.h>
#include <assert.h>

bool is_palindrome(char *s, int length)
{
    if (length < 2)
    {
        return false;
    }

    for (int i = 0; i < length / 2; i++)
    {
        int start_idx = i;
        int end_idx = length - i - 1;

        if (s[start_idx] != s[end_idx])
        {
            return false;
        }
    }
    return true;
}

bool is_semidrome(char *s)
{
    int s_length = 0;
    while (s[s_length])
    {
        s_length += 1;
    }

    if (s_length < 2)
    {
        return false;
    }

    int start_idx = 0;
    int end_idx = s_length - 1;

    while (start_idx < end_idx)
    {
        char new_string[100];
        strcpy(new_string, s + start_idx);

        if (is_palindrome(new_string, end_idx - start_idx + 1))
        {
            start_idx += end_idx - start_idx + 1;
            end_idx = s_length - 1;
        }
        else
        {
            end_idx -= 1;
        }
    }

    if (start_idx == s_length)
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
//     assert(!is_semidrome("a"));
//     assert(is_semidrome("baab"));
//     assert(!is_semidrome("abab"));
//     assert(is_semidrome("racecar"));
//     assert(!is_semidrome("racecars"));
//     assert(is_semidrome("racecarsasss"));
//     assert(is_semidrome("popeye"));
//     assert(!is_semidrome("aab"));
//     assert(is_semidrome("aabbbbcc"));
//     return 0;
// }