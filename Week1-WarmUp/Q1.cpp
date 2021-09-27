#include <iostream>
#include <string>
using namespace std;

#define MAX_COWS 1010
int DP[MAX_COWS][MAX_COWS];
#define MOD 2012

int main(void)
{
    //read
    string parenthesis;
    cin >> parenthesis;

    //number open
    int *open = new int[parenthesis.size()];
    int currOpen = 0;
    for (int i = 0; i < parenthesis.size(); i++)
        open[i] = (parenthesis[i] == '(' ? currOpen++ : currOpen--);

    //run DP - top down or bottem up?
    DP[parenthesis.size()][0] = 1;
    for (int i = 1; i < parenthesis.size(); i++)
        DP[parenthesis.size()][i] = 0;
    for (int i = parenthesis.size() - 1; i >= 0; i--)
        for (int j = 0; j <= open[i]; j++)
        {
            DP[i][j] = DP[i + 1][j];
            if (parenthesis[i] == '(' && j < parenthesis.size() - 1)
                DP[i][j] += DP[i + 1][j + 1];
            if (parenthesis[i] == ')' && j > 0)
                DP[i][j] += DP[i + 1][j - 1];
            DP[i][j] %= MOD;
        }

    //print and delete
    cout << DP[0][0] << endl;
    delete[] open;
    return 0;
}