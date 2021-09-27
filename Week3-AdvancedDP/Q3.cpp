#include <iostream>
using namespace std;

#define MAX_PIES 301

int main(void)
{
    //read
    int numPies, numCows;
    cin >> numPies >> numCows;
    int maxCowWt[MAX_PIES][MAX_PIES][MAX_PIES] = {0};
    int weight, start, end;
    for (int i = 0; i < numCows; i++)
    {
        cin >> weight >> start >> end;
        start--, end--;
        for (int pos = start; pos <= end; pos++)
            maxCowWt[pos][start][end] = max(maxCowWt[pos][start][end], weight);
    }

    //maximum cow weight (cow_pos, range_start, range_end)
    for (int dist = 1; dist < numPies; dist++)
        for (end = dist; end < numPies; end++)
            for (int pos = end - dist; pos <= end; pos++)
                maxCowWt[pos][end - dist][end] = max(maxCowWt[pos][end - dist][end],
                                                     max(maxCowWt[pos][end - dist + 1][end],
                                                         maxCowWt[pos][end - dist][end - 1]));

    //DP: max ∈ [i, j]
    int DP[MAX_PIES][MAX_PIES] = {0};
    for (int i = numPies - 1; i >= 0; i--)
        for (int j = i; j < numPies; j++)
            for (int k = i; k <= j; k++)
            {
                if (k != j)
                    DP[i][j] = max(DP[i][j], DP[i][k] + DP[k + 1][j]);
                if (k == i)
                    DP[i][j] = max(DP[i][j], DP[k + 1][j] + maxCowWt[k][i][j]);
                else if (k == j)
                    DP[i][j] = max(DP[i][j], DP[i][k - 1] + maxCowWt[k][i][j]);
                else
                    DP[i][j] = max(DP[i][j], DP[i][k - 1] + DP[k + 1][j] + maxCowWt[k][i][j]);
            }

    //print
    cout << DP[0][numPies - 1] << endl;
    return 0;
}