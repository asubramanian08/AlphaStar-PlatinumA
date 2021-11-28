#include <iostream>
#include <algorithm>
using namespace std;

using pii = pair<int, int>;

// MinSegTree
#define BIG_NUM 100000000000
class MinSegTree
{
private:
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
        tree[pos] = min(tree[2 * pos + 0], tree[2 * pos + 1]);
    }

public:
    MinSegTree(int size, long long *arr)
    {
        tree = new long long[size * 4];
        cover = new pii[size * 4];
        init_tree(1, 0, size - 1, arr);
    }
    ~MinSegTree()
    {
        delete[] tree;
        delete[] cover;
    }
    long long query(int left, int right, int pos = 1)
    {
        if (left > right || cover[pos].second < left || right < cover[pos].first)
            return BIG_NUM;
        if (left <= cover[pos].first && cover[pos].second <= right)
            return tree[pos];
        return min(query(left, right, 2 * pos + 0),
                   query(left, right, 2 * pos + 1));
    }
    void update(int node, long long set, int pos = 1)
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

struct sling
{
    int start, end, time, endPos;
    int operator<(const sling &s) const
    {
        if (start == s.start)
            return end < s.end;
        return start < s.start;
    }
};
struct query
{
    int start, end, questNum;
    int operator<(const query &q) const
    {
        if (start == q.start)
            return end < q.end;
        return start < q.start;
    }
};
long long slingTime(sling slingShot, bool right, bool upper)
{
    long long ans = slingShot.time;
    ans += right ? +slingShot.start : -slingShot.start;
    ans += upper ? +slingShot.end : -slingShot.end;
    return ans;
}

int main(void)
{
    // read
    int numSlingShots, numQueries;
    cin >> numSlingShots >> numQueries;
    sling *slingShots = new sling[numSlingShots];
    for (int i = 0; i < numSlingShots; i++)
        cin >> slingShots[i].start >> slingShots[i].end >> slingShots[i].time;
    sort(slingShots, slingShots + numSlingShots);
    query *queries = new query[numQueries];
    for (int i = 0; i < numQueries; i++)
    {
        cin >> queries[i].start >> queries[i].end;
        queries[i].questNum = i;
    }
    sort(queries, queries + numQueries);

    // set up
    long long *temp = new long long[numSlingShots];
    fill(temp, temp + numSlingShots, 0);
    MinSegTree left(numSlingShots, temp), right(numSlingShots, temp);
    pii *slingByY = new pii[numSlingShots]; // end, pos
    for (int i = 0; i < numSlingShots; i++)
        slingByY[i] = pii(slingShots[i].end, i);
    sort(slingByY, slingByY + numSlingShots);
    for (int i = 0; i < numSlingShots; i++)
        slingShots[slingByY[i].second].endPos = i;
    long long *ans = new long long[numQueries], slingPos;
    for (int i = 0; i < numQueries; i++)
        ans[queries[i].questNum] = abs(queries[i].start - queries[i].end);

    // run MinSegTree: query upper
    for (int i = 0; i < numSlingShots; i++)
    {
        left.update(i, BIG_NUM);
        right.update(i, slingTime(slingShots[slingByY[i].second], true, true));
    }
    slingPos = 0;
    for (int i = 0; i < numQueries; i++)
    {
        while (slingPos < numSlingShots && slingShots[slingPos].start < queries[i].start)
        {
            left.update(slingShots[slingPos].endPos, slingTime(slingShots[slingPos], false, true));
            right.update(slingShots[slingPos].endPos, BIG_NUM);
            slingPos++;
        }
        int pos = lower_bound(slingByY, slingByY + numSlingShots, pii(queries[i].end, 0)) - slingByY;
        long long leftVal = left.query(pos, numSlingShots - 1) + queries[i].start - queries[i].end,
                  rightVal = right.query(pos, numSlingShots - 1) - queries[i].start - queries[i].end;
        ans[queries[i].questNum] = min(ans[queries[i].questNum], min(leftVal, rightVal));
    }

    // run MinSegTree: query lower
    for (int i = 0; i < numSlingShots; i++)
    {
        left.update(i, BIG_NUM);
        right.update(i, slingTime(slingShots[slingByY[i].second], true, false));
    }
    slingPos = 0;
    for (int i = 0; i < numQueries; i++)
    {
        while (slingPos < numSlingShots && slingShots[slingPos].start < queries[i].start)
        {
            left.update(slingShots[slingPos].endPos, slingTime(slingShots[slingPos], false, false));
            right.update(slingShots[slingPos].endPos, BIG_NUM);
            slingPos++;
        }
        int pos = lower_bound(slingByY, slingByY + numSlingShots, pii(queries[i].end, 0)) - slingByY;
        long long leftVal = left.query(0, pos - 1) + queries[i].start + queries[i].end,
                  rightVal = right.query(0, pos - 1) - queries[i].start + queries[i].end;
        ans[queries[i].questNum] = min(ans[queries[i].questNum], min(leftVal, rightVal));
    }

    // print and delete
    for (int i = 0; i < numQueries; i++)
        cout << ans[i] << endl;
    delete[] slingShots;
    delete[] queries;
    delete[] temp;
    delete[] slingByY;
    delete[] ans;
    return 0;
}