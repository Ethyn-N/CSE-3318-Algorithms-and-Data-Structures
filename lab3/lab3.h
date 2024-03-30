#ifndef LAB3_H
#define LAB3_H

#include <stdio.h>
#include <stdlib.h>

void readInput(int *n, int *first_sum, int *second_sum, int**S, int***C);
void subsetSumRow(int n, int first_sum, int*S, int**C);
void subsetSumColumn(int n, int second_sum, int*S, int**C);
void subsetSumGrid(int n, int first_sum, int second_sum, int*S, int**C);
void writeOutput(int n, int first_sum, int second_sum, int*S, int**C);
void freeMemory(int *S, int **C, int first_sum);
void printCost(int first_sum, int second_sum, int **C);

#endif
