//Name: Ethyn Nguyen
//ID: 1001930354
//Omega: gcc -o lab1 lab1.c -std=c99
//Run: ./lab1
//     ./lab1 < a.dat
//     ./lab1 < b.dat

//Program Description: 
//C program to maintain n counters indexed by 0 .. n-1. 
//n will be the first input value and all counters are initially valued as zero.
//Uses indirection, maps/permutations, and swapping to ensure count is maintained when 
//incrementing/reducing a count value by 1.

//Dynamically allocates three tables - map, index, and count.
//index[i] indicates which of the n counters has its value presently stored as count[i].
//map[i] is used to find the counter with index i, i.e. it is always true that index[map[i]]==i.

#include "lab1.h" //Must include in order for program to run

int main()
{
  int n, choice;

  n = input(); //input n from user

  //Allocating memory to int pointer arrays map, index, and count with size n
  int *map = calloc(n, sizeof(int));
  int *index = calloc(n, sizeof(int));
  int *count = calloc(n, sizeof(int));

  //initializing all the values of counter to 0
  for(int i = 0; i < n; i++) {
    map[i] = i;
    index[i] = i;
    count[i] = 0;
  } 
  
  choice = input(); //User input that determines which operation to run
  
  //Loop that continues to run until choice = 0 or choice is not operations 1-5.
  while(1)
  {
    switch (choice) 
    {
        case 0: //terminate execution
            freeData(map, index, count);
            exit(0);
            break;
        case 1: //Operation 1
            printIndex(n, count, map);
            choice = input();
            break;
        case 2: //Operation 2
            printCount(n, index, count);
            choice = input();
            break;
        case 3: //Operation 3
            incrementCounter(n, map, index, count);
            choice = input();
            break;
        case 4: //Operation 4
            reduceCounter(n, map, index, count);
            choice = input();
            break;
        case 5: //Operation 5
            counterValues(n, count);
            choice = input();
            break;
        default:
            printf("Default\n");
            freeData(map, index, count);
            exit(-1);
    }
  }
}

//Returns value using scanf. Makes sure value is valid
int input() {
  // Input: None
  // Output: Returns int number using scanf. Will not return number until input is valid.
  // Processing: (O(n) time)

    int number, args;
    char x;
    do
    {
        args = scanf("%d", &number);
        if (args < 1) //If scanf is invalid. EX: user inputs char instead of int
       {
           while ((x = getchar()) != EOF && x != '\n');
           printf("\nInvalid choice. Try again: ");
       }   
    } while (args < 1); //While input is invalid, try again until an int is accepted
    
    return (number);
}

int binarySearchLast(int arr[], int n, int key) {
  // Input: int array arr[] with n elements in ascending order.
  //        int key to find.
  // Output: Returns subscript of the last arr[] element <= key.
  //         Returns -1 if key<arr[0].
  // Processing: Binary search.

    int low, high, mid;
    low = 0;
    high = n-1;
  // subscripts between low and high are in search range.
  // size of range halves in each iteration.
  // When low>high, low==high+1 and arr[high]<=key and arr[low]>key.

    while (low <= high) {
      mid = (low + high)/2;
      if (arr[mid] <= key) {
        low = mid + 1;
      }
      else {
        high = mid - 1;
      }
    }
    return high;
}

int binarySearchFirst(int arr[], int n, int key) {
  // Input: int array arr[] with n elements in ascending order.
  //        int key to find.
  // Output: Returns subscript of the first arr[] element >= key.
  //         Returns n if key>arr[n-1].
  // Processing: Binary search.

    int low, high, mid;
    low = 0;
    high = n-1;
  // Subscripts between low and high are in search range.
  // Size of range halves in each iteration.
  // When low>high, low==high+1 and arr[high]<key and arr[low]>=key.

    while (low <= high)
    {
      mid = (low + high)/2;
      if (arr[mid] < key)
        low = mid + 1;
      else
        high = mid - 1;
    }
    return low;
}

//Prints the menu. Used for testing.
void printMenu() {
    printf("\n0 - terminate\n");
    printf("1 - print the counters in ascending index\n");
    printf("2 - print the counters in ascending counter value order as (index, count) pairs.\n");
    printf("3 i - add one to the counter indexed by i. (O(log n) time\n");
    printf("4 i - subtract one from the counter indexed by i. (O(log n) time\n");
    printf("5 i j - determine the number of counters whose values are no smaller than i and no larger than j. \n");

    printf("\nSelect the appropriate option: ");
}

//Operation 1: print the counters in ascending index value order as (index, count) pairs. (O(n) time)
void printIndex(int n, int count[], int map[]) {
  // Input: int array map[] which stores index values for count.
  //        int array count[] with n elements in ascending order.
  // Output: Void. Prints the counters in ascending index value order as (index, count) pairs.
  // Processing: (O(n) time).

    printf("print by index\n");
    for(int i = 0; i < n; i++) {
        printf("%-3d %-3d\n", i, count[map[i]]);
    }
    printf("-------\n");
}

//Operation 2: print the counters in ascending counter value order as (index, count) pairs. (O(n) time)
void printCount(int n, int index[], int count[]) {
  // Input: int array index[] which stores values for index.
  //        int array count[] with n elements in ascending order.
  // Output: Void. Prints the counters in ascending counter value order as (index, count) pairs.
  // Processing: (O(n) time).

    printf("print by count\n");
    for (int i = 0; i < n; i++) { //(O(n) time)
        printf("%-3d %-3d\n", index[i], count[i]);
    }
    printf("-------\n");
}

//Operation 3: Increments the counter and does the necessary swapping of the count, index, and map arrays
void incrementCounter(int n, int *map, int *index, int *count) {
  // Input: int array map[] with n elements which stores index values for count.
  //        int array count[] with n elements in ascending order.
  //        int array index[] with n elements which stores valeus for index
  // Output: Void. Increments the counter and does the necessary swapping of the count, index, and map arrays
  // Processing: O(log n) time).

    //User input
    int p = input();

    //Search for last instance of p in count[] using map[]. O(log n) time)
    int lastCount = binarySearchLast(count, n, count[map[p]]);

    //Swapping count[map[p]] and count[lastCount] and incrementing the count
    int temp = count[map[p]];
    count[map[p]] = count[lastCount];
    count[lastCount] = temp;
    count[lastCount] += 1;

    //temp map swap values using the values of index so that index[map[i]]==i holds true
    int map_p = index[map[p]];
    int map_last = index[lastCount];

    //Swapping index[map[p]] and index[lastCount]
    temp = index[map[p]];
    index[map[p]] = index[lastCount];
    index[lastCount] = temp;

    //Swapping map[map_p] and map[map_last]
    temp = map[map_p];
    map[map_p] = map[map_last];
    map[map_last] = temp;
}

//Operation 4: Reduces the counter and does the necessary swapping of the count, index, and map arrays
void reduceCounter(int n, int *map, int *index, int *count) {
  // Input: int array map[] with n elements which stores index values for count.
  //        int array count[] with n elements in ascending order.
  //        int array index[] with n elements which stores valeus for index
  // Output: Void. Reduces the counter and does the necessary swapping of the count, index, and map arrays
  // Processing: O(log n) time).

    //User input
    int p = input();

    //Search for first instance of p in count[] using map[]. O(log n) time)
    int firstCount = binarySearchFirst(count, n, count[map[p]]);

    //Swapping count[map[p]] and count[firstCount]
    int temp = count[map[p]];
    count[map[p]] = count[firstCount];
    count[firstCount] = temp;
    count[firstCount] -= 1;

    //temp map swap values using the values of index so that index[map[i]]==i holds true
    int map_p = index[map[p]];
    int map_first = index[firstCount];

    //Swapping index[map[p]] and index[firstCount]
    temp = index[map[p]];
    index[map[p]] = index[firstCount];
    index[firstCount] = temp;

    //Swapping map[map_p] and map[map_first]
    temp = map[map_p];
    map[map_p] = map[map_first];
    map[map_first] = temp;
}

//Operation 5: Determines the number of counters whose values are no smaller than i and no larger than j.
void counterValues(int n, int *count) {
  // Input: int array count[] with n elements in ascending order.
  // Output: Void. Prints the number of counters whose values are no smaller than i and no larger than j.
  // Processing: O(log n) time).

    //User input taking in the range of values which to look for in count[]
    int i = input();
    int j = input();

    //Search for first index of i in count[]
    int start = binarySearchFirst(count, n, i);

    //Search for last index of j in count[]
    int end = binarySearchLast(count, n, j);
    
    //Subtracts end and start and adds 1 to find the number of counters with values between i and j.
    int counters = end - start + 1;

    //Prints number of counters valued between i and j
    printf("%d counters valued between %d and %d\n", counters, i, j);
}

//Frees the map, index, and count arrays' memory
void freeData(int *map, int *index, int *count) {
    free(map);
    free(index);
    free(count);
}
