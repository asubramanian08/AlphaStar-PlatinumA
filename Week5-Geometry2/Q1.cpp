#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int numStones;
using pii = pair<int, int>;

//convex hull
long long orient(pii p1, pii p2, pii p3)
{
    return (long long)(p2.first - p1.first) * (long long)(p3.second - p1.second) -
           (long long)(p2.second - p1.second) * (long long)(p3.first - p1.first);
}
vector<pii> upper_hull(pii *stones)
{
    sort(stones, stones + numStones);
    vector<pii> hull;
    for (int i = numStones - 1; i >= 0; i--)
    {
        while (hull.size() >= 2 && orient(hull[hull.size() - 2], hull[hull.size() - 1], stones[i]) <= 0)
            hull.pop_back();
        hull.push_back(stones[i]);
    }
    reverse(hull.begin(), hull.end());
    return hull;
}
vector<pii> lower_hull(pii *stones)
{
    sort(stones, stones + numStones);
    vector<pii> hull;
    for (int i = 0; i < numStones; i++)
    {
        while (hull.size() >= 2 && orient(hull[hull.size() - 2], hull[hull.size() - 1], stones[i]) <= 0)
            hull.pop_back();
        hull.push_back(stones[i]);
    }
    return hull;
}

int score(vector<pii> uHull, vector<pii> lHull, pii *stones)
{
    int ans = 0, high;
    for (int i = 0; i < numStones; i++)
    {
        //upper hull
        high = lower_bound(uHull.begin(), uHull.end(), stones[i]) - uHull.begin();
        if (high == 0 && (uHull[high].first == stones[i].first &&
                          uHull[high].second < stones[i].second))
            high++;
        if (high == 0 || high == uHull.size())
            continue;
        if (orient(uHull[high - 1], uHull[high], stones[i]) > 0)
            continue;
        //lower_hull
        high = lower_bound(lHull.begin(), lHull.end(), stones[i]) - lHull.begin();
        if (high == 0 && (lHull[high].first == stones[i].first &&
                          lHull[high].second > stones[i].second))
            high++;
        if (high == 0 || high == lHull.size())
            continue;
        if (orient(lHull[high - 1], lHull[high], stones[i]) < 0)
            continue;
        //change
        ans++;
    }
    return ans;
}

int main(void)
{
    //read
    cin >> numStones;
    pii *stonesA = new pii[numStones];
    for (int i = 0; i < numStones; i++)
        cin >> stonesA[i].first >> stonesA[i].second;
    pii *stonesB = new pii[numStones];
    for (int i = 0; i < numStones; i++)
        cin >> stonesB[i].first >> stonesB[i].second;

    //captured
    int capturedA = score(upper_hull(stonesA), lower_hull(stonesA), stonesB);
    int capturedB = score(upper_hull(stonesB), lower_hull(stonesB), stonesA);

    //print and delete
    cout << capturedA << ' ' << capturedB << endl;
    delete[] stonesA;
    delete[] stonesB;
    return 0;
}