#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

#define MAX_SIDE 750
#define MOD 1000000007

// BIT or fenwick tree
class BIT
{
private:
    int *bTree;
    int elements;

public:
    void init(int elem);
    ~BIT() { delete[] bTree; }
    int query(int node);
    void updt(int node, int cng);
};
void BIT::init(int elem)
{
    elements = elem;
    bTree = new int[elem + 1];
    for (int i = 1; i <= elem; i++)
        bTree[i] = 0;
}
int BIT::query(int node)
{
    if (node == -1)
        return 0;
    int sum = 0;
    for (int i = node + 1; i > 0; i -= (i & -i))
        sum = (sum + bTree[i]) % MOD;
    return sum;
}
void BIT::updt(int node, int cng)
{
    for (int i = node + 1; i <= elements; i += (i & -i))
        bTree[i] = (bTree[i] + cng) % MOD;
}

int findPos(const vector<int> &colsIn, int col) { return upper_bound(colsIn.begin(), colsIn.end(), col) - colsIn.begin() - 1; }

int main(void)
{
    // read
    int rows, cols, colors;
    cin >> rows >> cols >> colors;
    int grid[MAX_SIDE][MAX_SIDE];
    for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++)
        {
            cin >> grid[i][j];
            grid[i][j]--;
        }

    // optimize rows
    vector<int> *colsIn = new vector<int>[colors];
    for (int j = 0; j < cols; j++)
        for (int i = 0; i < rows; i++)
            if (colsIn[grid[i][j]].empty() ||
                colsIn[grid[i][j]].back() != j)
                colsIn[grid[i][j]].push_back(j);

    // run imaginary DP
    BIT total, *colored = new BIT[colors];
    total.init(cols);
    for (int i = 0; i < colors; i++)
        colored[i].init(colsIn[i].size());
    int *ans = new int[cols];
    total.updt(0, 1);
    colored[grid[0][0]].updt(0, 1);
    for (int i = 1; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            ans[j] = colored[grid[i][j]].query(findPos(colsIn[grid[i][j]], j - 1));
            ans[j] = (total.query(j - 1) - ans[j] + MOD) % MOD;
        }
        for (int j = 0; j < cols; j++)
        {
            total.updt(j, ans[j]);
            colored[grid[i][j]].updt(findPos(colsIn[grid[i][j]], j), ans[j]);
        }
    }

    // print and delete
    cout << ans[cols - 1] << endl;
    delete[] colsIn;
    delete[] colored;
    delete[] ans;
    return 0;
}