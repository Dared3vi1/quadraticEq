#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <math.h>



void collectResults (double const* x1, double const* x2, const int result, FILE* log) {
    switch(result) {
        case 0:
            fprintf(log, "No solutions!\n");
            break;
        case 1:
            fprintf(log, "There are two coinciding solutions: \nx1 = x2 = %.2f\n", *x1);
            break;
        case 2:
            fprintf(log, "There are two solutions: \nx1 = %.2f, x2 = %.2f\n", *x1, *x2);
            break;
        case 3:
            fprintf(log, "Linear equation! \nThere is one solution: \nx1 = %.2f\n", *x1);
            break;
        case 4:
            fprintf(log, "Endless number of solutions!\n");
            break;
        default:
            fprintf(log, "Unreachable!?\n");
            break;
    }

}

int solveQuadraticEq(double const* a, double const* b, double const* c, double* x1, double* x2, FILE* log) {

    // Check input parameters
    if (a == NULL || b == NULL || c == NULL || x1 == NULL || x2 == NULL) {
        fprintf(stderr, "One of the input parameters is NULL!\n");
        return -1;
    }
    if (x1 == x2) {
        fprintf(stderr, "x1 and x2 has the same memory address!\n");
        return -1;
    }

    // Print eq form to log;
    fprintf(log, "The equation has the following form: \n%.2fx^2 + %.2fx + %.2f = 0 \n", *a, *b, *c);

    // Check if equation is linear
    if (*a == 0) {
        if (*b == 0) {
            if (*c == 0) {
                return 4;
            } else {
                return 0;
            }
        }
        *x1 = -(*c) / (*b);
        return 3;
    }

    // Calculate discriminant
    double discriminant_sqrt = sqrt((*b) * (*b) - 4 * (*a) * (*c));
    fprintf(log, "discriminant = %.2f\n", discriminant_sqrt);

    // If discriminant < 0 => no solutions
    if (isnan(discriminant_sqrt)) {
        return 0;
    }

    // If discriminant = 0 => two coinciding solutions
    if (discriminant_sqrt == 0) {
        *x1 = *x2 = -(*b) / (2 * (*a));
        return 1;
    }

    // If discriminant > 0 => two solutions
    *x1 = (-(*b) + discriminant_sqrt) / (2 * (*a));
    *x2 = (-(*b) - discriminant_sqrt) / (2 * (*a));

    return 2;


}

void doTest (double a, double b, double c, double x1_r, double x2_r,
             FILE* log, int result_r, int* i) {
    fprintf(log, "Test %d: \n", *i);
    double x1 = 0, x2 = 0;
    int result = solveQuadraticEq(&a, &b, &c, &x1, &x2, log);
    collectResults(&x1, &x2, result, log);
    assert(x1 == x1_r && x2 == x2_r && result == result_r);
    fprintf(log, "Test#%d passed\n", *i);
    *i = *i + 1;
    fprintf(log, "\n\n");
}

void test(FILE* log) {
    double x1 = 0, x2 = 0;
    int i = 1;

    // Test 1
    doTest(2, -11, -21, 7, -1.5, log, 2, &i);

    // Test 2
    doTest(1, 0, -4, 2, -2, log, 2, &i);

    // Test 3
    doTest(1, 1, -12, 3, -4, log, 2, &i);

    // Test 4
    doTest(0, 1, 1, -1, x2, log, 3, &i);

    // Test 5
    doTest(0, 1, 0, 0, x2, log, 3, &i);

    // Test 6
    doTest(0, 0, 1, x1, x2, log, 0, &i);

    // Test 7
    doTest(0, 0, 0, x1, x2, log, 4, &i);

    // Test 8
    doTest(1, -5, 0, 5, 0, log, 2, &i);

    // Test 9
    doTest(1, 6, 5, -1, -5, log, 2, &i);

    // Test 10
    doTest(1, -6, 9, 3, 3, log, 1, &i);


}

void printResult(double const* a, double const* b, double const* c,
                 double const* x1, double const* x2, const int result) {
    printf("The equation has the following form:\n");
    printf("%.2fx^2", *a);
    if (*b < 0) {
        printf(" - %.2fx", -(*b));
    } else {
        printf(" + %.2fx", *b);
    }
    if (*c < 0) {
        printf(" - %.2f", -(*c));
    } else {
        printf(" + %.2f", *c);
    }
    printf(" = 0\n");
    collectResults (x1, x2, result, stdout);

}


int main () {
    FILE* log;
    log = fopen("../log.txt", "w+");

    double a = 1, b = -2, c = 1;
    double x1 = 0, x2 = 0;

    //Results of tests are in log.txt
    test(log);

    int result = solveQuadraticEq(&a, &b, &c, &x1, &x2, log);
    printResult(&a, &b, &c, &x1, &x2, result);
    fclose(log);

    return 0;
}
