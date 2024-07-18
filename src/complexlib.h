typedef struct{
    double real;
    double imaginary;
} complex_number;

complex_number sum(const complex_number num1, const complex_number num2);

complex_number subtract(const complex_number num1, const complex_number num2);

complex_number multiply(const complex_number num1, const complex_number num2);

complex_number divide(const complex_number num1, const complex_number num2);

complex_number complex_power(const complex_number element, const int degree);

double complex_abs_value(const complex_number num );
