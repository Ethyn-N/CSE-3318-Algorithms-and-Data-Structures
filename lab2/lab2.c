#include <stdio.h>
#include <string.h>

//structure to store word and its count
struct words
{
    char word[50];
    int count;
};

//Function to swap the position of two elements
void swap(struct words *a, struct words *b) {
    char temp[50];

    strcpy(temp, a->word);
    strcpy(a->word, b->word);
    strcpy(b->word, temp);

    int t = a->count;

    a->count = b->count;
    b->count = t;
}

//define the function heapify() to perform
//only one "heap assisted" merge of all
//n files simultaneously
void heapify(struct words arr[200], int N, int i) {
    // Find largest among root, left child and right child
 
    // Initialize largest as root
    int largest = i;

    int left = 2 * i + 1;
    int right = 2 * i + 2;

    // If left child is larger than root
    if (left < N && strcmp(arr[left].word, arr[largest].word) > 0)
        largest = left;
    //use if condition to compare the strings
    if (right < N && strcmp(arr[right].word, arr[largest].word) > 0)
        largest = right;
    //use if condition
    //to check for the largest string
    if (largest != i) {
        //swap the arrays
        swap(&arr[i], &arr[largest]);
        //heapify the arrays
        heapify(arr, N, largest);
    }
}

// Main function to do heap sort
// N = size of heap
void heapSort(struct words arr[200], int N) {
    int i;

    // Build max heap
    for (i = N/2 - 1; i >= 0; i--) {
        heapify(arr, N, i);
    }

    // Heap sort
    for (i = N-1; i >= 0; i--) {
        swap(&arr[0], &arr[i]);

        // Heapify root element to get highest element at
        // root again
        heapify(arr,i,0);
    }
}

int main() {
    int i;

    FILE *fp = fopen("in.dat", "r");
    FILE *outfp = fopen("out.dat", "w+");

    if (fp == NULL) {
        printf("Error opening file\n");
        return 0;
    }

    char buffer[100];
    int counter = 0;
    int N;

    fscanf(fp, "%d", &N);
    struct words data[N * 50];

    for (int fs = 0; fs<N; fs++) {
        fscanf(fp, "%s", buffer);
        FILE *fp2 = fopen(buffer, "r");
        char word[50];

        while (!feof(fp2)) {
            fscanf(fp2, "%s", word);
            int found = 0;

            for (i = 0; i < counter; i++) {
                if (strcmp(data[i].word, word) == 0) {
                    found = 1;
                    data[i].count++;
                }
            }

            if (found == 0) {
                strcpy(data[counter].word, word);
                data[counter].count=1;
                counter++;
            }
        }
        fclose(fp2);
    }
    
    fclose(fp);
    heapSort(data, counter);

    for (i = 0; i < counter; i++) {
        fprintf(outfp, "%s %d\n", data[i].word, data[i].count);
        printf("%s %d\n", data[i].word, data[i].count);
    }

    return 0;
}