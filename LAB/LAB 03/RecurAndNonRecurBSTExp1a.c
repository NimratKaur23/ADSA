#include <stdio.h>

/* ===== Recursive Binary Search ===== */
int binarySearchRecursive(int arr[], int left, int right, int key) {
    if (left <= right) {
        int mid = left + (right - left) / 2;

        if (arr[mid] == key)
            return mid;  /* Key found */
        else if (key < arr[mid])
            return binarySearchRecursive(arr, left, mid - 1, key);
        else
            return binarySearchRecursive(arr, mid + 1, right, key);
    }
    return -1;  /* Key not found */
}

/* ===== Iterative (Non-Recursive) Binary Search ===== */
int binarySearchIterative(int arr[], int n, int key) {
    int left = 0, right = n - 1, mid;
    while (left <= right) {
        mid = left + (right - left) / 2;

        if (arr[mid] == key)
            return mid;
        else if (key < arr[mid])
            right = mid - 1;
        else
            left = mid + 1;
    }
    return -1;  /* Key not found */
}

int main() {
    int n, key, i;
    int arr[100];  /* Array declaration at the top */

    printf("Enter the number of elements: ");
    scanf("%d", &n);

    printf("Enter %d integers in ascending order:\n", n);
    for (i = 0; i < n; i++) {
        scanf("%d", &arr[i]);
    }

    printf("Enter the key to search: ");
    scanf("%d", &key);

    /* Recursive Search */
    {
        int resultRecursive = binarySearchRecursive(arr, 0, n - 1, key);
        if (resultRecursive != -1)
            printf("Recursive Search: Key %d found at position %d\n", key, resultRecursive + 1);
        else
            printf("Recursive Search: Key %d not found\n", key);
    }

    /* Iterative Search */
    {
        int resultIterative = binarySearchIterative(arr, n, key);
        if (resultIterative != -1)
            printf("Iterative Search: Key %d found at position %d\n", key, resultIterative + 1);
        else
            printf("Iterative Search: Key %d not found\n", key);
    }

    return 0;
}