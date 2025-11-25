#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

typedef struct FibNode {
    int key, degree, mark;
    struct FibNode *parent, *child, *left, *right;
} FibNode;

typedef struct FibHeap {
    FibNode *min;
    int n;
} FibHeap;

// ---------------------- Node and Heap Creation ---------------------

FibNode* createNode(int key) {
    FibNode *node = (FibNode*)malloc(sizeof(FibNode));
    node->key = key;
    node->degree = 0;
    node->mark = 0;
    node->parent = NULL;
    node->child = NULL;
    node->left = node;
    node->right = node;
    return node;
}


FibHeap* createHeap() {
    FibHeap *H = (FibHeap*)malloc(sizeof(FibHeap));
    H->min = NULL;
    H->n = 0;
    return H;
}


// ---------------------- Insert Operation ---------------------

void insert(FibHeap *H, int key) {
    FibNode *node = createNode(key);

    if (H->min == NULL) {
        H->min = node;
    } else {
        node->right = H->min->right;
        node->left = H->min;
        H->min->right->left = node;
        H->min->right = node;

        if (node->key < H->min->key)
            H->min = node;
    }
    H->n++;
}

// ---------------------- Display Root List ---------------------

void display(FibHeap *H) {
    if (H->min == NULL) {
        printf("\nHeap is empty.\n");
        return;
    }
    FibNode *temp = H->min;
    printf("\nRoot List: ");
    do {
        printf("%d ", temp->key);
        temp = temp->right;
    } while (temp != H->min);
    printf("\n");
}

// ---------------------- Link Tree ---------------------

void fibLink(FibNode *y, FibNode *x) {
    y->left->right = y->right;
    y->right->left = y->left;

    y->parent = x;

    if (x->child == NULL) {
        x->child = y;
        y->left = y;
        y->right = y;
    } else {
        y->right = x->child->right;
        y->left = x->child;
        x->child->right->left = y;
        x->child->right = y;
    }
    x->degree++;
    y->mark = 0;
}

// ---------------------- Consolidate ---------------------

void consolidate(FibHeap *H) {
    int maxD = 50;
    FibNode *A[maxD];
    for (int i = 0; i < maxD; i++) A[i] = NULL;

    FibNode *w = H->min;
    if (w == NULL) return;

    FibNode *start = w;
    do {
        FibNode *x = w;
        int d = x->degree;

        while (A[d] != NULL) {
            FibNode *y = A[d];
            if (x->key > y->key) {
                FibNode *temp = x;
                x = y;
                y = temp;
            }
            fibLink(y, x);
            A[d] = NULL;
            d++;
        }
        A[d] = x;
        w = w->right;

    } while (w != start);

    H->min = NULL;

    for (int i = 0; i < maxD; i++) {
        if (A[i] != NULL) {
            if (H->min == NULL) {
                H->min = A[i];
            } else {
                A[i]->right = H->min->right;
                A[i]->left = H->min;
                H->min->right->left = A[i];
                H->min->right = A[i];

                if (A[i]->key < H->min->key)
                    H->min = A[i];
            }
        }
    }
}

// ---------------------- Extract Min ---------------------

FibNode* extractMin(FibHeap *H) {
    FibNode *z = H->min;
    if (z != NULL) {

        if (z->child != NULL) {
            FibNode *x = z->child;
            do {
                FibNode *next = x->right;

                x->left->right = x->right;
                x->right->left = x->left;

                x->right = H->min->right;
                x->left = H->min;
                H->min->right->left = x;
                H->min->right = x;

                x->parent = NULL;
                x = next;
            } while (x != z->child);
        }

        z->left->right = z->right;
        z->right->left = z->left;

        if (z == z->right)
            H->min = NULL;
        else {
            H->min = z->right;
            consolidate(H);
        }
        H->n--;
    }
    return z;
}

// ---------------------- Cut Helpers ---------------------

void cut(FibHeap *H, FibNode *x, FibNode *y) {
    if (y->child == x) {
        if (x->right != x)
            y->child = x->right;
        else
            y->child = NULL;
    }
    x->left->right = x->right;
    x->right->left = x->left;

    y->degree--;

    x->right = H->min->right;
    x->left = H->min;
    H->min->right->left = x;
    H->min->right = x;

    x->parent = NULL;
    x->mark = 0;
}

void cascadingCut(FibHeap *H, FibNode *y) {
    FibNode *z = y->parent;
    if (z != NULL) {
        if (y->mark == 0)
            y->mark = 1;
        else {
            cut(H, y, z);
            cascadingCut(H, z);
        }
    }
}

// ---------------------- Decrease Key ---------------------

void decreaseKey(FibHeap *H, FibNode *x, int newKey) {
    if (newKey > x->key) {
        printf("Error: New key is greater!\n");
        return;
    }

    x->key = newKey;
    FibNode *y = x->parent;

    if (y != NULL && x->key < y->key) {
        cut(H, x, y);
        cascadingCut(H, y);
    }

    if (x->key < H->min->key)
        H->min = x;
}

// ---------------------- Delete ---------------------

void deleteNode(FibHeap *H, FibNode *x) {
    decreaseKey(H, x, INT_MIN);
    extractMin(H);
}

// ---------------------- Search Node in Root List (Simple) ---------------------

FibNode* search(FibNode* start, int key) {
    if (start == NULL) return NULL;

    FibNode *temp = start;
    do {
        if (temp->key == key) return temp;

        // search in children
        FibNode *res = search(temp->child, key);
        if (res != NULL) return res;

        temp = temp->right;
    } while (temp != start);

    return NULL;
}

// ---------------------- MAIN MENU ---------------------

int main() {
    FibHeap *H = createHeap();
    int choice, key, newKey;
    FibNode *node;

    while (1) {
        printf("\n===== Fibonacci Heap Menu =====\n");
        printf("1. Insert\n");
        printf("2. Find Min\n");
        printf("3. Extract Min\n");
        printf("4. Decrease Key\n");
        printf("5. Delete Key\n");
        printf("6. Display Root List\n");
        printf("7. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch (choice) {
        case 1:
            printf("Enter key to insert: ");
            scanf("%d", &key);
            insert(H, key);
            break;

        case 2:
            if (H->min)
                printf("Minimum: %d\n", H->min->key);
            else
                printf("Heap is empty.\n");
            break;

        case 3:
            node = extractMin(H);
            if (node)
                printf("Extracted Min: %d\n", node->key);
            else
                printf("Heap is empty.\n");
            break;

        case 4:
            printf("Enter key to decrease: ");
            scanf("%d", &key);
            node = search(H->min, key);
            if (node == NULL) {
                printf("Key not found.\n");
                break;
            }
            printf("Enter new smaller key: ");
            scanf("%d", &newKey);
            decreaseKey(H, node, newKey);
            break;

        case 5:
            printf("Enter key to delete: ");
            scanf("%d", &key);
            node = search(H->min, key);
            if (node == NULL) {
                printf("Key not found.\n");
                break;
            }
            deleteNode(H, node);
            printf("Deleted.\n");
            break;

        case 6:
            display(H);
            break;

        case 7:
            exit(0);

        default:
            printf("Invalid choice.\n");
        }
    }
}