<h1> Newton - Raphson: Εργασία_3 Άσκηση 2η </h1>

<p>Authors: <br> 
  <ul>
      <li>Δημοσθένης Θεοδοσίου (sdi2300051)</li>
      <li>Ευστάθιος Κωνσταντίνου (sdi2300099)</li>
  </ul>
</p>

<p>Αυτή είναι η παρουσίαση της λύσης μας για την δεύτερη άσκηση της 4ης εργασίας στα πλαίσια του μαθήματος <i>Εισαγωγή στον Προγραμματισμό</i> του <i>Εθνικού και Καποδιστριακού Πανεπιστημίου Αθηνών.</i></p>

<h3>Note</h3>

<p>For the program above to be compiled the following commands should be given: </p>

<p>gcc -Ofast -Wall -Wextra -Werror -pedantic -c -o complexlib.o complexlib.c <br>
gcc -Ofast -Wall -Wextra -Werror -pedantic -c -o fractal.o fractal.c <br>
gcc -o fractal complexlib.o fractal.o -lm </p>

Then for execution given a file input with the polynomial you should type: 

./fractal input -g output.bmp

<h2>Τμήμα_1: Υλοποίηση βασικής αριθμητικής μιγαδικών</h2>
<p>Πρωτού αναλύσουμε τις λειτουργίες του προγράμματος είναι σκόπιμο να περιγράψουμε το 
  βασικό struct που δομεί τον κώδικα, καθώς και τις συναρτήσεις που καθορίζουν την βασική αριθμητική μεταξύ των μιγαδικών οντοτήτων.
</p>
<p>Αναλυτικότερα , όπως ζητήται και από την εκφώνηση της εργσίας για κάθε μιγαδικό αριθμό αποθηκεύουμε το πραγματικό και το μιγαδικό μέρος του ως εξής: </p>

```c
typedef struct{
    double real;
    double imaginary;
} complex_number;
```

<p>Έχοντας λοιπόν την οντότητά μας complex_number καλώς ορισμένη , ήρθε η ώρα να αναλύσουμε μερικές θεμελιώδεις συναρτήσεις που την αξιοποιούν. Συγκεκριμένα έχουμε δημιουργήσει 6 συναρτήσεις αριθμητικής μιγαδικών που επιτελούν
τις ακόλουθες λειτουργίες: Άθροισμα , Αφαίρεση , Γινόμενο , Πηλίκο , Ύψωση σε δύναμη , Υπολογισμός πραγματικής απόλυτης τιμής . </p>

<p>Για τις 4 πρώτες συναρτήσεις , όπου χρησιμοποιούμε 2 μιγαδικούς ( num1 , num2 ) έχουμε ορίσει με εντολές define τα μέλη τους ως εξής: </p>

```c
// Δοθέντων 2 μιγαδικών αριθμών: a + bi , c + di
// A == a, B == b, C == c, D == d 
#define A num1.real
#define B num1.imaginary
#define C num2.real
#define D num2.imaginary
```

<p>Ακολουθούν οι βασικές συναρτήσεις</p>
<ol>
<li> <h3>Συνάρτηση αθροίσματος 2 μιγαδικών αριθμών: complex_number sum( complex_num num_1 , complex_num num_2): </h3></li>

```c
// άθροισμα 2 μιγαδικών αριθμών: num1 , num2 => num1 + num2
complex_number sum(complex_number num1, complex_number num2){
    complex_number total;
    total.real = A + C;
    total.imaginary = B + D;
    return total;
}
```
<li> <h3>Συνάρτηση διαφοράς 2 μιγαδικών αριθμών: complex_number subtract(complex_number num1, complex_number num2): </h3></li>

```c
// διαφορά 2 μιγαδικών αριθμών: num1, num2 => num1 - num2
complex_number subtract(complex_number num1, complex_number num2){
    complex_number diffrence;
    diffrence.real = A - C;
    diffrence.imaginary = B - D;
    return diffrence;
}
```
<li> <h3>Συνάρτηση γινομένου 2 μιγαδικών αριθμών: complex_number multiply(complex_number num1, complex_number num2): </h3></li>

```c
// γινόμενο 2 μιγαδικών αριθμών: num1 , num2 => num1 * num2
complex_number multiply(complex_number num1, complex_number num2){
    complex_number product;
    product.real = A * C - B * D;
    product.imaginary = A * D + B * C;
    return product;
}
```
<li> <h3>Συνάρτηση πηλίκου 2 μιγαδικών αριθμών: complex_number divide(complex_number num1, complex_number num2): </h3></li>

```c
complex_number divide(complex_number num1, complex_number num2){
    complex_number quotient;
    quotient.real = (A * C + B * D) / (C * C + D * D);
    quotient.imaginary = (B * C - A * D) / (C * C + D * D);
    return quotient;
}
```

<p>Στις επόμενες 2 συναρτήσεις θα δουλέψουμε με μεμονωμένους μιγαδικούς αριθμούς, χωρίς να χρησιμοποιούμε τους παραπάνω συμβολισμούς με τα define.</p>

<li> <h3>Συνάρτηση ύψωσης μιγαδικού αριθμού σε πραγματική δύναμη: complex_number complex_power( complex_number element, int degree): </h3></li>

```c
// ύψωση μιγαδικού σε πραγματική δύναμη: num1 , degree => num1 ^ degree
complex_number complex_power( complex_number element, int degree){
    complex_number total;
    total.real = 1;
    total.imaginary = 0;
    for( int i = 0 ; i < degree; i++)
    {
        total = multiply( total , element);
    }
    return total;
}
```
<li> <h3>Συνάρτηση υπολογισμού απόλυτης τιμής μιγαδικού αριθμού: double complex_abs_value( complex_number num ): </h3></li>

```c

// απόλυτη τιμή μιγαδικού αριθμού: num1 ( a + bi ) => sqrt( a^2 + b^2) 
double complex_abs_value( complex_number num ){
    double abs_value = sqrt( pow( num.real, 2 ) + pow( num.imaginary, 2 ) );
    return abs_value;
}
```
  
</ol>

<h2>Τμήμα_2: Οργάνωση σε αρχεία</h2>

<p>Έχοντας πλέον τις παραπάνω λειτουργικές συναρτήσεις στην φαρέτρα μας μπορούμε πλέον να ξεκινήσουμε να κάνουμε πράξεις με μιγαδικούς αριθμούς! Προκειμένου όμως να συμπεριλάβουμε 
  τις συναρτήσεις αυτές που βρίσκονται στο αρχείο complex_lib.c στο fractal.c , θα χρειαστεί να δημιουργήσουμε το αρχείο κεφαλίδας complexlib.h που θα επιτρέψει την σύνδεση των 2 προγούμενων αρχείων.</p>
<p>Παρατείθενται τα περιεχόμενα του complexlib.h: </p>

```c

typedef struct{
    double real;
    double imaginary;
} complex_number;

complex_number sum(complex_number num1, complex_number num2);

complex_number subtract(complex_number num1, complex_number num2);

complex_number multiply(complex_number num1, complex_number num2);

complex_number divide(complex_number num1, complex_number num2);

complex_number complex_power( complex_number element, int degree);

double complex_abs_value( complex_number num );
```


<p>Ακολούθως στην αρχή του αρχείου fractal.c περιλαμβάνεται η ακόλουθη εντολή προς τον προμεταγλωττιστή: </p>

```c
#include "complexlib.h"
```
<h2>Τμήμα_2: Συνάρτηση main και κύριες επαναληπτικές δομές</h2>
<p>Ας ξεκινήσουμε διαβάζοντας τα δεδομένα από το αρχείο, δημιοργώντας το αντίστοιχο ρεύμα επικοινωνίας: </p>

```c
// άνοιγμα ρεύματος επικοινωνίας με αρχείο

FILE *finfo = fopen(argv[1], "r"); 
if(finfo == NULL)
{
  fprintf(stderr, "Error: Failed to open file\n");
  return EXIT_FAILURE;
}
```

<p>Έχοντας ανοίξει επιτυχώς το ρεύμα επικοινωνίας μπορούμε να ξεκινήσουμε διαβάζοντας τον βαθμό του πολυωνύμο και να κάνουμε τον έλεγχο για την ορθότητά του: </p>


```c
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
```
<p>Εάν ο βαθμός υπερβάινει το 10, τότε βρισκόμαστε εκτός προδιαγραφών και τερματίζουμε το πρόγραμμα πρόωρα εκτυπώνοντας και τα κατάλληλα διαγνωστικά μηνύματα. Είναι σημαντικό να θυμηθούμε να κλείσουμε το ρεύμα με το 
αρχείο πρωτού κάνουμε return EXIT_FAILURE == 1.</p>
<p>Τώρα χρειάζεται να αποθηκεύσουμε τους συντελεστές του πολυωνύμου μας. Αυτό θα το κάνουμε δεσμεύοντας δυναμικά ένα πίνακα double degree + 1 όρων (λόγω και της σταθεράς a0): </p>

```c
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
```
<p>Το μόνο που μένει τώρα είναι να διαβάσουμε το εύρος μας και το βήμα που θα ακολουθήσουμε: </p>

```c
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
```

<p>Τώρα εξετάζουμε δύο περιπτώσεις , η πρώτη είναι η εκτύπωση των ριζών και η δεύτερη η δημιουργία του fractal. Ας εξετάσουμε την πρώτη τώρα :</p>

<p>Χάρην readability δημιουργήσαμε ένα άλλο struct ονόματι Result με την ακόλουθη δομή: </p>

```c
typedef struct{
    complex_number number;
    int outcome;
} Result;
```
<p>Για κάθε συνδυασμό στο εύρος, αρκεί να βρούμε το outcome του. Τα πιθανά outcomes είναι COVERGED , ISNAN , INCOMPLETE , τα οποία αντιστοιχούν σε ακέραιες τιμές.</p>

```c
for(double i = min.imaginary ; i <= max.imaginary ; i += step){
            // διάσχιση εύρους κατά το πραγματικό εύρος
            for(double j = min.real ; j <= max.real ; j += step){
                // αρχικοποίση σημείου εκκίνησης
                z.imaginary = i;
                z.real = j;
                res = newton(z, degree, coefficient);
                // έλεγχος αποτελέσματος
                switch(res.outcome){
                    case CONVERGED:
                        printf("%+.2lf %+.2lfi ", res.number.real, res.number.imaginary);
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
```


<p>Μετά το πέρας της διαδικασίας κάνουμε return 0; και τερματίζει το πρόγραμμα.</p>

<p>Τέλος μένει να δούμε τι κάνει αυτή η newton...</p>

```c
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
```
<p>Η συνάρτηση newton σαν λειτουργικότητα πραγματοποιεί ακριβώς τις ίδιες διαδικασίες με την newton της προηγούμενης εργασίας με την διαφορά ότι όλα πραγματοποιούνται σε επίπεδο struct και ότι χρησιμοποιούνται οι συναρτήσεις calculate_f και calculate_derivative.</p>

```c
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
```

```c
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
```

<p></p>

<p>Ας δούμε την άλλη περίπτωση τώρα δηλαδή την δημιουργία του fractal</p>

<p>Αρχικά δημιοργούμε το header της bmp εικόνας καλώντας την συνάρτησ print_header() , της οποίας η λειτουργία είναι τετριμμένη ,αφού χρησιμοποιήσαμε ολόιδιες τεχνικές με αυτές για την προηγούμενη εργασία</p>

```c
/ εκτύπωση header bmp αρχείου
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
```

<p>Στην συνέχεια εκτυπώνουμε μεμονωμένα pixel εφαρμόζοντας πάλι όπως πριν τον αλγόριθμο newton για κάθε συνδυασμό στο εύρος. Μόνο που αυτή την φορά ανάλογα με το αποτέλεσμα εκτυπώνουμε και διαφορετικό χρώμα : </p>

```c
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
                    ans = diff_root_calc( roots, &root_cnt, foo, degree);
                    print_pixel( ans ,fp);
                    break;
            }
 }
```

<p>Στην εργασία δεν υπήρχε κάποια σαφής οδηγία για το πιο χρώμα αντιστοιχεί σε ποιό αποτέλεμσμα για αυτό και εμείς δημιουργήσαμε έναν πίνακα 10 χρωμάτων τύπου pixel όπου pixel ένα struct 3 στοιχείων τύπου (r , g , b)</p>


```c

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
```

<p>Προσοχή! Λόγω little endian τα pixel πρέπει πάντοτε να εκτυπώνονται με ανεστραμμένα τα στοιχεία τους, δηλαδή bgr αντί για rgb. Συνεχίζοντας , μετά απο κάθε αποτέλεσμα του newton έχουμε 3 περιπτώσεις. H πρώτη INCOMPLTE αντιστοιχεί στην εκτύπωση ενός λευκού pixel, δηλαδή του palette[11]. H δεύτερη NAN αντιστοιχεί στην εκτύπωση ενός μαύρου πixel palette[10]. Στην περίπτωση που εν τέλει ο αλγόριθμος κατέληξε σε μία ρίζα αρκεί να ελέγξουμε αν αυτή την ρίζα την έχουμε ξανασυναντήσει ή όχι. Αν ναι εκτυπώνουμε το χρώμα το οποίο είχαμε εκτυπώσει και για το αντίστοιχο πανομοιότυπο προηγούμενο αποτέλεσμα, αν όχι εκτυπώονουμε ένα καινούριο χρώμα! Η τελευταία αυτή διαδικασία πραγματοποιήται με την συνάρτηση diff_root_calc η οποία αποθηκεύει όλες τις διαφορετικές ρίζες που έχουμε συναντήσει μέχρι στιγμής: </p>

```c
int diff_root_calc(complex_number roots[] , int *root_cnt , Result foo, const int degree)
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
```
<p>Μετά την εκτύπωση κάθε σειράς πixel οφείλουμε να εκτυπώσουμε και το ανάλογο padding. Τέλος η συνάρτηση εκτύπωσης pixel λειτουργεί απλούστατα ως εξής: </p>


```c
void print_pixel( int i , FILE *fp)
{
    // εκτύπωση pixel σε μορφή little endian
    // b g r από r g b
    fputc(palette[i].b , fp);
    fputc(palette[i].g , fp);
    fputc(palette[i].r , fp);
}

```




