// Unbalanced binary search tree header file - lab 4 fall 2022

// These will have to change if data in node is more than just an int.
#ifndef LAB4_H
#define LAB4_H

typedef int Key;
typedef Key Item;
#define key(A) (A)
#define less(A, B) (key(A) < key(B))
#define greater(A, B) (key(A) < key(B))
#define eq(A, B) (key(A) == key(B))

typedef struct STnode* link;

struct STnode {
    Item item;  // Data for this node
    link l, r;  // left & right links
    char tombstone; // 0 = live key, 1 = dead key
    int N;      // subtree size (counts only live nodes)
};

typedef struct LNode* l_node;

// A linked list node
struct LNode {
    Item item;      // Data for this node
    char tombstone; // 0 = live key, 1 = dead key
    l_node next;    // Next node
};

extern Item NULLitem;

link NEW(Item item, link l, link r, int N);     // Allocates and fills in a node

void STinit();          // Initialize tree with just a sentinel

void fixN(link h);

void fixAllN(link h);

link search(link h, Key v);

link STsearch(Key v);   // Find node for a key

Item selectR(link h, int i);

Item STselect(int k);   // Treat tree as flattened into an ordered array

int invSelect(link h, Key v);

int STinvSelect(Key v); // Inverse of STselect

void STinsert(Item item);     // Insert an item.  No uniqueness check

link insert(link h, Item item);

void STdelete(Key v);         // Delete item for a key, if present.

void checkInorder(link h);

int checkN(link h);

int verifyBSTproperties();    // Ensure that tree isn't damaged

void STprintTree();           // Dumps out tree

int count(link h);

int getLive(link h);  // Number of nodes in tree with active keys.

int getDead(link h);  // Number of nodes in tree with tombstone set.

int getRecycled();  // Number of elements available from last removeDead()

int getHeight(link h);  // Longest path to sentinel

void inorder(link h);

l_node sorted_l_node(link h);

void remove_dead_l_node();

link loadLive(int count);

void removeDead();  // Rebuild tree and put tombstoned elements in recycling list.

#endif
