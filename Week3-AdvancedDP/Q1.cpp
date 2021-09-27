#include <iostream>
using namespace std;

#define MAX_GAME 262144
#define MAX_VALUE 60
#define NA -1

int main(void)
{
    //read
    int gameSize;
    cin >> gameSize;
    int *game = new int[gameSize];
    for (int i = 0; i < gameSize; i++)
        cin >> game[i];

    //setup DP
    int DP[MAX_GAME][MAX_VALUE];
    for (int i = 0; i < gameSize; i++)
        for (int j = 0; j < MAX_VALUE; j++)
            DP[i][j] = NA;

    //add game values to DP
    for (int i = 0; i < gameSize; i++)
        DP[i][game[i]] = i + 1;

    //DP
    int maxVal = 1;
    for (int j = 2; j < MAX_VALUE; j++)
        for (int i = 0; i < gameSize; i++)
        {
            if (DP[i][j - 1] != NA && DP[i][j - 1] < gameSize)
                DP[i][j] = DP[DP[i][j - 1]][j - 1];
            if (DP[i][j] != NA)
                maxVal = max(maxVal, j);
        }

    //print and delete
    cout << maxVal << endl;
    delete[] game;
    return 0;
}