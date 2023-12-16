#include <stdio.h>
#include <assert.h>
#include <math.h>

#define EPSILON 0.0001
#define PI acos(-1)

double newton(double (*f)(double), double (*df)(double), double x0, int iters)
{

    double x = x0;
    for (int i = 0; i < iters; ++i)
    {
        double y_i = f(x);
        double m = df(x);

        x = (-y_i + (m * x)) / m;
    }

    return x;
}

// int main(void)
// {
//     double approx = newton(sin, cos, 2.0, 100);
//     if (fabs(approx - PI) < EPSILON)
//     {
//         printf("%g\n", approx);
//     }
//     return 0;
// }