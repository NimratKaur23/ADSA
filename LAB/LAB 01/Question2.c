#include <stdio.h>


int binaryComparisons = 0;
int ternaryComparisons = 0;


int binarySearch(int arr[], int n, int key) {
    int left = 0, right = n - 1;
    while (left <= right) {
        int mid = left + (right - left) / 2;
        binaryComparisons++;  
        if (arr[mid] == key)
            return mid;
        else if (arr[mid] < key)
            left = mid + 1;
        else
            right = mid - 1;
    }
    return -1;
}

int ternarySearch(int arr[], int n, int key) {
    int left = 0, right = n - 1;
    while (left <= right) {
        int mid1 = left + (right - left) / 3;
        int mid2 = right - (right - left) / 3;

        ternaryComparisons++;  
        if (arr[mid1] == key)
            return mid1;

        ternaryComparisons++;  
        if (arr[mid2] == key)
            return mid2;

        if (key < arr[mid1]) {
            right = mid1 - 1;
        } else if (key > arr[mid2]) {
            left = mid2 + 1;
        } else {
            left = mid1 + 1;
            right = mid2 - 1;
        }
    }
    return -1;
}

int main() {
    int n, key;

    printf("Enter number of elements: ");
    scanf("%d", &n);

    int arr[n];
    printf("Enter sorted elements: ");
    for (int i = 0; i < n; i++)
        scanf("%d", &arr[i]);

    printf("Enter element to search: ");
    scanf("%d", &key);

    
    binaryComparisons = 0;
    ternaryComparisons = 0;

    int result1 = binarySearch(arr, n, key);
    int result2 = ternarySearch(arr, n, key);

    
    if (result1 != -1)
        printf("\nBinary Search: Found at index %d", result1);
    else
        printf("\nBinary Search: Not Found");

    if (result2 != -1)
        printf("\nTernary Search: Found at index %d", result2);
    else
        printf("\nTernary Search: Not Found");

    printf("\n\nNumber of Comparisons:");
    printf("\nBinary Search Comparisons = %d", binaryComparisons);
    printf("\nTernary Search Comparisons = %d\n", ternaryComparisons);

    return 0;
}