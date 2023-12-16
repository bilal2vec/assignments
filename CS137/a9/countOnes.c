#include <assert.h>
#include <string.h>
#include <stdlib.h>

int countLongestOnes(char *s)
{
    int i = 0;
    int letter;

    int longest_so_far = 0;
    int current_seq_len = 0;

    int prev_was_one = 0;

    while (letter = s[i])
    {
        if (prev_was_one == 1)
        {
            if (letter == 49)
            {
                current_seq_len += 1;
                prev_was_one = 1;
            }
            else
            {
                current_seq_len = 0;
                prev_was_one = 0;
            }
        }
        else
        {
            if (letter == 49)
            {
                current_seq_len = 1;
                prev_was_one = 1;
            }
            else
            {
                current_seq_len = 0;
                prev_was_one = 0;
            }
        }

        if (current_seq_len > longest_so_far)
        {
            longest_so_far = current_seq_len;
        }

        i += 1;
    }

    return longest_so_far;
}

// int main(void)
// {
//     assert(countLongestOnes("000") == 0);
//     assert(countLongestOnes("10111000110") == 3);
// }