#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// A B-Tree node
typedef struct BTreeNode {
    int *keys;      // An array of keys
    int t;          // Minimum degree (defines the range for number of keys)
    struct BTreeNode **C; // An array of child pointers
    int n;          // Current number of keys
    bool leaf;      // Is true when node is leaf. Otherwise false
} BTreeNode;

// A utility function that creates a new B-Tree node
BTreeNode *createNode(int t, bool leaf) {
    BTreeNode *node = (BTreeNode *)malloc(sizeof(BTreeNode));
    node->t = t;
    node->leaf = leaf;
    node->keys = (int *)malloc(sizeof(int) * (2*t - 1));
    node->C = (BTreeNode **)malloc(sizeof(BTreeNode*) * (2*t));
    node->n = 0;
    for (int i = 0; i < 2*t; ++i) node->C[i] = NULL;
    return node;
}

// Function to traverse all nodes in a subtree rooted with this node
void traverse(BTreeNode *root) {
    if (!root) return;
    int i;
    for (i = 0; i < root->n; i++) {
        if (!root->leaf)
            traverse(root->C[i]);
        printf("%d ", root->keys[i]);
    }
    if (!root->leaf)
        traverse(root->C[i]);
}

// Function to search key k in subtree rooted with this node
BTreeNode *search(BTreeNode *root, int k, int *pos) {
    if (!root) return NULL;
    int i = 0;
    while (i < root->n && k > root->keys[i])
        i++;
    if (i < root->n && root->keys[i] == k) {
        if (pos) *pos = i;
        return root;
    }
    if (root->leaf) return NULL;
    return search(root->C[i], k, pos);
}

// A utility function to split the child y of this node. i is index of y in child array C[]
// The child y must be full when this function is called
void splitChild(BTreeNode *x, int i, BTreeNode *y) {
    int t = y->t;
    // Create a new node z which will store (t-1) keys of y
    BTreeNode *z = createNode(t, y->leaf);
    z->n = t - 1;

    // Copy the last (t-1) keys of y to z
    for (int j = 0; j < t-1; j++)
        z->keys[j] = y->keys[j + t];

    // Copy the last t children of y to z
    if (!y->leaf) {
        for (int j = 0; j < t; j++)
            z->C[j] = y->C[j + t];
    }

    // Reduce the number of keys in y
    y->n = t - 1;

    // Create space for new child
    for (int j = x->n; j >= i+1; j--)
        x->C[j+1] = x->C[j];

    // Link the new child to this node
    x->C[i+1] = z;

    // Move keys in x to make space for middle key of y
    for (int j = x->n-1; j >= i; j--)
        x->keys[j+1] = x->keys[j];

    // Copy the middle key of y to x
    x->keys[i] = y->keys[t-1];

    // Increment count of keys in this node
    x->n = x->n + 1;
}

// A utility function to insert a new key in the subtree rooted with this node
// The node must be non-full when this function is called
void insertNonFull(BTreeNode *x, int k) {
    int i = x->n - 1;

    if (x->leaf) {
        // Find the location of new key to be inserted and move all greater keys one space ahead
        while (i >= 0 && x->keys[i] > k) {
            x->keys[i+1] = x->keys[i];
            i--;
        }
        x->keys[i+1] = k;
        x->n = x->n + 1;
    } else {
        while (i >= 0 && x->keys[i] > k)
            i--;
        i++;
        // See if the found child is full
        if (x->C[i]->n == 2*x->t - 1) {
            splitChild(x, i, x->C[i]);
            if (x->keys[i] < k)
                i++;
        }
        insertNonFull(x->C[i], k);
    }
}

// The main function that inserts a new key in this B-Tree
BTreeNode *insert(BTreeNode *root, int k, int t) {
    if (!root) {
        // Allocate memory for root
        root = createNode(t, true);
        root->keys[0] = k; // Insert key
        root->n = 1;       // Update number of keys in root
        return root;
    }

    if (root->n == 2*t - 1) {
        // If root is full, then tree grows in height
        BTreeNode *s = createNode(t, false);
        s->C[0] = root;
        splitChild(s, 0, root);

        // New root has two children; decide which of the two children will have new key
        int i = 0;
        if (s->keys[0] < k)
            i++;
        insertNonFull(s->C[i], k);
        return s;
    } else {
        insertNonFull(root, k);
        return root;
    }
}

// A utility function that returns the index of the first key that is >= k
int findKey(BTreeNode *node, int k) {
    int idx = 0;
    while (idx < node->n && node->keys[idx] < k)
        ++idx;
    return idx;
}

// A function to remove key k from the sub-tree rooted with this node
void removeKey(BTreeNode *node, int k);

// Remove the key present in idx-th position in this node which is a leaf
void removeFromLeaf(BTreeNode *node, int idx) {
    for (int i = idx+1; i < node->n; ++i)
        node->keys[i-1] = node->keys[i];
    node->n--;
}

// Remove the key present in idx-th position in this node which is a non-leaf
void removeFromNonLeaf(BTreeNode *node, int idx) {
    int k = node->keys[idx];

    // If the child that precedes k (C[idx]) has at least t keys,
    // find the predecessor 'pred' of k in the subtree rooted at C[idx].
    // Replace k by pred. Recursively delete pred in C[idx]
    if (node->C[idx]->n >= node->t) {
        BTreeNode *cur = node->C[idx];
        while (!cur->leaf)
            cur = cur->C[cur->n];
        int pred = cur->keys[cur->n - 1];
        node->keys[idx] = pred;
        removeKey(node->C[idx], pred);
    }
    // If the child C[idx] has less than t keys, examine C[idx+1]. If C[idx+1]
    // has at least t keys, find the successor 'succ' of k in the subtree rooted at C[idx+1]
    // Replace k by succ. Recursively delete succ in C[idx+1]
    else if (node->C[idx+1]->n >= node->t) {
        BTreeNode *cur = node->C[idx+1];
        while (!cur->leaf)
            cur = cur->C[0];
        int succ = cur->keys[0];
        node->keys[idx] = succ;
        removeKey(node->C[idx+1], succ);
    }
    // If both C[idx] and C[idx+1] have less than t keys, merge k and all of C[idx+1]
    // into C[idx]. Now C[idx] contains 2t-1 keys. Free C[idx+1] and recursively delete k from C[idx]
    else {
        int t = node->t;
        BTreeNode *child = node->C[idx];
        BTreeNode *sibling = node->C[idx+1];

        // Pull a key from the current node and insert it into (t-1)th position of C[idx]
        child->keys[child->n] = node->keys[idx];

        // Copy the keys from sibling to child
        for (int i = 0; i < sibling->n; ++i)
            child->keys[child->n + 1 + i] = sibling->keys[i];

        // Copy the child pointers from sibling to child
        if (!child->leaf) {
            for (int i = 0; i <= sibling->n; ++i)
                child->C[child->n + 1 + i] = sibling->C[i];
        }

        // Move keys in current node
        for (int i = idx+1; i < node->n; ++i)
            node->keys[i-1] = node->keys[i];

        // Move child pointers in current node
        for (int i = idx+2; i <= node->n; ++i)
            node->C[i-1] = node->C[i];

        child->n += sibling->n + 1;
        node->n--;
        free(sibling->keys);
        free(sibling->C);
        free(sibling);

        removeKey(child, k);
    }
}

// A function to get predecessor of keys[idx]
int getPredecessor(BTreeNode *node, int idx) {
    BTreeNode *cur = node->C[idx];
    while (!cur->leaf)
        cur = cur->C[cur->n];
    return cur->keys[cur->n - 1];
}

// A function to get successor of keys[idx]
int getSuccessor(BTreeNode *node, int idx) {
    BTreeNode *cur = node->C[idx+1];
    while (!cur->leaf)
        cur = cur->C[0];
    return cur->keys[0];
}

// A function to fill child C[idx] which has less than t-1 keys
void fill(BTreeNode *node, int idx) {
    if (idx != 0 && node->C[idx-1]->n >= node->t)
        ; // borrowFromPrev will be implemented inline in caller
    else if (idx != node->n && node->C[idx+1]->n >= node->t)
        ; // borrowFromNext will be implemented inline in caller
    else {
        // Merge C[idx] with sibling
        // This function is folded into callers for simplicity in this implementation
    }
}

// Borrow a key from C[idx-1] and insert it into C[idx]
void borrowFromPrev(BTreeNode *node, int idx) {
    BTreeNode *child = node->C[idx];
    BTreeNode *sibling = node->C[idx-1];

    // Move all keys in child one step forward
    for (int i = child->n - 1; i >= 0; --i)
        child->keys[i+1] = child->keys[i];

    // If child is not leaf, move its child pointers one step forward
    if (!child->leaf) {
        for (int i = child->n; i >= 0; --i)
            child->C[i+1] = child->C[i];
    }

    // Set child's first key equal to keys[idx-1] from current node
    child->keys[0] = node->keys[idx-1];

    // Move sibling's last child as C[idx]'s first child
    if (!child->leaf)
        child->C[0] = sibling->C[sibling->n];

    // Move the last key from sibling up to the parent
    node->keys[idx-1] = sibling->keys[sibling->n - 1];

    child->n += 1;
    sibling->n -= 1;
}

// Borrow a key from the C[idx+1] and place it in C[idx]
void borrowFromNext(BTreeNode *node, int idx) {
    BTreeNode *child = node->C[idx];
    BTreeNode *sibling = node->C[idx+1];

    // node's key[idx] is inserted as the last key in C[idx]
    child->keys[child->n] = node->keys[idx];

    // Sibling's first child is inserted as the last child into C[idx]
    if (!child->leaf)
        child->C[child->n + 1] = sibling->C[0];

    // First key from sibling is inserted into node
    node->keys[idx] = sibling->keys[0];

    // Move all keys in sibling one step left
    for (int i = 1; i < sibling->n; ++i)
        sibling->keys[i-1] = sibling->keys[i];

    // Move the child pointers one step left
    if (!sibling->leaf) {
        for (int i = 1; i <= sibling->n; ++i)
            sibling->C[i-1] = sibling->C[i];
    }

    child->n += 1;
    sibling->n -= 1;
}

// Merge C[idx] with C[idx+1]
// C[idx+1] is freed after merging
void merge(BTreeNode *node, int idx) {
    BTreeNode *child = node->C[idx];
    BTreeNode *sibling = node->C[idx+1];

    // Pull a key from the current node and insert it into (t-1)th position of C[idx]
    child->keys[child->n] = node->keys[idx];

    // Copy the keys from sibling to child
    for (int i = 0; i < sibling->n; ++i)
        child->keys[child->n + 1 + i] = sibling->keys[i];

    // Copy the child pointers from sibling to child
    if (!child->leaf) {
        for (int i = 0; i <= sibling->n; ++i)
            child->C[child->n + 1 + i] = sibling->C[i];
    }

    // Move keys and child pointers in current node
    for (int i = idx+1; i < node->n; ++i)
        node->keys[i-1] = node->keys[i];
    for (int i = idx+2; i <= node->n; ++i)
        node->C[i-1] = node->C[i];

    child->n += sibling->n + 1;
    node->n--;

    free(sibling->keys);
    free(sibling->C);
    free(sibling);
}

// The main function that removes key k from the sub-tree rooted with this node
void removeKey(BTreeNode *node, int k) {
    int idx = findKey(node, k);

    // The key to be removed is present in this node
    if (idx < node->n && node->keys[idx] == k) {
        if (node->leaf)
            removeFromLeaf(node, idx);
        else
            removeFromNonLeaf(node, idx);
    } else {
        // If this node is a leaf node, then the key is not present in tree
        if (node->leaf) {
            printf("The key %d is does not exist in the tree\n", k);
            return;
        }

        // The key to be removed is present in the sub-tree rooted with this node
        // The flag indicates whether the key is present in the sub-tree rooted with the last child of this node
        bool flag = ((idx == node->n) ? true : false);

        // If the child where the key is supposed to exist has less that t keys, we fill that child
        if (node->C[idx]->n < node->t) {
            // If the previous child has more than t-1 keys, borrow a key
            if (idx != 0 && node->C[idx-1]->n >= node->t)
                borrowFromPrev(node, idx);
            // If the next child has more than t-1 keys, borrow a key
            else if (idx != node->n && node->C[idx+1]->n >= node->t)
                borrowFromNext(node, idx);
            // Merge child with a sibling
            else {
                if (idx != node->n)
                    merge(node, idx);
                else
                    merge(node, idx-1), idx--;
            }
        }

        // After the fill, the child either moved or merged; recurse to appropriate child
        if (flag && idx > node->n)
            removeKey(node->C[idx-1], k);
        else
            removeKey(node->C[idx], k);
    }
}

// Wrapper to remove a key from B-Tree; it adjusts the root if necessary
BTreeNode *removeFromBTree(BTreeNode *root, int k) {
    if (!root) {
        printf("Tree is empty\n");
        return root;
    }

    removeKey(root, k);

    // If the root node has 0 keys, make its first child as the new root if it exists
    if (root->n == 0) {
        BTreeNode *tmp = root;
        if (root->leaf) {
            // tree had only one node
            root = NULL;
        } else {
            root = root->C[0];
        }
        // Free the old root
        if (tmp) {
            if (tmp->keys) free(tmp->keys);
            if (tmp->C) free(tmp->C);
            free(tmp);
        }
    }
    return root;
}

// A helper to free entire tree (post-order)
void freeBTree(BTreeNode *root) {
    if (!root) return;
    if (!root->leaf) {
        for (int i = 0; i <= root->n; ++i)
            freeBTree(root->C[i]);
    }
    if (root->keys) free(root->keys);
    if (root->C) free(root->C);
    free(root);
}

// Driver program to test above functions
int main() {
    int t = 3; // A B-Tree with minimum degree 3 (max 5 keys per node)
    BTreeNode *root = NULL;

    int keys[] = {10, 20, 5, 6, 12, 30, 7, 17};
    int nkeys = sizeof(keys)/sizeof(keys[0]);

    for (int i = 0; i < nkeys; ++i)
        root = insert(root, keys[i], t);

    printf("Traversal of tree constructed:\n");
    traverse(root);
    printf("\n");

    root = removeFromBTree(root, 6);
    printf("Traversal after deleting 6:\n");
    traverse(root);
    printf("\n");

    root = removeFromBTree(root, 13); // not present
    root = removeFromBTree(root, 7);
    printf("Traversal after deleting 7:\n");
    traverse(root);
    printf("\n");

    root = removeFromBTree(root, 4); // not present
    root = removeFromBTree(root, 2); // not present
    root = removeFromBTree(root, 16); // not present

    // Insert more keys
    int more[] = {8, 9, 1};
    for (int i = 0; i < 3; ++i)
        root = insert(root, more[i], t);

    printf("Traversal after inserting 8,9,1:\n");
    traverse(root);
    printf("\n");

    // Clean up
    freeBTree(root);
    return 0;
}
