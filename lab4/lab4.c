// Name: Ethyn Nguyen
// ID: 1001930354
// Omega: gcc -o lab4 lab4.c -std=c99
// Run: ./lab4
//      ./lab4 < lab4fall22.a.dat
//      ./lab4 < lab4fall22.b.dat

// Program Description: 
// C program for maintaining an unbalanced binary search tree (with a sentinel at the bottom) 
// to process a sequence of commands (standard input) from the following list:

// 0 - Exit the program

// 1 x - Insert positive integer key x, unless x is already present in the tree. Besides inserting the key, subtree sizes must be
// updated. (Processing a duplicate x is handled as an update, even though there is no satellite data.)

// 2 x - Logically delete the item for positive integer key x by using a tombstone. If there is no item, then the operation is
// ignored. Subtree sizes must be updated.

// 3 x - Find the rank of x, i.e. the number of keys in the tree that are not larger than x (error message if x is not in the tree).
// Tombstoned items are not included!

// 4 k - Find the key with rank k (error message if k is not legal). Tombstoned items are not included!

// 5 - Print statistics - number of live nodes, number of dead nodes, number of nodes in the recycling list (which is likely to
// vary from the provided output), and the number of nodes on the longest path to the sentinel.

// 6 - Rebuild tree by collecting live nodes in an ascending order linked list and placing dead nodes on a recycling list (to be
// used later when inserting). The tree is then (recursively) rebuilt using an inorder approach without calling the insertion
// code. (The sample program load.c shows how to do this using an ordered input file instead of the list of live nodes.)

// 7 - Print the tree as is done in load.c, but indicating dead keys with surrounding parentheses and also outputting the
// subtree sizes.

// 8 - Perform an audit to check the tree for the inorder traversal property and correct subtree sizes (number of live nodes in
// each subtree) to give a final indication that the tree is “clean” or “corrupt”.

#include <stdlib.h>
#include <stdio.h>
#include "lab4.h" // Must include in order for program to run

link z, head;               // Pointers to sentinel and root
l_node live, dead;          // Pointers to live and dead lists
Item NULLitem = (-9999999);  // Data for sentinel

int trace = 0;  // Controls trace output for insert

int main() {
    int op, key, rank;
    int size;

    STinit();  // Initialize binary search tree
    verifyBSTproperties();

    scanf("%d",&op);

    while (op) {
        switch (op) {
            case 0:
                exit(0);
                break;
            case 1:
                scanf("%d",&key);
                printf("1 %d\n",key);
                STinsert(key);
                fixAllN(head);
                size = count(head);
                break;
            case 2:
                scanf("%d",&key);
                printf("2 %d\n",key);
                STdelete(key);
                fixAllN(head);
                break;
            case 3:
                scanf("%d",&key);
                printf("3 %d\n",key);
                rank = STinvSelect(key);
                if (rank == (-1))
                    printf("Key %d is not in tree\n", key);
                else
                    printf("Key %d has rank %d\n", key, rank);
                break;
            case 4:
                scanf("%d",&rank);
                printf("4 %d\n", rank);
                if (rank < 1 || rank > getLive(head))
                    printf("rank %d range error\n", rank);
                else
                    printf("rank %d has key %d\n", rank, key(STselect(rank)));
                break;
            case 5:
                printf("5\n");
                printf("Live %d Dead %d Recycled %d Height %d\n",
                    getLive(head), getDead(head), getRecycled(), getHeight(head));
                break;
            case 6:
                printf("6\n");
                removeDead();
                fixAllN(head);
                break;
            case 7:
                printf("7\n");
                STprintTree();
                break;
            case 8:
                printf("8\n");
                if (verifyBSTproperties())
                    printf("clean\n");
                else
                    printf("corrupt\n");
                break;
            default:
                printf("Bad operation %d\n",op);
        }
  
        fflush(stdout);  // In case output is going to a fifo
        scanf("%d",&op);
    }
    printf("0\n");
}


link NEW(Item item, link l, link r, int N) {
// Input: Item key, left child, right child, subtree size
// Output: Void. Allocates memory for a new node in the binary search tree
// Processing: (O(1) time).

    link x = malloc(sizeof *x); 
    x->item = item;
    x->l = l;
    x->r = r;
    x->tombstone = 0;
    x->N = N;
    return x;
}

l_node new_l_node(Item item, char tombstone) {
// Input: Item key, tombstone
// Output: Void. Allocates memory for a new node in linked list
// Processing: (O(1) time).

    l_node x = malloc(sizeof *x);
    x->item = item;
    x->tombstone = tombstone;
    x->next = NULL;
    return x;
}

void STinit() {
// Input: None
// Output: Void. Initializes head and z pointers of binary search tree
// Processing: (O(1) time).

    head = (z = NEW(NULLitem, 0, 0, 0));
    head->tombstone = 0;
}

void fixN(link h) {
// Input: link h
// Output: Fixes subtree size of h, assuming that subtrees have correct sizes
// Processing: (O(1) time).

    if (h->tombstone != 1)
        h->N = h->l->N + h->r->N + 1;
    else
        h->N = h->l->N + h->r->N;
}

void fixAllN(link h) {
// Input: link h
// Output: Fixes subtree size of all h
// Processing: (O(h) time).

    if (h->l != z)
        fixAllN(h->l);
    if (h->r != z)
        fixAllN(h->r);
    fixN(h);
}

link insert(link h, Item item) {
// Input: link h, Item key
// Output: Recursively adds a new node to the binary search tree
// Processing: (O(h) time).

    Key v = key(item);

    if (h == z) {
        if (getRecycled() > 0) {
            l_node temp = dead;
            dead = dead->next;
            free(temp);
        }
        link new = NEW(item, z, z, 1);
        return new;
    }
    
    if (eq(v, h->item)) {
        if (h->tombstone == 1)
            h->tombstone = 0;
        return h;
    }
 
    if (less(v, h->item)) {
        h->l = insert(h->l, item);
    }   
    else {
        h->r = insert(h->r, item);
    }
        
    return h;
}

void STinsert(Item item) {
// Input: Item item
// Output: Inserts an item. No uniquenes check
// Processing: (O(h) time).

    head = insert(head, item);
    head->tombstone = 0;
}

Key lastInorder;

void cleanUpUnbalanced(link h) {
// Input: link h
// Output: Checks a tree constructed elsewhere
// Processing: (O(n) time).

    fixAllN(h);
    head = h;
    z->tombstone = 0;
    verifyBSTproperties();
}

// Checks all required properties.
// If a fatal problem is found, the tree is printed before exit(0)
int verifyBSTproperties() {
// Input: None
// Output: Checks all required properties.
//         If a fatal problem is found, the tree is printed before exit(0)
// Processing: (O(n) time).
    int result = 0;
    lastInorder=(-99999999);
    checkInorder(head);
    result = checkN(head);
    return result;
}

void checkInorder(link h) {
// Input: link h
// Output: Checks that inorder yields keys in ascending order
// Processing: (O(n) time)

    if (h==z)
        return;

    checkInorder(h->l);

    if (!less(lastInorder, h->item)) {
        printf("Inorder error\n");
        STprintTree();
        exit(0);
    }
    lastInorder = key(h->item);
    checkInorder(h->r);
}

int checkN(link h) {
// Input: link h
// Output: Verifies that subtree sizes are correct
// Processing: (O(n) time)

    int work;

    if (h == z) {
        if (h->N!=0) {
            printf("Count for sentinel is %d, should be 0\n",h->N);
            STprintTree();
            exit(0);
        }
    }
    else {
        if (h->tombstone != 1)
            work = checkN(h->l) + checkN(h->r) + 1;
        else
            work = checkN(h->l) + checkN(h->r);

        if (h->N != work) {
            printf("Count for key %d is %d, should be %d\n", key(h->item), h->N, work);
            STprintTree();
            exit(0);
        }
    }
    return h->N;
}

int invSelect(link h, Key v) {
// Input: link h, Key v
// Output: Returns the rank of the key v
// Processing: (O(h) time)
    
    Key t = key(h->item);
    int work;

    if (h==z)
        return -1;  // v doesn't appear as a key
    if (eq(v, t) && h->tombstone != 1)
        return h->l->N+1;
    if (less(v, t))
        return invSelect(h->l,v);

    work=invSelect(h->r,v);

    if (work==(-1))
        return -1;  // v doesn't appear as a key

    return 1 + h->l->N + work;
}

int STinvSelect(Key v) {
// Input: Key v
// Output: Returns the rank of the key v
// Processing: (O(h) time)

    return invSelect(head, v);
}

Item selectR(link h, int i) {
// Input: link h, int i
// Output: Returns the ith smallest key where i=1 returns the smallest key.
// Processing: (O(h) time)

    int r = h->l->N+1;

    if (h == z) {
        printf("Impossible situation in select\n");
        STprintTree();
        exit(0);
    }
    if (i==r && h->tombstone != 1)
        return h->item;
    if (i<r)
        return selectR(h->l, i);

    return selectR(h->r, i-r);
}

Item STselect(int k) {
// Input: int k
// Output: Returns the ith smallest key where i=1 returns the smallest key.
// Processing: (O(h) time)

    if (k<1 || k>head->N) {
        printf("Range error in STselect() k %d N %d\n", k, head->N);
        exit(0);
    }
    return selectR(head, k);
}

link search(link h, Key v) { 
// Input: link h, Key v
// Output: Recursive search that returns key if key exists and is not dead
// Processing: (O(h) time)

    Key t = key(h->item);
    if (h == z) 
        return z;
    if (eq(v, t) && h->tombstone != 1)
        return h;
    if (less(v, t))
        return search(h->l, v);

    return search(h->r, v);
}

link STsearch(Key v) {
// Input: Key v
// Output: Returns key if key exists and is not dead
// Processing: (O(h) time)

    return search(head, v);
}

void STdelete(Key v) {
// Input: int k
// Output: Deletes a key by turning it into a tombstone
// Processing: (O(h) time)

    link key = search(head, v);
    if (key == z)
        return;

    key->tombstone = 1;
    key->N = key->N - 1;
}

void print(link h, int level) {
// Input: link h, int level
// Output: Prints the binary search tree to console
// Processing: (O(n) time)

    int i;

    if (h == z) {
        for (i = 0; i < level; i++)
            printf("   ");

        printf(".\n");
        return;
    }

    print(h->r, level+1);

    for (i = 0; i < level; i++)
        printf("   ");

    if (h->tombstone != 1)
        printf("%d %d\n", h->item, h->N);
    else
        printf("(%d) %d\n", h->item, h->N);

    print(h->l, level+1);
}

void STprintTree() {
// Input: None
// Output: Prints the binary search tree to console
// Processing: (O(n) time)

    print(head, 0);
}

int count(link h) {
// Input: link h
// Output: Returns total number of nodes in binary search tree
// Processing: (O(h) time)

    int c = 1;

    if (h == z)
        return 0;
    else {
        c += count(h->l);
        c += count(h->r);
        return c;
    }
}

int getLive(link h) {
// Input: link h
// Output: Returns total number of live nodes in binary search tree
// Processing: (O(1) time)

    if (h == z)
        return 0;
    
    if (h->tombstone != 1)
        return 1 + getLive(h->l) + getLive(h->r);
    else
        return getLive(h->l) + getLive(h->r);
}

int getDead(link h) {
// Input: link h
// Output: Returns total number of dead nodes in binary search tree
// Processing: (O(1) time)

    if (h == z)
        return 0;
    
    if (h->tombstone == 1)
        return 1 + getDead(h->l) + getDead(h->r);
    else
        return getDead(h->l) + getDead(h->r);
}

int getRecycled() {
// Input: link h
// Output: Returns total number of recycled nodes in binary search tree
// Processing: (O(1) time)

    int count = 0;
    l_node current = dead; // Initialize current
    while (current != NULL) {
        count++;
        current = current->next;
    }
    return count;
}

int getHeight(link h) {
// Input: link h
// Output: Returns the height of the binary search tree
// Processing: (O(h) time)
 
    if (h == z) {
        return 0;
    }

    int lefth = getHeight(h->l);
    int righth = getHeight(h->r);

    if (lefth > righth) {
        return lefth + 1;
    } else {
        return righth + 1;
    }

    return 0;
}

l_node sorted_l_node(link h) {
// Input: link h
// Output: Converts binary search tree to linked list
// Processing: (O(h) time)

    l_node l_node1 = (h->l != z) ? sorted_l_node(h->l) : NULL;
    l_node l_node2 = (h->r != z) ? sorted_l_node(h->r) : NULL;
    l_node l_node3 = new_l_node(h->item, h->tombstone);

    // The "middle" l_node3 cannot be NULL; append l_node2 to l_node3
    l_node3->next = l_node2; // If l_node2 is NULL, it's OK
    if (!l_node1) return l_node3; // Nothing to prepend
    l_node last = l_node1;
    while (last->next) last = last->next; // Go to the end of l_node1
    last->next = l_node3; // Append l_node3+l_node2 to the end of l_node1
    return l_node1;
}

void remove_dead_l_node() {
// Input: None
// Output: Removes dead nodes and puts them on a recycling list
// Processing: (O(n) time)

  l_node temp;
  if (live->tombstone == 1) {
    temp = live;
    live = live->next;
    temp->next = dead;
    dead = temp;
    remove_dead_l_node();
  } else {
    l_node current = live;
    while (current->next != NULL) {
        if (current->next->tombstone == 1) {
            temp = current->next;
            current->next = current->next->next;
            temp->next = dead;
            dead = temp;
        } else
            current = current->next;
    }
  }
}

link loadLive(int count) { 
// Input: int count
// Output: Loads binary search tree with live nodes
// Processing: (O(n) time)

    int leftCount, rightCount;
    link work, left;

    if (count == 0)
        return z;

    leftCount = count/2;
    rightCount = count-leftCount-1;

    left = loadLive(leftCount);

    work = (link) malloc(sizeof(struct STnode));
    work->l = left;

    work->item = live->item;
    work->tombstone = 0;
    work->N = 0;

    live = live->next;

    work->r = loadLive(rightCount);
    return work;
}

void removeDead() {
// Input: None
// Output: Rebuild tree by collecting live nodes in an ascending order linked list 
// Processing: (O(n) time)

    live = sorted_l_node(head);
    remove_dead_l_node();
    head = loadLive(getLive(head));
}



