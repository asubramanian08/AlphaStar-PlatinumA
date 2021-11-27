#include <iostream>
using namespace std;

// LazyToggleSegTree
class LazyToggleSegTree
{
private:
    using pii = pair<int, int>;
    int *tree;
    bool *changeDown;
    pii *cover;
    void init_tree(int pos, int left, int right)
    {
        tree[pos] = 0;
        changeDown[pos] = false;
        cover[pos] = pii(left, right);
        if (left != right)
        {
            init_tree(2 * pos + 0, left, (left + right) / 2 + 0);
            init_tree(2 * pos + 1, (left + right) / 2 + 1, right);
        }
    }
    void updateChangeDown(int pos)
    {
        if (changeDown[pos] == false)
            return;
        tree[pos] = (cover[pos].second - cover[pos].first + 1) - tree[pos];
        if (cover[pos].first != cover[pos].second)
        {
            changeDown[2 * pos + 0] ^= changeDown[pos];
            changeDown[2 * pos + 1] ^= changeDown[pos];
        }
        changeDown[pos] = false;
    }

public:
    LazyToggleSegTree(int size)
    {
        tree = new int[size * 4];
        cover = new pii[size * 4];
        changeDown = new bool[size * 4];
        init_tree(1, 0, size - 1);
    }
    ~LazyToggleSegTree()
    {
        delete[] tree;
        delete[] changeDown;
        delete[] cover;
    }
    int query(int left, int right, int pos = 1)
    {
        updateChangeDown(pos);
        if (left <= cover[pos].first && cover[pos].second <= right)
            return tree[pos];
        if (cover[pos].second < left || right < cover[pos].first)
            return 0;
        return query(left, right, 2 * pos + 0) +
               query(left, right, 2 * pos + 1);
    }
    void update(int rangeLeft, int rangeRight, int pos = 1)
    {
        // fully covered
        if (rangeLeft <= cover[pos].first &&
            cover[pos].second <= rangeRight)
            changeDown[pos] ^= true;
        updateChangeDown(pos);
        // no intersection
        if (cover[pos].second < rangeLeft ||
            rangeRight < cover[pos].first)
            return;
        // some intersection
        if (cover[pos].first < rangeLeft ||
            rangeRight < cover[pos].second)
        {
            update(rangeLeft, rangeRight, 2 * pos + 0);
            update(rangeLeft, rangeRight, 2 * pos + 1);
            tree[pos] = tree[2 * pos + 0] + tree[2 * pos + 1];
        }
    }
};

int main(void)
{
    int numSwitches, opers;
    cin >> numSwitches >> opers;
    LazyToggleSegTree ltst(numSwitches);
    for (int i = 0; i < opers; i++)
    {
        int oper, start, end;
        cin >> oper >> start >> end;
        if (oper == 0)
            ltst.update(start - 1, end - 1);
        else
            cout << ltst.query(start - 1, end - 1) << endl;
    }
    return 0;
}