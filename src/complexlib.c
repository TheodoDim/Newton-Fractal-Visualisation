// Δοθέντων 2 μιγαδικών αριθμών: a + bi , c + di
// A == a, B == b, C == c, D == d 
#define A num1.real
#define B num1.imaginary
#define C num2.real
#define D num2.imaginary
#include <math.h>

typedef struct{
    double real;
    double imaginary;
} complex_number;

// άθροισμα 2 μιγαδικών αριθμών: num1 , num2 => num1 + num2
complex_number sum(const complex_number num1, const complex_number num2){
    complex_number total;
    total.real = A + C;
    total.imaginary = B + D;
    return total;
}

// διαφορά 2 μιγαδικών αριθμών: num1, num2 => num1 - num2
complex_number subtract(const complex_number num1, const complex_number num2){
    complex_number diffrence;
    diffrence.real = A - C;
    diffrence.imaginary = B - D;
    return diffrence;
}

// γινόμενο 2 μιγαδικών αριθμών: num1 , num2 => num1 * num2
complex_number multiply(const complex_number num1, const complex_number num2){
    complex_number product;
    product.real = A * C - B * D;
    product.imaginary = A * D + B * C;
    return product;
}

// πηλίκο 2 μιγαδικών αριθμών: num1 , num2 => num1 / num2
complex_number divide(const complex_number num1, const complex_number num2){
    complex_number quotient;
    quotient.real = (A * C + B * D) / (C * C + D * D);
    quotient.imaginary = (B * C - A * D) / (C * C + D * D);
    return quotient;
}

// ύψωση μιγαδικού σε πραγματική δύναμη: num1 , degree => num1 ^ degree
complex_number complex_power(const complex_number element, const int degree){
    complex_number total;
    total.real = 1;
    total.imaginary = 0;
    for( int i = 0 ; i < degree; i++)
    {
        total = multiply( total , element);
    }
    return total;
}

// απόλυτη τιμή μιγαδικού αριθμού: num1 ( a + bi ) => sqrt( a^2 + b^2) 
double complex_abs_value(const complex_number num )
{
    double abs_value = sqrt( pow( num.real, 2 ) + pow( num.imaginary, 2 ) );
    return abs_value;
}
