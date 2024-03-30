//Name: Ethyn Nguyen
//ID: 1001930354
//Omega: gcc -o lab2 lab2.c -std=c99
//Run: ./lab2

//Program Description: 
//C program to take n files containing strings in ascending order (no duplicates within a file) and produce a 
//file out.dat containing a line for each string (in ascending order) with the number of files (k) containing the string.

//Program will be driven by a file in.dat:

//The first line will contain the value for n.
//Each of the remaining n lines will contain a simple file name, i.e. there will not be a directory path.
//Each of the n files will contain at least one string. The strings will consist of no more than 50 letters and digits

//Uses three tables - words, heap, and in_files.
//words[i] corresponds to file pointer in in_files[i]
//Program is to perform exactly one “heap assisted” merge of all n files simultaneously.

#include "lab2.h" //Must include in order for program to run

int main() {
    int i;

    //Opening input file for reading and output file for writing
    FILE *fp = fopen("in.dat", "r");
    FILE *outfp = fopen("out.dat", "w+");

    //If input file cannot be found, print error message and terminate program
    if (fp == NULL) {
        printf("Error opening file. Need file titled \"in.dat\". The first line will contain the value for n.\n"
                "Each of the remaining n lines will contain a simple file name, i.e. there will not be a directory path.\n"
                "Each of the n files will contain at least one string. The strings will consist of no more than 50 letters and digits.\n");
        return -1;
    }

    char buffer[MAX];
    int size = 0; //size of heap
    int counter = 1; //count of instances of string in (k) files
    int n;

    fscanf(fp, "%d", &n); //value n is taken from first line of input file 

    FILE *in_files[n];   //n pointers-to-files. 
    for (i = 0; i < n; i++) { //Loop to open n input files in a File pointer array for reading
        char filename[100];
        fscanf(fp, "%s", filename);
        in_files[i] = fopen(filename, "r"); //Opening input file for reading
        if (in_files[i] == NULL) {
            printf("Error opening file. Need \"n\" files in \"in.dat\" titled with scheme \"in[i].dat\". EX: in0.dat, in1.dat, in2.dat, etc.\n");
            return -1;
        }
    }

    fclose(fp);

    char words[n][MAX];
    char heap[n][MAX];

    char *previous_word = calloc(MAX, sizeof(char));

    //Initialize the heap with the first string from each file.
    for (i = 0; i < n; i++) {
        fscanf(in_files[i], "%s", buffer);
        strcpy(words[i], buffer);
        insertnode(heap, &size, buffer);
    }

    i = 0;
    while (size > 0) { //While heap is not empty
        while (i < n) { //Loop that goes through words[i] until words[i] is equal to the root of the heap
            if (strcmp(words[i], heap[0]) == 0) {
                if (fscanf(in_files[i], "%s", buffer) != EOF) { //If in_files[i] has a word to add to the heap
                    deleteRoot(heap, &size, &previous_word);
                    strcpy(words[i], buffer);
                    insertnode(heap, &size, buffer);

                    if (strcmp(previous_word, heap[0]) == 0) { //If the minimum string is the same as the previous minimum
                        counter++;
                    }
                    else { //if the minimum string is different from the previous minimum
                        fprintf(outfp,"%s %d\n", previous_word, counter);
                        counter = 1;
                    }

                    break;
                }
                else { //If in_files[i] has no word to add to the heap
                    deleteRoot(heap, &size, &previous_word);

                    strcpy(words[i], "~");

                    if (strcmp(previous_word, heap[0]) == 0) { //If the minimum string is the same as the previous minimum
                        counter++;
                    }
                    else { //if the minimum string is different from the previous minimum
                        fprintf(outfp,"%s %d\n", previous_word, counter);
                        counter = 1;
                    }

                    break;
                }
            }
            i++;
        }
        i = 0; //Restart loop
    }

    fclose(outfp);
    freeMemory(in_files, previous_word, n);

    return 0;
}

//Function to swap the position of two elements
void swap(char *a, char *b) {
// Input: char *a which stores an array of chars
//        char *b which stores an array of chars
// Output: Void. Swaps the char* stored at a and b
// Processing: (O(n) time).

    if (strcmp(a, b) == 0)
        return;

    char temp[MAX];

    strcpy(temp, a);
    strcpy(a, b);
    strcpy(b, temp);
}

// // Function to heapify a subtree rooted with node i
// // which is an index in arr[].
// // n is size of heap
void heapify(char arr[][MAX], int n, int i) {
// Input: char arr[][MAX] with n elements which stores the heap of chr* with max length MAX for each char*
//        int i which is the index at which to heapify from
// Output: Void. Converts arr to a min heap
// Processing: (O(log n) time).

    int smallest = i; // Initialize smallest as root
    int left = 2 * i + 1; // left = 2*i + 1
    int right = 2 * i + 2; // right = 2*i + 2

    // If left child is larger than root
    if (left < n && strcmp(arr[left], arr[smallest]) < 0)
        smallest = left;
 
    // If right child is larger than smallest so far
    if (right < n && strcmp(arr[right], arr[smallest]) < 0)
        smallest = right;
 
    // If smallest is not root
    if (smallest != i) {
        swap(arr[i], arr[smallest]);
 
        // Recursively heapify the affected sub-tree
        heapify(arr, n, smallest);
    }
}

//Function to insert an element into the heap and do the necessary sorting
void insertnode(char arr[][MAX], int *n, char key[MAX]) {
// Input: char arr[][MAX] with *n elements which stores the heap of chr* with max length MAX for each char*
//        char key[MAX] which stores the new element to insert into the heap
// Output: Void. Inserts key into the heap and does the necessary sorting
// Processing: (O(log n) time).

    // Increase the size of Heap by 1
    *n = *n + 1;
 
    // Insert the element at end of Heap
    strcpy(arr[*n - 1], key);

    // Heapify the new node following a
    // Bottom-up approach and
    // Re-sort the heap array in ascending order as a min heap
    for (int i = *n / 2 - 1; i >= 0; i--)
         heapify(arr, *n, i);
}

//Function to delete the root from Heap
void deleteRoot(char arr[][MAX], int *n, char **root) {
// Input: char arr[][MAX] with *n elements which stores the heap of chr* with max length MAX for each char*
//        char **root which stores the root element before sorting is done
// Output: Void. Removes the root from the heap and does the necessary sorting
// Processing: (O(log n) time).

    char lastElement[MAX];

    // Get the last element
    strcpy(lastElement, arr[*n - 1]);
    strcpy(*root, arr[0]);

    // Replace root with last element
    strcpy(arr[0], lastElement);
    strcpy(arr[*n - 1], "~");

    if (*n == 0)
        return;
    
    // Decrease size of heap by 1
    *n = *n - 1;

    // Re-sort the heap array in ascending order as a min heap
    heapify(arr, *n, 0);
}

//Fucntion to free the memory of previous_word and close the file pointers in in_files
void freeMemory(FILE *in_files[], char *previous_word, int n) {
// Input: FILE *in_files[] which stores n pointers-to-files
//        char **previous_word which stores the root element
//        from before inserting an element or removing the root from the heap
// Output: Void. Frees the memory of previous_word and closes the file pointers in in_files
// Processing: (O(n) time).

    free(previous_word);

    for (int i = 0; i < n; i++) {
        fclose(in_files[i]);
    }
}
