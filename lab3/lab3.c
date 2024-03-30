// Name: Ethyn Nguyen
// ID: 1001930354
// Omega: gcc -o lab3 lab3.c -std=c99
// Run: ./lab3
//      ./lab3 < lab3.a.dat
//      ./lab3 < lab3.b.dat
//      ./lab3 < lab3.c.dat
//      ./lab3 < lab3.d.dat
//      ./lab3 < lab3.e.dat
//      ./lab3 < lab3.f.dat

// Program Description: 
// C program that uses dynamic programming to determine two separate
// subsequences of the input such that the first subsequence sums to the first target value and the second
// subsequence sums to the second target value.

// Input should be read from standard input.

// The first line of the input will give n, the length of the sequence, along with the two target values.
// Each of the remaining input lines will include one sequence value.
// All values will be positive integers.

// Program should echo the target values and the input sequence.
// If a problem instance has a solution, each of the two subsequences should be output.
// Each value is to be preceded by its index in the input.
// A message should be provided for instances without a solution.

// Uses two tables - S and C.
// S[i] corresponds to input set
// C[i][j] corresponds to cost table

#include "lab3.h" // Must include in order for program to run

int main() {
    int n; // Size of input set
    int first_sum; // First target sum
    int second_sum; // Second target sum
    int *S; // Input set
    int **C; // Cost table

    readInput(&n, &first_sum, &second_sum, &S, &C);

    subsetSumRow(n, first_sum, S, C);
    subsetSumColumn(n, second_sum, S, C);
    subsetSumGrid(n, first_sum, second_sum, S, C);

    writeOutput(n, first_sum, second_sum, S, C);

    freeMemory(S, C, first_sum);
}

// Loads all input and allocates space for input set and cost table
void readInput(int *n, int *first_sum, int *second_sum, int**S, int***C) {    
// Input: S* with *n + 1 elements (S[*n + 1]) which stores the values of the input set
//        C*** with row length *first_sum + 1 and column length *second_sum + 1 (C[*first_sum + 1][*second_sum + 1]) which stores the values of the cost table
// Output: Void. Loads input and allocates S and C
// Processing: (O(n) time).

    int i;

    scanf("%d", n);
    scanf("%d", first_sum);
    scanf("%d", second_sum);

    *S = (int*) malloc((*n + 1) * sizeof(int));

    *C = (int**) malloc((*first_sum + 1) * sizeof(int*));

    for (i = 0; i < *first_sum + 1; i++) {
        (*C)[i] = (int*) malloc((*second_sum + 1) * sizeof(int));
    }

    if (!(*S) || !(*C)) {
        printf("malloc failed %d\n",__LINE__);
        exit(0);
    }

    (*S)[0] = 0; // Sentinel zero

    for (i = 1; i <= *n; i++)
        scanf("%d", *S + i);
}

// Generates subset sum for each row in the first column of Cost function (C[i][0])
void subsetSumRow(int n, int first_sum, int*S, int**C) {
// Input: int first_sum which stores the first target sum
//        S* with *n + 1 elements (S[*n + 1]) which stores the values of the input set
//        C*** with row length *first_sum + 1 and column length *second_sum + 1 (C[*first_sum + 1][*second_sum + 1]) which stores the values of the cost table
// Output: Void. Generates and stores subset sum in first column of cost function (C[i][0])
// Processing: (O(n * first_sum) time).

    int i, potentialSum, leftover;

    // Initialize table for DP
    C[0][0] = 0; // DP base case

    // For each potential sum, determine the smallest index such
    // that its input value is in a subset to achieve that sum.

    for (potentialSum = 1; potentialSum <= first_sum; potentialSum++) {
        for (i = 1; i <= n; i++) {
            leftover = potentialSum - S[i];             // To be achieved with other values
            if (leftover >=0 && C[leftover][0] < i)     // Possible to have a solution
                break;                                  // Indices are included in
        }                                               // ascending order.
        C[potentialSum][0] = i;
    }
}

// Generates subset sum for each column in the first row of Cost function (C[0][j])
void subsetSumColumn(int n, int second_sum, int*S, int**C) {
// Input: int second_sum which stores the second target sum
//        S* with *n + 1 elements (S[*n + 1]) which stores the values of the input set
//        C*** with row length *first_sum + 1 and column length *second_sum + 1 (C[*first_sum + 1][*second_sum + 1]) which stores the values of the cost table
// Output: Void. Generates and stores subset sum in first row of cost function (C[0][j])
// Processing: (O(n * second_sum) time).

    int j, potentialSum, leftover;

    // Initialize table for DP
    C[0][0] = 0; // DP base case

    // For each potential sum, determine the smallest index such
    // that its input value is in a subset to achieve that sum.

    for (potentialSum = 1; potentialSum <= second_sum; potentialSum++) {
        for (j = 1; j <= n; j++) {
            leftover = potentialSum - S[j];            // To be achieved with other values
            if (leftover >=0 && C[0][leftover] < j)    // Possible to have a solution
                break;                                 // Indices are included in
        }                                              // ascending order.
        C[0][potentialSum] = j;
    }
}

// Generates subset sum for iterated values i and j in Cost function (C[i][j])
void subsetSumGrid(int n, int first_sum, int second_sum, int*S, int**C) {
// Input: int first_sum which stores the first target sum
//        int second_sum which stores the second target sum
//        S* with *n + 1 elements (S[*n + 1]) which stores the values of the input set
//        C*** with row length *first_sum + 1 and column length *second_sum + 1 (C[*first_sum + 1][*second_sum + 1]) which stores the values of the cost table
// Output: Void. Generates and stores subset sum in iterated values i and j in Cost function (C[i][j])
// Processing: (O(n * first_sum * second_sum) time).

    int i, j, k, leftover_1, leftover_2;

    // Initialize table for DP
    C[0][0] = 0; // DP base case

    // For each potential sum, determine the smallest index such
    // that its input value is in a subset to achieve that sum.

    for (i = 1; i <= first_sum; i++) { // Iterate over rows
        for (j = 1; j <= second_sum; j++) { // Iterate over columns
            for (k = 1; k <= n; k++) { // Iterate over n elements in input table
                leftover_1 = i - S[k]; // C(i - S[k])
                leftover_2 = j - S[k]; // C(j - S[k])        
                if (leftover_1 >= 0 && C[leftover_1][j] < k)
                    break;
                if (leftover_2 >= 0 && C[i][leftover_2] < k)
                    break;
            }
            C[i][j] = k;                            
        }                                           
    }
}

// Prints the resulting subset sums of the first and second target sums to the console
void writeOutput(int n, int first_sum, int second_sum, int*S, int**C) {
// Input: int first_sum which stores the first target sum
//        int second_sum which stores the second target sum
//        S* with *n + 1 elements (S[*n + 1]) which stores the values of the input set
//        C*** with row length *first_sum + 1 and column length *second_sum + 1 (C[*first_sum + 1][*second_sum + 1]) which stores the values of the cost table
// Output: Void. Outputs the backtrace of the first and second target sums
// Processing: (O(n) time).

    int i, j = 0;

    printf("Targets are %d and %d\n", first_sum, second_sum);
    
    // Output the input set
    printf("  i   S\n");
    printf("-------\n");
    for (i = 1; i <= n; i++)
        printf("%3d %3d\n", i, S[i]);

    // Output the backtrace
    if (C[first_sum][second_sum] == n + 1) { // If no solution is viable for both target sums
        printf("No solution\n");
        return;
    }
    else {        
        int i = first_sum;
        int j = second_sum;

        int first_sum_list_size = 0;
        int first_sum_list_index[1000];
        int first_sum_list[1000];

        int second_sum_list_size = 0;
        int second_sum_list_index[1000];
        int second_sum_list[1000];

        while (i > 0 || j > 0) { // While cost function is not at sentinel zero
            if (i - S[C[i][j]] >= 0 && C[i][j] > C[i - S[C[i][j]]][j]) {
                first_sum_list_index[first_sum_list_size] = C[i][j];
                first_sum_list[first_sum_list_size] = S[C[i][j]];
                first_sum_list_size++;

                i -= S[C[i][j]];
                continue;
            }
            if (j - S[C[i][j]] >= 0 && C[i][j] > C[i][j - S[C[i][j]]]) {
                second_sum_list_index[second_sum_list_size] = C[i][j];
                second_sum_list[second_sum_list_size] = S[C[i][j]];
                second_sum_list_size++;

                j -= S[C[i][j]];
                continue;
            }
        }

        printf("subsequence for %d:\n", first_sum);
        for (i = first_sum_list_size - 1; i >= 0; i--) {
            printf("%3d %3d\n", first_sum_list_index[i], first_sum_list[i]);
        }

        printf("subsequence for %d:\n", second_sum);
        for (j = second_sum_list_size - 1; j >= 0; j--) {
            printf("%3d %3d\n", second_sum_list_index[j], second_sum_list[j]);
        }
    }
}

// Frees the allocated memory of S and C
void freeMemory(int *S, int **C, int first_sum) {
// Input: int first_sum which stores the first target sum
//        S* with *n + 1 elements (S[*n + 1]) which stores the values of the input set
//        C*** with row length *first_sum + 1 and column length *second_sum + 1 (C[*first_sum + 1][*second_sum + 1]) which stores the values of the cost table
// Output: Void. Frees the memory of S and C
// Processing: (O(n) time).
 
    free(S);
    
    for (int i = 0; i < first_sum + 1; i++)
        free((C)[i]);

    free(C);
}

// Prints the cost function as a grid. Used for testing and verification
void printCost(int first_sum, int second_sum, int **C) {
// Input: int first_sum which stores the first target sum
//        int second_sum which stores the second target sum
//        C*** with row length *first_sum + 1 and column length *second_sum + 1 (C[*first_sum + 1][*second_sum + 1]) which stores the values of the cost table
// Output: Void. Prints the cost function as a grid to the console
// Processing: (O(first_sum * second_sum) time).

    for (int i = 0; i <= first_sum; i++) {
        for (int j = 0; j <= second_sum; j++)
            printf("%d ", C[i][j]);
            
        printf("\n");
    }
}
