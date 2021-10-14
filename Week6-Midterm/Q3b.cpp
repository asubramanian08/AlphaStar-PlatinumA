#include <iostream>
using namespace std;

#define MAX_SIDE 500
#define MOD 1000000007

int main(void)
{
    // read
    int sideLen;
    cin >> sideLen;
    int grid[MAX_SIDE][MAX_SIDE];
    char letter;
    for (int i = 0; i < sideLen; i++)
        for (int j = 0; j < sideLen; j++)
        {
            cin >> letter;
            grid[i][j] = letter - 'A';
        }

    // 2D arrays for DP and next
    long long(*DP)[MAX_SIDE] = new long long[MAX_SIDE][MAX_SIDE];
    long long(*next)[MAX_SIDE] = new long long[MAX_SIDE][MAX_SIDE];
    for (int i = 0; i < sideLen; i++)
        for (int j = 0; j < sideLen; j++)
            DP[i][j] = 0;

    // DP[l][r]: number of palindromes of length 2*len+1 ending at columns l and r
    for (int col = 0; col < sideLen; col++)
        DP[col][col] = 1;
    for (int len = 0; len < sideLen - 1; len++)
    {
        for (int i = 0; i < sideLen; i++)
            for (int j = 0; j < sideLen; j++)
                next[i][j] = 0;
        for (int lCol = 0; lCol < sideLen; lCol++)
            for (int rCol = lCol; rCol < sideLen; rCol++)
            {
                int lRow = (sideLen - 1 - len) - lCol;
                int rRow = (sideLen - 1 + len) - rCol;
                if (0 > lRow || lRow >= sideLen || 0 > rRow || rRow >= sideLen)
                    continue;
                DP[lCol][rCol] %= MOD;
                if (lRow > 0 && rRow < sideLen - 1 && grid[lRow - 1][lCol] == grid[rRow + 1][rCol])
                    next[lCol][rCol] += DP[lCol][rCol];
                if (lRow > 0 && rCol < sideLen - 1 && grid[lRow - 1][lCol] == grid[rRow][rCol + 1])
                    next[lCol][rCol + 1] += DP[lCol][rCol];
                if (lCol > 0 && rRow < sideLen - 1 && grid[lRow][lCol - 1] == grid[rRow + 1][rCol])
                    next[lCol - 1][rCol] += DP[lCol][rCol];
                if (lCol > 0 && rCol < sideLen - 1 && grid[lRow][lCol - 1] == grid[rRow][rCol + 1])
                    next[lCol - 1][rCol + 1] += DP[lCol][rCol];
            }
        swap(DP, next);
    }

    // print
    cout << (DP[0][sideLen - 1] % MOD) << endl;
    return 0;
}