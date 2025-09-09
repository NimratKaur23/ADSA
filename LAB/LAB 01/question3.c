#include <stdio.h>


int weigh(int coins[], int left, int right, int mid) {
    int sumLeft = 0, sumRight = 0;
    for (int i = left; i <= mid; i++) sumLeft += coins[i];
    for (int i = mid + 1; i <= right; i++) sumRight += coins[i];

    if (sumLeft < sumRight) return -1; 
    else if (sumLeft > sumRight) return 1; 
    else return 0; 
}
int findLighterCoin(int coins[], int left, int right, int normalWeight) {
    if (left == right) {
        if (coins[left] < normalWeight)
            return left;  
        else
            return -1;   
    }

    int mid = (left + right) / 2;
    int result = weigh(coins, left, right, mid);

    if (result == -1) {
        return findLighterCoin(coins, left, mid, normalWeight);
    } else if (result == 1) {
        return findLighterCoin(coins, mid + 1, right, normalWeight);
    } else {
        return -1;  
    }
}

int main() {
    int n;
    printf("Enter number of coins: ");
    scanf("%d", &n);

    int coins[n];
    printf("Enter weights of %d coins: ", n);
    for (int i = 0; i < n; i++)
        scanf("%d", &coins[i]);

    int normalWeight = coins[0];  
    int index = findLighterCoin(coins, 0, n - 1, normalWeight);

    if (index == -1)
        printf("All coins are perfect. No lighter coin found.\n");
    else
        printf("Defective lighter coin found at index %d (weight = %d)\n", index, coins[index]);

    return 0;
}
