#include <iostream>
#include <algorithm>
using namespace std;
#define BIG_NUM 1000000000000

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
    // read
    int roadLen;
    cin >> roadLen;
    int *side1 = new int[roadLen];
    for (int i = 0; i < roadLen; i++)
        cin >> side1[i];
    int *side2 = new int[roadLen];
    for (int i = 0; i < roadLen; i++)
        cin >> side2[i];

    // set up
    for (int i = 0; i < roadLen; i++)
        side1[i]--, side2[i]--;
    int *transform = new int[roadLen];
    long long currInv, minInv = BIG_NUM;

    // shift side1
    for (int i = 0; i < roadLen; i++)
        side2[i] = transform[side2[i]] = i;
    for (int i = 0; i < roadLen; i++)
        side1[i] = transform[side1[i]];
    currInv = inversions(side1, roadLen);
    for (int i = roadLen - 1; i >= 0; i--)
    {
        minInv = min(minInv, currInv);
        currInv -= roadLen - side1[i] - 1;
        currInv += side1[i];
    }

    // shift side2
    for (int i = 0; i < roadLen; i++)
        side1[i] = transform[side1[i]] = i;
    for (int i = 0; i < roadLen; i++)
        side2[i] = transform[side2[i]];
    currInv = inversions(side2, roadLen);
    for (int i = roadLen - 1; i >= 0; i--)
    {
        minInv = min(minInv, currInv);
        currInv -= roadLen - side2[i] - 1;
        currInv += side2[i];
    }

    // print and delete
    cout << minInv << endl;
    delete[] side1;
    delete[] side2;
    delete[] transform;
    return 0;
}