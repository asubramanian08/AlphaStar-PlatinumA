#include <iostream>
#include <algorithm>
using namespace std;

using cow = pair<pair<long long, long long>, long long>;
#define mp make_pair

class BIT
{
private:
    int *bTree;
    int elements;

public:
    BIT(int elem);
    BIT(int elem, int *initialize, int numInit);
    ~BIT() { delete[] bTree; }
    int query(int node);
    void updt(int node, int cng);
};
BIT::BIT(int elem)
{
    elements = elem;
    bTree = new int[elem + 1];
    for (int i = 1; i <= elem; i++)
        bTree[i] = 0;
}
BIT::BIT(int elem, int *initialize, int numInit)
{
    elements = elem;
    bTree = new int[elem + 1];
    for (int i = 1; i <= elem; i++)
        bTree[i] = 0;
    for (int i = 0; i < numInit; i++)
        updt(i, initialize[i]);
}
int BIT::query(int node)
{
    if (node == -1)
        return 0;
    int sum = 0;
    for (int i = node + 1; i > 0; i -= (i & -i))
        sum += bTree[i];
    return sum;
}
void BIT::updt(int node, int cng)
{
    for (int i = node + 1; i <= elements; i += (i & -i))
        bTree[i] += cng;
}

int main(void)
{
    // read
    int numCows, laps, length;
    cin >> numCows >> laps >> length;
    cow *cows = new cow[numCows]; // remainder, laps (first speed), finish place
    for (int i = 0; i < numCows; i++)
    {
        cin >> cows[i].first.second;
        cows[i].first.first = 0;
    }
    sort(cows, cows + numCows);
    for (int i = 0; i < numCows; i++)
    {
        cows[i].first.first = (cows[i].first.second * laps) % cows[numCows - 1].first.second;
        cows[i].first.second = (cows[i].first.second * laps) / cows[numCows - 1].first.second;
        cows[i].second = i;
    }
    sort(cows, cows + numCows);

    // BIT: setup and run
    BIT bit(numCows);
    for (int i = 0; i < numCows; i++)
        bit.updt(cows[i].second, cows[i].first.second);
    long long ans = 0;
    for (int i = numCows - 1; i >= 0; i--)
    {
        int currVal = bit.query(cows[i].second) - bit.query(cows[i].second - 1);
        ans += cows[i].second * currVal - bit.query(cows[i].second - 1);
        bit.updt(cows[i].second, 1);
    }

    // print and delete
    cout << ans << endl;
    delete[] cows;
    return 0;
}