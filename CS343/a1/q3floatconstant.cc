#include <iostream>
#include <cmath>

using namespace std;

#include "q3floatconstant.h"

// Checks if character is a "+" or "-" sign and returns the appropriate sign value (1 or -1) and suspends coroutine if so
int FloatConstant::get_sign() {
    if (ch == '+' || ch == '-') {
        char sign = ch;
        suspend();
        if (sign == '-') {
            return -1;
        }
    }
    return 1;
}

// Suspends coroutine until character is not equal to c to skip over all instances of c
void FloatConstant::skip_char(char c) {
    while (ch == c) {
        suspend();
    }
}

// Throws Error event and suspends coroutine to return to caller
void FloatConstant::throw_error() {
    _Resume Error() _At resumer();
    suspend();
}

// Throws Match event with value and suspends coroutine to return to caller
void FloatConstant::throw_match(double value) {
    _Resume Match(value) _At resumer();
    suspend();
}

void FloatConstant::main() {
    // Handle error case where there is leading whitespace
    if (ch == ' ') {
        throw_error();
    }

    // Initialize counters for mantissa and exponent digits
    // Get sign of mantissa
    double mantissa = 0.0;
    int mantissa_sign = get_sign();
    int n_mantissa_digits = 0;
    int n_mantissa_fractional_digits = 0;

    int exponent = 0;
    int exponent_sign = 1;
    int n_exponent_digits = 0;

    // Skip leading zeros in mantissa
    skip_char('0');

    // Read in all mantissa digits before decimal point
    while (true) {
        // Break if decimal point or exponent is reached
        if (ch == '.') {
            break;
        }
        
        if (ch == 'E' || ch == 'e') {
            break;
        }

        // Throw error if character is not a digit
        if (!isdigit(ch)) {
            throw_error();
        }

        // Increment number of mantissa digits and add update mantissa value
        n_mantissa_digits += 1;
        mantissa = mantissa * 10 + (ch - '0');

        // Throw error if mantissa has more than 16 digits
        if (n_mantissa_digits > 16) {
            throw_error();
        }

        // Suspend coroutine to wait for next character
        suspend();
    }


    // If decimal point is reached, read in all mantissa digits after decimal point
    if (ch == '.') {
        suspend();

        // Read in all mantissa digits after decimal point
        while (true) {

            // Break if exponent or floating suffix is reached
            if (ch == 'E' || ch == 'e') {
                break;
            }

            if (ch == 'f' || ch == 'l' || ch == 'F' || ch == 'L') {
                break;
            }

            // Break if end of text is reached
            if (ch == EOT) {
                break;
            }

            // Throw error if character is not a digit
            if (!isdigit(ch)) {
                throw_error();
            }

            // Increment number of mantissa digits and add update mantissa value
            n_mantissa_digits += 1;
            n_mantissa_fractional_digits += 1;
            mantissa = mantissa + ((ch - '0') / pow(10, n_mantissa_fractional_digits));

            // Throw error if mantissa has more than 16 digits
            if (n_mantissa_digits > 16) {
                throw_error();
            }

            // Suspend coroutine to wait for next character
            suspend();
        }
    }

    // Throw error if mantissa has no digits
    // All valid floating point numbers must have at least one digit before or after the decimal point
    if (n_mantissa_digits == 0) {
        throw_error();
    }

    // If exponent is reached, read in all exponent digits
    if (ch == 'E' || ch == 'e') {
        suspend();

        // Get sign of exponent
        exponent_sign = get_sign();

        // Read in all exponent digits
        while (true) {
            // Break if floating suffix or EOT is reached
            if (ch == 'f' || ch == 'l' || ch == 'F' || ch == 'L') {
                break;
            }

            if (ch == EOT) {
                break;
            }

            // Throw error if character is not a digit
            if (!isdigit(ch)) {
                throw_error();
            }

            // Increment number of exponent digits and add update exponent value
            n_exponent_digits += 1;
            exponent = exponent * 10 + (ch - '0');

            // Throw error if exponent has more than 3 digits
            if (n_exponent_digits > 3) {
                throw_error();
            }

            // Suspend coroutine to wait for next character
            suspend();
        }

        // Throw error if exponent has no digits
        if (n_exponent_digits == 0) {
            throw_error();
        }
    }

    // Throw match event if floating suffix or EOT is reached
    if (ch == 'f' || ch == 'l' || ch == 'F' || ch == 'L') {
        // Throw error if mantissa has no digits and floating suffix is reached
        if (n_mantissa_digits == 0) {
            throw_error();
        } 

        // Throw match event with value if floating suffix is reached, since this is a valid floating point number
        throw_match((mantissa_sign * mantissa) * pow(10, exponent_sign * exponent));
    }

    // Throw match event with value if EOT is reached, since this is a valid floating point number
    if (ch == EOT) {
        // Throw error if mantissa has no digits and EOT is reached
        if (n_mantissa_digits == 0) {
            throw_error();
        }

        // Throw match event with value if EOT is reached, since this is a valid floating point number
        throw_match((mantissa_sign * mantissa) * pow(10, exponent_sign * exponent));
    }

    // Throw error if any other character is reached or if coroutine is resumed after EOT is reached
    _Resume Error() _At resumer();
}
