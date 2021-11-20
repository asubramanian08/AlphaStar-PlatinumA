#include <iostream>
#include <algorithm>
using namespace std;

#define BIG_NUM 1000000000
#define MAX_GUARDS 100000
#define MAX_REMOVE 101

int main(void)
{
    // read
    int numGuards, remove;
    cin >> numGuards >> remove;
    using pii = pair<int, int>;
    pii *guards = new pii[numGuards];
    for (int i = 0; i < numGuards; i++)
        cin >> guards[i].first >> guards[i].second;

    // remove enclosed cows
    sort(guards, guards + numGuards);
    int currEnd = -1, change = 0;
    for (int i = 0; i < numGuards; i++)
        if (guards[i].second < currEnd)
        {
            guards[i] = pii(BIG_NUM, BIG_NUM);
            change++;
        }
        else
            currEnd = guards[i].second;
    sort(guards, guards + numGuards);
    numGuards -= change, remove = max(remove - change, 0);

    // DP[i][j][used]: i = guard number, j = amount removed
    int DP[MAX_GUARDS][MAX_REMOVE][2] = {0};
    for (int i = 0; i <= remove; i++)
        DP[0][i][1] = DP[0][i][0] = -BIG_NUM;
    DP[0][remove][1] = guards[0].second - guards[0].first;
    DP[0][remove - 1][0] = DP[0][remove][0] = 0;
    int lastOverlap = 0;
    for (int i = 1; i < numGuards; i++)
    {
        while (guards[lastOverlap].second < guards[i].first)
            lastOverlap++;
        if (lastOverlap == i)
            lastOverlap--;
        int overlap = max(guards[lastOverlap].second - guards[i].first, 0);
        int currCow = guards[i].second - guards[i].first;
        for (int j = 0; j <= remove; j++)
            DP[i][j][1] = max(DP[lastOverlap][min(j + i - lastOverlap - 1, remove)][1] - overlap,
                              DP[lastOverlap][min(j + i - lastOverlap - 1, remove)][0]) + currCow;
        for (int j = 0; j <= remove; j++)
            DP[i][j][0] = max(DP[i - 1][min(j + 1, remove)][0], DP[i - 1][min(j + 1, remove)][1]);
    }

    // print and delete
    cout << max(DP[numGuards - 1][0][1], DP[numGuards - 1][0][0]) << endl;
    delete[] guards;
    return 0;
}