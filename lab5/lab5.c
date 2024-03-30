// Name: Ethyn Nguyen
// ID: 1001930354
// Omega: gcc -o lab5 lab5.c -std=c99 -lm
// Run: ./lab5
//      ./lab5 < lab5fall22.a.dat
//      ./lab5 < lab5fall22.b.dat
//      ./lab5 < lab5fall22.c.dat
//      ./lab5 < lab5fall22.d.dat

// Program Description: 
// C program to determine a minimum spanning forest for a weighted, undirected graph.

// The first line is two integer values: n, the number of vertices, and m, the number of edges.
// The remaining m lines will each contain three values defining an edge: a vertex name (string of no more than 25 characters)
// a different vertex name (another string of no more than 25 characters)
// and an integer value for the weight of the edge.

// While reading the input, new vertex names should be stored in a double hash table along with consecutively assigned
// vertex numbers. In addition, a table of vertex names to allow printing the name for a vertex number will be needed.
// If the input does not include n different vertex names, give a warning and continue. If the input has more than n
// different names, give a disparaging message and stop.

// Print the trees in the forest. If the input graph is connected, there will be only one tree in the final result. Otherwise,
// the edges should be sorted so that the edges in each tree are output together and labeled as such. 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAX_LENGTH 25

struct data {
    char key[MAX_LENGTH];
    int value;
};

struct hashtable_item {
    int flag;
	/*
	 * flag = 0 : data not present
	 * flag = 1 : some data already present
	*/
	struct data *item;
};

struct hashtable_item *array;
int hash_max;
int hash_size;
int prime;

int numVertices, numEdges;
int *parent, *weight, numTrees;

struct edge {
  int tail, head, weight;
};

typedef struct edge edgeType;
edgeType *edgeTab;

// Find root of tree containing x
int find(int x) {
// Input: int x
// Output: int. Root of tree containing x
// Processing: (O(n) time).

    int i, j, root;

    // Find root
    for (i = x; parent[i] != i; i = parent[i]);
    root = i;

    // path compression - make all nodes on path
    // point directly at the root

    for (i = x; parent[i] != i; j = parent[i], parent[i] = root, i = j);
    return root;
}

// i and j must be roots!
void unionFunc(int i, int j) {
// Input: int i and int j (Must both be roots).
// Output: Void. Finds the leader of the roots and decreases the number of trees
// Processing: (O(1) time).

    if (weight[i] > weight[j]) {
        parent[j] = i;
        weight[i] += weight[j];
    }
    else {
        parent[i] = j;
        weight[j] += weight[i];
    }
    numTrees--;
}

// Used in call to qsort() for edgeTabs
int weightAscending(const void* xin, const void* yin) {
// Input: const void* xin, const void* yin
// Output: int. Used in call to qsort() to sort edgeTabs in accending edge weights
// Processing: (O(1) time).

    edgeType *x, *y;

    x = (edgeType*) xin;
    y = (edgeType*) yin;
    return x->weight - y->weight;
}

// Hashcode for hash 1
int hashcode1(char key[MAX_LENGTH]) {
// Input: char key
// Output: int. Generates hashcode value
// Processing: (O(n) time).

    int h1 = 0;
    for(int i = 0; key[i] != '\0'; i++)
        h1 = (h1 + key[i]) % (hash_max);
        
	return h1;
}

// Hashcode for hash 2
int hashcode2(char key[MAX_LENGTH]) {
// Input: char key
// Output: int. Generates hashcode value
// Processing: (O(n) time).

	int h2 = 0;
    for(int i = 0; key[i] != '\0'; i++)
        h2 = (prime - ((h2 + key[i]) % prime));
    
    return h2;
}

// Inserts new element into hashtable
int insert(char key[MAX_LENGTH], int value) {
// Input: char key, int value
// Output: int. Inserts new keys into the hashvalue. Returns selected key's value.
// Processing: (O(n) time).

	int hash1 = hashcode1(key);
	int hash2 = hashcode2(key);
    
	int index = hash1;

	if (hash_size == hash_max) {
		printf("\n Hash Table is full, cannot insert more items \n");
		return -1;
	}

	/* probing through other array elements */
	while (array[index].flag == 1) {
		if (strcmp(array[index].item->key, key) == 0) {
			// printf("\n Key already present, return array value \n");
			return array[index].item->value;
		}

		index = (index + hash2) % hash_max; 

		if (index == hash1) {
			// printf("\n Add is failed \n");
			return -1;
		}
	}

    array[index].item = (struct data*) malloc(sizeof(struct data));
    strcpy(array[index].item->key, key);
    array[index].item->value = value;

	array[index].flag = 1;

	hash_size++;

	// printf("\n Key (%s) has been inserted \n", key);

    return value;

}

// Gets largest possible prime number less than hash_max
int get_prime() {
// Input: None
// Output: int. Gets largest possible prime number less than hash_max. Used to calculate hashcode.
// Processing: (O(n^2) time).

	for (int i = hash_max - 1; i >= 1; i--) {
		int flag = 0;
		for (int j = 2; j <= (int)sqrt(i); j++) {
            if (i % j == 0)
				flag++;
		}
		if (flag == 0)
			return i;
	}
	return 3;
}

// initializes array
void init_array() {
// Input: None
// Output: int. Gets largest possible prime number less than hash_max. Used to calculate hashcode.
// Processing: (O(n^2) time).

	for(int i = 0; i < hash_max; i++) {
		array[i].item = NULL;
		array[i].flag = 0;
	}
    prime = get_prime();
    hash_size = 0;
}

// Used in call to qsort() for leaders array
int comparator (const void * p1, const void * p2) {
// Input: const void * p1, const void * p2
// Output: int. Used in call to qsort() for leaders array
// Processing: (O(1) time).

    return (*(int*)p1 - *(int*)p2);
}

// Frees memory
void freeMemory(char **vertex_names, int n) {
// Input: char **vertex_names, int n
// Output: Void. Frees memory for the hashtable, list of vertex names, edgeTab, parent, and weight
// Processing: (O(n) time).

    for (int i = 0; i < hash_max; i++)
        free(array[i].item);
    free(array);

    for(int i = 0; i < n; i++)
        free(vertex_names[i]);
    free(vertex_names);
    
    free(edgeTab);
    free(parent);
    free(weight);
}

//Returns value using scanf. Makes sure value is valid
int input() {
  // Input: None
  // Output: Returns int number using scanf. Will not return number until input is valid.
  // Processing: (O(1) time)

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

int main() {
    char **vertex_names; // Stores names of new vertices
    int n, m, hash_int, compare_int, vertex_names_size;

    int MSTweight = 0;
    int root1, root2;

    n = input(); //numVertices
    m = input(); //numEdges

    int MST[m]; //MST table

    // Allocate memory for array
    hash_max = n*2;
    array = (struct hashtable_item*) malloc(hash_max * sizeof(struct hashtable_item));
	init_array();

    // Allocate memory for vertex_names
    vertex_names = malloc(n * sizeof(char*));
    for(int i = 0; i < n; i++)
        vertex_names[i] = malloc(MAX_LENGTH * sizeof(char));

    if (!array) {
        printf("malloc failed\n");
        exit(0);
    }

    // Allocate memory for edgeTab
    edgeTab = (edgeType*) malloc(m*sizeof(edgeType));
    parent = (int*) malloc(n*sizeof(int));
    weight = (int*) malloc(n*sizeof(int));
    if (!edgeTab || !parent || !weight) {
        printf("error 2\n");
        exit(0);
    }

    compare_int = 0;
    hash_int = 0;
    vertex_names_size = 0;

    char tail[MAX_LENGTH];
    char head[MAX_LENGTH];
    int edge_weight = 0;

    for (int i = 0; i < m; i++) {
        scanf("%s %s %d", tail, head, &edge_weight);

        //edgeTab tail
        compare_int = insert(tail, hash_int);
        if (compare_int == hash_int) {  // If inserting new vertex
            edgeTab[i].tail = hash_int;
            strcpy(vertex_names[vertex_names_size], tail);
            hash_int++;
            vertex_names_size++;
        }
        else {
            edgeTab[i].tail = compare_int;
        }

        //edgeTab head
        compare_int = insert(head, hash_int);
        if (compare_int == hash_int) {  // If inserting new vertex
            edgeTab[i].head = hash_int;
            strcpy(vertex_names[vertex_names_size], head);
            hash_int++;
            vertex_names_size++;
        }
        else {
            edgeTab[i].head = compare_int;
        }

        //edgeTab weight
        edgeTab[i].weight = edge_weight;
    }

    for (int i = 0; i < n; i++) {
        parent[i] = i;
        weight[i] = 1;
    }
    numTrees = n;
    qsort(edgeTab, m, sizeof(edgeType), weightAscending);

    int MST_index = 0;

    for (int i = 0; i < m; i++) {
        root1 = find(edgeTab[i].tail);
        root2 = find(edgeTab[i].head);
        if (root1 == root2) {
            // printf("%d %d %d discarded\n", edgeTab[i].tail, edgeTab[i].head, edgeTab[i].weight);
        }
        else {
            // printf("%d %d %d included\n", edgeTab[i].tail, edgeTab[i].head, edgeTab[i].weight);
            MSTweight += edgeTab[i].weight;
            MST[MST_index] = i;
            MST_index++;
            unionFunc(root1, root2);
        }
    }

    int MST_leader[MST_index]; 

    for (int i = 0; i < MST_index; i++) {
        MST_leader[i] = find(edgeTab[MST[i]].tail);
    }

    int leader1, leader2 = 0;
    leader1 = MST_leader[0];
    for(int i = 0; i < MST_index; i++) {
        if(MST_leader[i] != leader1) {
            leader2 = MST_leader[i];
            break;
        }
    }

    if (leader2 < leader1) {
        int temp = leader1;
        leader1 = leader2;
        leader2 = temp;
    }

    int new_MST_leader1[MST_index];
    int new_MST_leader2[MST_index];
    int new_MST_leader1_index = 0;
    int new_MST_leader2_index = 0;

    for(int i = 0; i < MST_index; i++) {
        if (MST_leader[i] == leader1) {
            new_MST_leader1[new_MST_leader1_index] = MST[i];
            new_MST_leader1_index++;
        }
        else {
            new_MST_leader2[new_MST_leader2_index] = MST[i];
            new_MST_leader2_index++;
        }
    }

    int result[MST_index];
    int k, l = 0;

    for (k = 0; k < new_MST_leader1_index; k++) {
        result[k] = new_MST_leader1[k];
    }

    for (k = 0, l = new_MST_leader1_index; l < MST_index && k < new_MST_leader2_index; k++, l++) {
        result[l] = new_MST_leader2[k];
    }

    qsort(MST_leader, MST_index, sizeof(int), comparator);

    if (vertex_names_size < n)
        printf("Warning: continuing with only %d vertices\n", vertex_names_size);
    if (vertex_names_size > n) {
        printf("Warning: Has %d vertices but stated to have at most %d. Exiting program...\n", vertex_names_size, n);
        freeMemory(vertex_names, n);
        exit(0);
    }

    printf("Sum of weights of spanning edges %d\n", MSTweight);

    printf("\n");

    int index = 0;

    if (leader1 != 0) {
        printf("Outputting tree with leader %d\n", leader1);

        for(int i = 0; MST_leader[i] == leader1 && i < MST_index; i++) {
            printf("%s %s %d\n", vertex_names[edgeTab[result[i]].tail], vertex_names[edgeTab[result[i]].head], edgeTab[result[i]].weight);
            index = i + 1;
        }
        printf("\n");
    }

    if (leader2 != 0) {
        printf("Outputting tree with leader %d\n", leader2);

        for(int i = index; i < MST_index; i++) {
            printf("%s %s %d\n", vertex_names[edgeTab[result[i]].tail], vertex_names[edgeTab[result[i]].head], edgeTab[result[i]].weight);
        }
    }

    freeMemory(vertex_names, n);
}
