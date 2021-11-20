// Segment Tree: Fully Passes
#include <iostream>
#include <cmath>
#include <vector>
#include <map>
using namespace std;
using pii = pair<int, int>;

// MinSegTree
#define BIG_NUM 10000000000
class MinSegTree
{
private:
    int *tree;
    pii *cover;
    void init_tree(int pos, int left, int right, int *arr)
    {
        cover[pos] = pii(left, right);
        if (left == right)
        {
            tree[pos] = arr[left];
            return;
        }
        init_tree(2 * pos + 0, left, (left + right) / 2 + 0, arr);
        init_tree(2 * pos + 1, (left + right) / 2 + 1, right, arr);
        tree[pos] = min(tree[2 * pos + 0], tree[2 * pos + 1]);
    }

public:
    MinSegTree(int size, int *arr)
    {
        tree = new int[size * 4];
        cover = new pii[size * 4];
        init_tree(1, 0, size - 1, arr);
    }
    ~MinSegTree()
    {
        delete[] tree;
        delete[] cover;
    }
    int query(int left, int right, int pos = 1)
    {
        if (left > right || cover[pos].second < left || right < cover[pos].first)
            return BIG_NUM;
        if (left <= cover[pos].first && cover[pos].second <= right)
            return tree[pos];
        return min(query(left, right, 2 * pos + 0),
                   query(left, right, 2 * pos + 1));
    }
    void update(int node, int set, int pos = 1)
    {
        // no intersection
        if (cover[pos].second < node ||
            node < cover[pos].first)
            return;
        // fully covered
        if (node == cover[pos].first &&
            node == cover[pos].second)
            tree[pos] = set;
        // some intersection
        else
        {
            update(node, set, 2 * pos + 0);
            update(node, set, 2 * pos + 1);
            tree[pos] = min(tree[2 * pos + 0], tree[2 * pos + 1]);
        }
    }
};

int main(void)
{
    // read
    int length;
    cin >> length;
    int *biosequence = new int[length];
    for (int i = 0; i < length; i++)
        cin >> biosequence[i];
    MinSegTree mst(length, biosequence); // RMQ: min segment tree

    // framed intervals: monetized queue
    vector<pii> intervals;
    vector<int> mq /*monetize queue*/;
    map<int, vector<int> > possibleEnds;
    for (int i = length - 1; i >= 0; i--)
    {
        // interval starting at i
        int currRef = biosequence[i] - i;
        int end = possibleEnds[currRef].empty() ? i : possibleEnds[currRef].back();
        if (end != i && mst.query(i, end) == biosequence[i])
            intervals.push_back(pii(i, end));
        // update monetized queue
        while (!mq.empty() && biosequence[mq.back()] < biosequence[i])
        {
            possibleEnds[biosequence[mq.back()] - mq.back()].pop_back();
            mq.pop_back();
        }
        mq.push_back(i);
        possibleEnds[currRef].push_back(i);
    }

    // empodia
    vector<pii> empodia;
    int minEnd = intervals.front().second + 1;
    for (int i = 0; i < intervals.size(); minEnd = min(minEnd, intervals[i].second), i++)
        if (intervals[i].second < minEnd)
            empodia.push_back(intervals[i]);

    // print and delete
    cout << empodia.size() << endl;
    for (int i = empodia.size() - 1; i >= 0; i--)
        cout << (empodia[i].first + 1) << ' '
             << (empodia[i].second + 1) << endl;
    delete[] biosequence;
    return 0;
}