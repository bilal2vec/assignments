#include <stdio.h>
#include <assert.h>

int mult_coeff(int poly1[], int n1, int poly2[], int n2, int deg)
{

    if (deg > (n1 - 1) + (n2 - 1))
    {
        return 0;
    }

    int coef = 0;

    for (int i = 0; i < n1; ++i)
    {
        for (int j = 0; j < n2; ++j)
        {

            int current_coef = poly1[i] * poly2[j];
            int current_degree = i + j;

            if (current_degree == deg)
            {
                coef += current_coef;
            }
        }
    }

    return coef;
}

// int main(void)
// {
//     int poly1[] = {1};       //1
//     int poly2[] = {1, 2, 3}; //3x^2 + 2x + 1
//     //
//     assert(0 == mult_coeff(poly1, 1, poly1, 1, 3));
//     assert(3 == mult_coeff(poly1, 1, poly2, 3, 2));
//     assert(2 == mult_coeff(poly1, 1, poly2, 3, 1));
//     int poly3[] = {1, 2, 3, 4, 5}; //5x^4 + 4x^3 + 3x^2 + 2x + 1
//     int poly4[] = {3, 2, 5, 4, 5}; //5x^4 + 4x^3 + 5x^2 + 2x + 3
//     //
//     assert(32 == mult_coeff(poly3, 5, poly4, 5, 3));
//     assert(3 == mult_coeff(poly3, 5, poly4, 5, 0));
//     assert(25 == mult_coeff(poly3, 5, poly4, 5, 8));
//     return 0;
// }