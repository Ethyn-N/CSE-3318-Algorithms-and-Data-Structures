#ifndef LAB2_H
#define LAB2_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 50

void swap(char *a, char *b);
void heapify(char arr[][MAX], int n, int i);
void insertnode(char arr[][MAX], int *n, char key[MAX]);
void deleteRoot(char arr[][MAX], int *n, char **root);
void freeMemory(FILE *in_files[], char *previous_word, int n);

#endif
