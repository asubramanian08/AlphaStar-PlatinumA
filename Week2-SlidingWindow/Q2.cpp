#include <iostream>
using namespace std;

#define MAX_ITEMS 100000
#define MAX_COINS 16
int maxDist[MAX_ITEMS][MAX_COINS];
int DP[1 << MAX_COINS] = {0};

int main(void)
{
    //read
    int numCoins, numItems;
    cin >> numCoins >> numItems;
    int *coins = new int[numCoins];
    for (int i = 0; i < numCoins; i++)
        cin >> coins[i];
    int *items = new int[numItems];
    for (int i = 0; i < numItems; i++)
        cin >> items[i];

    //maxDist[i][c] from i using c
    for (int c = 0; c < numCoins; c++)
        for (int start = numItems - 1, end = numItems, cost = 0; start >= 0; start--)
        {
            cost += items[start];
            while (coins[c] < cost)
            {
                end--;
                cost -= items[end];
            }
            maxDist[start][c] = end;
        }

    //DP[mask]: farthest it can go
    for (int mask = 0; mask < (1 << numCoins); mask++)
        if (DP[mask] != numItems)
            for (int i = 0; i < numCoins; i++)
                if (!(mask & 1 << i))
                    DP[mask | 1 << i] = max(DP[mask | 1 << i], maxDist[DP[mask]][i]);

    //remaining coins
    int ans = -1, curr;
    for (int mask = 0; mask < (1 << numCoins); mask++)
        if (DP[mask] == numItems)
        {
            curr = 0;
            for (int i = 0; i < numCoins; i++)
                if (!(mask & 1 << i))
                    curr += coins[i];
            ans = max(ans, curr);
        }

    //print and delete
    cout << ans << endl;
    delete[] coins;
    delete[] items;
    return 0;
}