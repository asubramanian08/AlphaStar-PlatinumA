#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

//BIT or fenwick tree
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

using pii = pair<int, int>;
int numCows;
pii *proficiency;
vector<int> *jobTree;
int *ans;

void DFS(int node, BIT &bit)
{
    int extra = bit.query(numCows - 1) - bit.query(proficiency[node].second);
    for (int i = 0; i < jobTree[node].size(); i++)
        DFS(jobTree[node][i], bit);
    int below = bit.query(numCows - 1) - bit.query(proficiency[node].second);
    ans[node] = below - extra;
    bit.updt(proficiency[node].second, 1);
}

int main(void)
{
    //read
    cin >> numCows;
    proficiency = new pii[numCows];
    for (int i = 0; i < numCows; i++)
    {
        cin >> proficiency[i].first;
        proficiency[i].second = i;
    }
    jobTree = new vector<int>[numCows];
    int parent;
    for (int i = 1; i < numCows; i++)
    {
        cin >> parent;
        jobTree[parent - 1].push_back(i);
    }

    //compress proficiency -> proficiency is second value
    sort(proficiency, proficiency + numCows);
    for (int i = 0; i < numCows; i++)
        proficiency[i] = pii(proficiency[i].second, i);
    sort(proficiency, proficiency + numCows);

    //DFS tree, answer with BIT
    BIT bit(numCows);
    ans = new int[numCows];
    DFS(0, bit);

    //print and delete
    for (int i = 0; i < numCows; i++)
        cout << ans[i] << endl;
    delete[] proficiency;
    delete[] jobTree;
    delete[] ans;
    return 0;
}