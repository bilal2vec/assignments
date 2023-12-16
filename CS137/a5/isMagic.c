#include <stdio.h>
#include <assert.h>

int isMagic(int grid[4][4])
{
    int uniques[16] = {0};
    for (int i = 0; i < 4; ++i)
    {
        for (int j = 0; j < 4; ++j)
        {
            int val = grid[i][j];

            if (uniques[val - 1] == 1)
            {
                return 0;
            }

            uniques[val - 1] = 1;
        }
    }

    int row_1_sum = grid[0][0] + grid[0][1] + grid[0][2] + grid[0][3];
    int row_2_sum = grid[1][0] + grid[1][1] + grid[1][2] + grid[1][3];
    int row_3_sum = grid[2][0] + grid[2][1] + grid[2][2] + grid[2][3];
    int row_4_sum = grid[3][0] + grid[3][1] + grid[3][2] + grid[3][3];

    int col_1_sum = grid[0][0] + grid[1][0] + grid[2][0] + grid[3][0];
    int col_2_sum = grid[0][1] + grid[1][1] + grid[2][1] + grid[3][1];
    int col_3_sum = grid[0][2] + grid[1][2] + grid[2][2] + grid[3][2];
    int col_4_sum = grid[0][3] + grid[1][3] + grid[2][3] + grid[3][3];

    int diag_1_sum = grid[0][0] + grid[1][1] + grid[2][2] + grid[3][3];
    int diag_2_sum = grid[0][3] + grid[1][2] + grid[2][1] + grid[3][0];

    if ((row_1_sum == row_2_sum) && (row_3_sum == row_4_sum) && (col_1_sum == col_2_sum) && (col_3_sum == col_4_sum) && (diag_1_sum == diag_2_sum))
    {
        return 1;
    }

    return 0;
}

// int main(void)
// {
//     int testGrid[4][4] = {{1, 15, 14, 4}, {10, 11, 8, 5}, {7, 6, 9, 12}, {16, 2, 3, 13}};
//     assert(isMagic(testGrid));
//     int testGrid2[4][4] = {{1, 15, 14, 4}, {10, 11, 8, 5}, {7, 6, 9, 12}, {2, 16, 3, 13}};
//     assert(!isMagic(testGrid2));
// }