#include <iostream>
using namespace std;

#define MAX_GRID 300
#define MAX_ROT_GRID 600

int countEqu(bool grid[MAX_GRID][MAX_GRID])
{
    //set rot grid
    bool rot_grid[MAX_ROT_GRID][MAX_ROT_GRID] = {{false}};
    for (int i = 0; i < MAX_GRID; i++)
        for (int j = 0; j < MAX_GRID; j++)
            rot_grid[i - j + MAX_GRID][i + j] = grid[i][j];

    //prefix sums
    int prefixSum[MAX_ROT_GRID][MAX_ROT_GRID];
    for (int i = 0; i < MAX_ROT_GRID; i++)
    {
        prefixSum[i][0] = rot_grid[i][0];
        for (int j = 1; j < MAX_ROT_GRID; j++)
            prefixSum[i][j] = prefixSum[i][j - 1] + rot_grid[i][j];
    }

    //answer
    int ans = 0;
    for (int i = 0; i < MAX_ROT_GRID; i++)
        for (int j = 0; j < MAX_ROT_GRID; j++)
            if (rot_grid[i][j])
                for (int k = j + 1; k < MAX_ROT_GRID; k++)
                    if (rot_grid[i][k])
                    {
                        int side = k - j;
                        if (i + side < MAX_ROT_GRID)
                            ans += 2 * (prefixSum[i + side][k - 1] - prefixSum[i + side][j]) +
                                   (rot_grid[i + side][k] + rot_grid[i + side][j]);
                        if (i - side >= 0)
                            ans += 2 * (prefixSum[i - side][k - 1] - prefixSum[i - side][j]) +
                                   (rot_grid[i - side][k] + rot_grid[i - side][j]);
                    }
    return ans;
}

int main(void)
{
    //read
    int sideLen;
    cin >> sideLen;
    bool grid[MAX_GRID][MAX_GRID] = {{false}};
    for (int i = 0; i < sideLen; i++)
        for (int j = 0; j < sideLen; j++)
        {
            char ch;
            cin >> ch;
            grid[i][j] = ch == '*';
        }

    //ans: count equilaterals
    int ans = countEqu(grid);
    bool flip_grid[MAX_GRID][MAX_GRID];
    for (int i = 0; i < sideLen; i++)
        for (int j = 0; j < sideLen; j++)
            flip_grid[sideLen - 1 - j][i] = grid[i][j];
    ans += countEqu(flip_grid);

    //print
    cout << (ans / 2) << endl;
    return 0;
}