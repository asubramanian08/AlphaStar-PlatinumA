#include <iostream>
#include <algorithm>
using namespace std;

// SumSegTree
class SumSegTree
{
private:
    using pii = pair<int, int>;
    long long *tree;
    pii *cover;
    void init_tree(int pos, int left, int right, long long *arr)
    {
        cover[pos] = pii(left, right);
        if (left == right)
        {
            tree[pos] = arr[left];
            return;
        }
        init_tree(2 * pos + 0, left, (left + right) / 2 + 0, arr);
        init_tree(2 * pos + 1, (left + right) / 2 + 1, right, arr);
        tree[pos] = tree[2 * pos + 0] + tree[2 * pos + 1];
    }

public:
    SumSegTree(int size, long long *arr)
    {
        tree = new long long[size * 4];
        cover = new pii[size * 4];
        init_tree(1, 0, size - 1, arr);
    }
    ~SumSegTree()
    {
        delete[] tree;
        delete[] cover;
    }
    long long query(int left, int right, int pos = 1)
    {
        if (cover[pos].second < left || right < cover[pos].first)
            return 0;
        if (left <= cover[pos].first && cover[pos].second <= right)
            return tree[pos];
        return query(left, right, 2 * pos + 0) +
               query(left, right, 2 * pos + 1);
    }
    void update(int node, long long change, int pos = 1)
    {
        // no intersection
        if (cover[pos].second < node ||
            node < cover[pos].first)
            return;
        // fully covered
        if (node == cover[pos].first &&
            node == cover[pos].second)
            tree[pos] += change;
        // some intersection
        else
        {
            update(node, change, 2 * pos + 0);
            update(node, change, 2 * pos + 1);
            tree[pos] = tree[2 * pos + 0] + tree[2 * pos + 1];
        }
    }
};

int main(void)
{
    //  read
    int numCows;
    cin >> numCows;
    using cow = pair<pair<int, int>, int>;
    cow *cows = new cow[numCows];
    for (int i = 0; i < numCows; i++)
        cin >> cows[i].first.first >> cows[i].first.second;
    sort(cows, cows + numCows, [](cow c1, cow c2)
         { return c1.first.second < c2.first.second; });
    for (int i = 0; i < numCows; i++)
        if (i != 0 && cows[i].first.second == cows[i - 1].first.second)
            cows[i].second = cows[i - 1].second;
        else
            cows[i].second = i;
    sort(cows, cows + numCows);

    // init SumSegTrees
    long long *temp = new long long[numCows];
    fill(temp, temp + numCows, 0);
    SumSegTree left_lmst(numCows, temp);
    SumSegTree right_lmst(numCows, temp);
    for (int i = 0; i < numCows; i++)
        right_lmst.update(cows[i].second, +1);

    // Run though vertical lines, bsearch horizontal lines
    long long ans = numCows, currU, currL;
    for (int i = 0; i < numCows; i++)
    {
        right_lmst.update(cows[i].second, -1);
        left_lmst.update(cows[i].second, +1);
        if (i < numCows - 1 && cows[i].first.first == cows[i + 1].first.first)
            continue;
        int lo = 0, hi = numCows - 1, mid;
        ans = min(ans, max(left_lmst.query(lo, hi), right_lmst.query(lo, hi)));
        while (hi - lo > 1)
        {
            mid = (lo + hi) / 2;
            currU = max(right_lmst.query(mid + 1, numCows - 1),
                        left_lmst.query(mid + 1, numCows - 1));
            currL = max(right_lmst.query(0, mid),
                        left_lmst.query(0, mid));
            ans = min(ans, max(currU, currL));
            (currU > currL ? lo : hi) = mid;
        }
    }

    // print and delete
    cout << ans << endl;
    delete[] cows;
    delete[] temp;
    return 0;
}