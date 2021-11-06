#include <iostream>
#include <algorithm>
using namespace std;

// inversions
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
long long inversions(int *arr, int len)
{
    using pii = pair<int, int>;
    pii *order = new pii[len];
    for (int i = 0; i < len; i++)
        order[i] = pii(arr[i], i);
    sort(order, order + len);
    BIT bit(len);
    long long ans = 0;
    for (int i = len - 1; i >= 0; i--)
    {
        ans += bit.query(order[i].second);
        bit.updt(order[i].second, 1);
    }
    return ans;
}

int main(void)
{
    long long cows, threshold, wt;
    cin >> cows >> threshold;
    int *prefixSum = new int[cows + 1];
    prefixSum[0] = 0;
    for (int i = 0; i < cows; i++)
    {
        cin >> wt;
        prefixSum[i + 1] = prefixSum[i] + (wt < threshold ? -1 : +1);
    }
    long long inv = inversions(prefixSum, cows + 1);
    long long ans = (cows) * (cows + 1) / 2 - inv;
    cout << ans << endl;
    delete[] prefixSum;
    return 0;
}