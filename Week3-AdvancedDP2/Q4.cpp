#include <iostream>
using namespace std;

#define MAX_RANGE 50
#define MAX_VALUE 51
#define NA -1000
#define UPDT(val) (DP[left][right][low][high] = max(DP[left][right][low][high], val))

int main(void)
{
    //read
    int numCows;
    cin >> numCows;
    int *cows = new int[numCows];
    for (int i = 0; i < numCows; i++)
        cin >> cows[i];

    //set up DP: range of 1; range -1 from flip; when low > high
    int DP[MAX_RANGE][MAX_RANGE][MAX_VALUE][MAX_VALUE];
    for (int pos = 0; pos < numCows; pos++)
        for (int low = 0; low < MAX_VALUE; low++)
            for (int high = 0; high < MAX_VALUE; high++)
                DP[pos][pos][low][high] = (low <= cows[pos]) && (cows[pos] <= high);
    for (int pos = 1; pos < numCows; pos++)
        for (int low = 0; low < MAX_VALUE; low++)
            for (int high = 0; high < MAX_VALUE; high++)
                DP[pos][pos - 1][low][high] = 0;
    for (int left = 0; left < numCows; left++)
        for (int right = 0; right < numCows; right++)
            for (int low = 0; low < MAX_VALUE; low++)
                for (int high = 0; high < low; high++)
                    DP[left][right][low][high] = NA;

    //DP[left][right][low][high]: LIS in range [left, right] where included values are in [low, high]
    for (int rangeDiff = 1; rangeDiff < numCows; rangeDiff++)
        for (int left = 0; left < numCows - rangeDiff; left++)
            for (int valueDiff = 0; valueDiff < MAX_VALUE; valueDiff++)
                for (int low = 0; low < MAX_VALUE - valueDiff; low++)
                {
                    int right = left + rangeDiff, high = low + valueDiff;
                    DP[left][right][low][high] = 0;
                    //Don't take
                    UPDT(DP[left + 1][right][low][high]);
                    UPDT(DP[left][right - 1][low][high]);
                    //Simple take
                    if (cows[left] >= low)
                        UPDT(DP[left + 1][right][cows[left]][high] + 1);
                    if (cows[right] <= high)
                        UPDT(DP[left][right - 1][low][cows[right]] + 1);
                    //Reverse take
                    if (cows[right] >= low)
                        UPDT(DP[left + 1][right - 1][cows[right]][high] + 1);
                    if (cows[left] <= high)
                        UPDT(DP[left + 1][right - 1][low][cows[left]] + 1);
                    if (cows[right] >= low && cows[left] <= high)
                        UPDT(DP[left + 1][right - 1][cows[right]][cows[left]] + 2);
                }

    //print and delete
    cout << DP[0][numCows - 1][0][MAX_VALUE - 1] << endl;
    delete[] cows;
    return 0;
}