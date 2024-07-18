#include "complexlib.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <math.h>

#define EXIT_FAILURE 1
#define CONVERGED 1
#define ISNAN 2
#define INCOMPLETE 3
#define NO 0
#define YES 1 
#define BYTES_PER_PIXEL 3
#define STANDARD_BPP 24
#define STANDARD_OFFSET 54
#define STANDARD_HEADER_SIZE 54
#define THRESHOLD (1E-6)
#define FILE_SIZE_OFFSET 2
#define NUMBER_OF_COLOUR_PLANES 1
#define STANDARD_BITMAP_SIZE 40

typedef struct{
    complex_number number;
    int outcome;
} Result;

// δημιουργία struct pixel για αναπαράσταση χρωμάτων
// ανεστραμμένη διάταξη r,g,b λόγω little endian
typedef struct{
    unsigned char b;
    unsigned char g;
    unsigned char r;
}pixel;

// δημιουργία πίνακα struct χρώματος
pixel palette[12] = {   { 0 , 0 , 255 } ,       // red
                        { 0 , 255 , 0 } ,       // lime
                        { 255 , 0 , 0 } ,       // blue
                        { 0 , 255 , 255 } ,     // yellow
                        { 255 , 255 , 0 } ,     // cyan
                        { 255 , 0 , 255 } ,     // magenta
                        { 0 , 128 , 255 } ,     // orange
                        { 255 , 51 , 153 } ,    // purple
                        { 0 , 128 , 128 } ,     // olive
                        { 139 , 139 , 0 },      // dark cyan
                        { 0 , 0 , 0},           // black
                        { 255 , 255 , 255 }};   // white         


// δήλωση προτύπων συναρτήσεων

complex_number calculate_f(const double *coefficient, const int degree, const complex_number z);
Result newton(complex_number current, const int degree, const double *coefficients);
complex_number calculate_derivative(const int degree, const double *coefficients, const complex_number z);
void print_pixel( int i ,FILE *fp);
int diff_root_calc(complex_number roots[] , int *root_cnt , Result foo);
int8_t calculatePadding(const int32_t width);
void print_header( FILE *fp, int countreal, int countimaginary);

int main(int argc, char ** argv)
{
    // έλεγχος πλήθους ορισμάτων και εκτύπωση διαγνωστικών μηνυμάτων

    if(argc < 2 || argc > 4)
    { 
        fprintf(stderr, "Error: Program wasnt called properly, usage: ./fractal filename or ./fractal filename -g output.bmp to print fractal\n"); 
        return EXIT_FAILURE;
    }
    int print_factorial = NO;
    if(argc == 4 && strcmp(argv[2], "-g")  == 0){
        print_factorial = YES;
    }

    // άνοιγμα ρεύματος επικοινωνίας με αρχείο
    FILE *finfo = fopen(argv[1], "r"); 
    if(finfo == NULL)
    {
        fprintf(stderr, "Error: Failed to open file\n");
        return EXIT_FAILURE;
    }
    int degree;

    // διάβασμα βαθμού πολυωνύμου και έλεγχος ορθότητας

    if( fscanf(finfo, "%d", &degree) != 1)
    {
        printf("scanf has failed\n");
        if( fclose(finfo) )
        {
            printf("File closing has failed\n");
        }
        return EXIT_FAILURE;
    }
    if(degree > 10)
    {  
        fprintf(stderr, "Error, polynomial degree must be at most 10\n");
        if( fclose(finfo) )
        {
            printf("File closing has failed\n");
        }
        return EXIT_FAILURE;
    }

    // δημιουργία πίνακα συντελεστών πολυωνύμου

    double *coefficient = malloc((degree + 1) * sizeof( *coefficient ));
    if(coefficient == NULL)
    {
        fprintf(stderr, "Error, malloc for coefficients failed\n");
        if( fclose(finfo) )
        {
            printf("File closing has failed\n");
        }
        return EXIT_FAILURE;
    }

    // διάβασμα συντελεστών πολυωνύμου και ορίων αναζήτησης

    for(int i = 0 ; i <= degree ; i++)
    {
        if( fscanf(finfo, " %lf", &coefficient[i]) != 1 )
        {
            printf("scanf has failed\n");
            if( fclose(finfo) )
            {
                printf("File closing has failed\n");
            }
            return EXIT_FAILURE;
        }
    }
    complex_number min, max;
    double step;
    if( fscanf(finfo, "%lf %lf %lf %lf %lf", &min.real, &min.imaginary, &max.real, &max.imaginary, &step) != 5 )
    {
        printf("scanf has failed\n");
        if( fclose(finfo) )
        {
            printf("File closing has failed\n");
        }
        return EXIT_FAILURE;
    } 

    // κλείσιμο ρεύματος επικοινωνίας με το αρχείο

    if( fclose(finfo) )
    {
        printf("File closing has failed\n");
        return EXIT_FAILURE;
    }
    complex_number z;
    Result res;
    // περίπτωση εκτύπωσης μόνο των ριζών
    if(print_factorial == NO){
        // διάσχιση εύρους κατά το μιγαδικό μέρος
        for(double i = min.real ; i <= max.real ; i += step){
            // διάσχιση εύρους κατά το πραγματικό εύρος
            for(double j = min.imaginary ; j <= max.imaginary ; j += step){
                // αρχικοποίση σημείου εκκίνησης
                z.imaginary = j;
                z.real = i;
                res = newton(z, degree, coefficient);
                // έλεγχος αποτελέσματος
                switch(res.outcome){
                    case CONVERGED:
                        printf("%+.2lf%+.2lfi ", res.number.real, res.number.imaginary);
                        break;
                    case ISNAN:
                        printf("nan ");
                        break;
                    case INCOMPLETE:
                        printf("incomplete ");
                        break;
                }
            }
            printf("\n");
        }
    // τερματισμός προγράμματος
    return 0;
    }

    // συνέχεια σε περίπτωση δημιουργίας εικόνας
    int countreal = 0;
    int countimaginary = 0;

    for(double i = min.imaginary ; i <= max.imaginary ; i += step){
        countimaginary++;
    }
    for(double j = min.real ; j <= max.real ; j += step){
        countreal++;
    }
    // αρχικοποίηση πίνακα διαφορετικών ριζών πολυωνύμου
    complex_number roots[10] ={ {0,0}};
    int root_cnt = 0 ,ans;
    // άνοιγμα ρεύματος επικοινωνίας για έξοδο
    FILE *fp = fopen(argv[3] , "w");
    if( fp == NULL )
    {
        printf("File handling has failed\n");
        return EXIT_FAILURE;
    }
    // υπολογισμός padding 
    const int8_t padding = calculatePadding(countreal);

    // εκτύπωση header bmp εικόνας
    print_header(fp, countreal, countimaginary);

    // διάσχιση εύρους κατά το μιγαδικό μέρος
    for(double i = min.imaginary ; i <= max.imaginary ; i += step){
        // διάσχιση εύρους κατά το πραγματικό μέρος
        for(double j = min.real ; j <= max.real ; j += step){
            z.imaginary = i;
            z.real = j;
            res = newton(z, degree, coefficient);
            // εκτύπωση pixel ανάλογα με αποτέλεσμα newton
            switch(res.outcome){
                case INCOMPLETE:
                    // εκτύπωση λευκού σε incomplete περίπτωση
                    print_pixel( 11 , fp);
                    break;
                case ISNAN:
                    // εκτύπωση μαύρου σε nan περίπτωση
                    print_pixel( 10 ,fp); 
                    break;
                case CONVERGED:
                    // εκτύπωση αντίστοιχου pixel με α/α ρίζας                                             
                    ans = diff_root_calc( roots, &root_cnt, res);
                    print_pixel( ans ,fp);
                    break;
            }
        }
        // εκτύπωση padding ανά γραμμή
        for(int i = 0 ; i < padding ; i++)
        {
            fputc('\0' , fp);
        }
    }
    // κλείσιμο ρεύματος επικοινωνίας / τερματισμός προγράμματος

    if( fclose(fp) )
    {
        printf("File handling failed\n");
        return EXIT_FAILURE;
    }
    return 0;
}

// εκτύπωση header bmp αρχείου
void print_header( FILE *fp, int countreal, int countimaginary ){
    char header[STANDARD_HEADER_SIZE] = {0};
    
    int32_t width = countreal;
    int32_t height = countimaginary;

    int8_t padding = calculatePadding(width);
    int32_t picture_size = width * height * BYTES_PER_PIXEL + padding * height;
    int32_t file_size = picture_size + STANDARD_HEADER_SIZE;

    header[0] = 'B';
    header[1] = 'M';

    *(int32_t *)(&header[FILE_SIZE_OFFSET]) = file_size;
    *(int32_t *)(&header[10]) = STANDARD_OFFSET;
    *(int32_t *)(&header[14]) = STANDARD_BITMAP_SIZE;  
    *(int32_t *)(&header[18]) = width;
    *(int32_t *)(&header[22]) = height;
    *(int16_t *)(&header[26]) = NUMBER_OF_COLOUR_PLANES;
    *(int16_t *)(&header[28]) = STANDARD_BPP;
    *(int32_t *)(&header[34]) = picture_size;

    fwrite(header, sizeof(*header), STANDARD_HEADER_SIZE, fp);
    
    return ;
}


// συνάρτηση υπολογισμού f(z) δοθέντως πίνακα συντελεστών, βαθμού και ορίσματος

complex_number calculate_f(const double *coefficient, const int degree, const complex_number z)
{
    // αρχικοποίηση αθροίσματος στο 0

    complex_number summation;
    summation.real = 0 ;
    summation.imaginary = 0 ;
    complex_number element;

    for( int i = 0 ; i <= degree ; i++)
    {
        // υπολογισμός i - οστού όρου στο άθροισμα

        element.real =  coefficient[i];
        element.imaginary = 0 ;
        // προσθήκη στο τελικό άθροισμα
        summation = sum( summation , multiply( complex_power( z, i ) , element));
    }
    // επιστροφή αθροίσματος
    return summation;
}

// υπολογισμος τιμής παραγώγου f'(z)
complex_number calculate_derivative(const int degree, const double *coefficients, const complex_number z){
    complex_number total, foo; 
    // αρχικοποίηση αθροίσματος στο 0 
    total.real = 0;
    total.imaginary = 0;
    foo.imaginary = 0;
    
    for(int i = 0 ; i <= degree ; i++){
        // υπολογισμός i - οστού όρου στο άθροισμα
        foo.real = i * coefficients[i];
        // προσθήκη στο τελικό άθροισμα
        total = sum(total, multiply(foo, complex_power(z, i - 1)));
    }
    // επιστροφή αθροίσματος
    return total;
}


Result newton(complex_number current, const int degree, const double *coefficients){
    complex_number previous, f, derivative;
    int count = 0;
    Result result;

    while(count < 1000){
        // εναλλαγή τωρινής και προηγούμενης τιμής
        previous = current;
        // υπολογισμοί f(previous) και f'(previous)
        f = calculate_f(coefficients, degree, previous);
        derivative = calculate_derivative(degree, coefficients, previous); 
    
        if(derivative.real == 0 && derivative.imaginary == 0)
        {
            // επιστροφή αποτελέσματος ISNAN σε περίπτωση διαίρεσης με το 0
            result.outcome = ISNAN;
            return result;
        }
        // υπολογισμος previous - f(previous) / f'(previous)
        current = subtract(previous, divide(f, derivative));
        // έλεγχος ελάχιστης διαφοράς διαδοχικών βημάτων
        if(complex_abs_value(subtract(current, previous)) < THRESHOLD)
        {
            result.outcome = CONVERGED;
            result.number = current;
            return result;
        }
        // προσαύξηση βήματος
        count++;
    }

    // εκχώρηση αποτελέσματος INCOMPLETE 
    // σε περίπτωση μη τερματισμού παραπάνω
    result.outcome = INCOMPLETE;
    return result;
}

// υπολογισμός πλήθους διαφορετικών ριζών και αντιστοίχισης ρίζας σε χρώμα
int diff_root_calc(complex_number roots[] , int *root_cnt , Result foo)
{
    // αναζήτηση ρίζας σε πίνακα ριζών
    for( int k = 0 ; k < *root_cnt ; k++)
    {
        if( complex_abs_value(subtract(foo.number, roots[k])) < THRESHOLD )
        {
            // επιστροφή α/α ήδη υπάρχωντος ρίζας και αντιστοίχιση σε χρώμα
            return k;
        }
    }
    // σε περίπτωση μη τερματισμού από επάνω
    // αποθήκευση καινούριας ρίζας στον πίνακα ριζών
    roots[*root_cnt] = foo.number;
    
    // προσαύξηση αριθμού ριζών κατά 1
    return (*root_cnt)++;
}

void print_pixel( int i , FILE *fp)
{
    // εκτύπωση pixel σε μορφή little endian
    // b g r από r g b
    fputc(palette[i].b , fp);
    fputc(palette[i].g , fp);
    fputc(palette[i].r , fp);
}

// υπολογισμός padding 
int8_t calculatePadding(const int32_t width){
    int8_t temp = (width * BYTES_PER_PIXEL) % 4;
    return (temp == 0) ? 0 : 4 - temp;
}
