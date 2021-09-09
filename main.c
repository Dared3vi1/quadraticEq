#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>

void printEq(double const* a, double const* b, double const* c, FILE* log) {
    fprintf(log, "The equation has the following form:\n");
    fprintf(log, "%.2fx^2", *a);
    if (*b < 0) {
        fprintf(log, " - %.2fx", -(*b));
    } else {
        fprintf(log, " + %.2fx", *b);
    }
    if (*c < 0) {
        fprintf(log, " - %.2f", -(*c));
    } else {
        fprintf(log, " + %.2f", *c);
    }
    fprintf(log, " = 0\n");
}

void collectResults (double const* x1, double const* x2, int const* result, FILE* log) {
    switch(*result) {
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
    printEq(a, b, c, log);

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

    // If discriminant < 0 => no solutions
    if (isnan(discriminant_sqrt)) {
        fprintf(log, "discriminant < 0\n");
        return 0;
    }
    fprintf(log, "discriminant = %.2f\n", discriminant_sqrt);

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

int calculate() {
    double parameters[] = {0, 0, 0};
    double x1 = 0, x2 = 0;
    fprintf(stdout, "Enter three parameters a, b, c (separated by space)\n");
    fprintf(stdout, "for an equation: ax^2 + bx + c\n");
    char inputStr[255];
    char* ptrEnd;
    for (int i = 0; i < 3; i++) {
        scanf("%s", inputStr);
        parameters[i] = strtod(inputStr, &ptrEnd);
        if (inputStr == ptrEnd) {
            fprintf(stderr, "There is invalid parameter!!!");
            return -1;
        }
    }
    int result = solveQuadraticEq(&(parameters[0]), &(parameters[1]), &(parameters[2]), &x1, &x2, stdout);
    collectResults(&x1, &x2, &result, stdout);
    return 0;
}

void doTest (double a, double b, double c, double x1_r, double x2_r,
             FILE* log, int result_r, int* i) {
    fprintf(log, "Test %d: \n", *i);
    double x1 = 0, x2 = 0;
    int result = solveQuadraticEq(&a, &b, &c, &x1, &x2, log);
    collectResults(&x1, &x2, &result, log);
    assert(x1 == x1_r && x2 == x2_r && result == result_r);
    fprintf(stdout, "Test#%d passed\n", *i);
    *i = *i + 1;
    fprintf(log, "\n\n");
}

void test() {
    double x1 = 0, x2 = 0;
    int i = 1;

    fprintf(stdout, "Test mode running... \n");
    FILE* log;
    log = fopen("../log.txt", "w+");

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

    fprintf(stdout, "\nCheck log.txt file\n");

    fclose(log);


}



int main (int argc, char *argv[]) {
    if (argc == 2 && strcmp(argv[1], "-test") == 0) {
        test();
        return 0;
    } else if (argc == 1) {
        int res = calculate();
        if (res == -1) {
            return 1;
        }
        return 0;
    } else {
        fprintf(stderr, "Invalid terminal input! \nIf you want to run tests add -test flag\n");
        return 1;
    }
}
