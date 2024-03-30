#ifndef LAB1_H
#define LAB1_H

#include <stdio.h>
#include <stdlib.h>

int input();
int binarySearchLast(int arr[], int n, int key);
int binarySearchFirst(int arr[], int n, int key);
void printMenu();
void printIndex(int n, int count[], int map[]);
void printCount(int n, int index[], int count[]);
void incrementCounter(int n, int *map, int *index, int *count);
void reduceCounter(int n, int *map, int *index, int *count);
void counterValues(int n, int *count);
void freeData(int *map, int *index, int *count);

#endif
