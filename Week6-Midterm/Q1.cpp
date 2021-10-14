#include <iostream>
#include <set>
#include <algorithm>
using namespace std;

using pii = pair<int, int>;
#define x first
#define y second
using rect = pair<pii, pii>;
int numRects;
#define mp make_pair

int sweepLine(rect *rects)
{
    // form data
    rect *hori = rects;
    sort(hori, hori + numRects);
    pair<pair<int, bool>, pii> *vert = new pair<pair<int, bool>, pii>[numRects * 2];
    for (int i = 0; i < numRects; i++)
    {
        vert[i * 2 + 0] = mp(mp(rects[i].first.x, true), pii(rects[i].first.y, rects[i].second.y));
        vert[i * 2 + 1] = mp(mp(rects[i].second.x, false), pii(rects[i].first.y, rects[i].second.y));
    }
    sort(vert, vert + numRects * 2);
    set<pair<pii, int> > on;

    // sweepline
    int ans = 0, currHori = 0;
    for (int i = 0; i < numRects * 2; i++)
    {
        // add lines
        while (currHori < numRects && hori[currHori].first.x < vert[i].x.first)
        {
            on.insert(mp(pii(hori[currHori].first.y, hori[currHori].second.y),
                         hori[currHori].second.x));
            currHori++;
        }
        // remove excess
        for (auto iter = on.begin(); iter != on.end();)
            if (iter->second <= vert[i].x.first)
                iter = on.erase(iter);
            else
                iter++;
        // exposed distance
        if (i < numRects * 2 - 1 && vert[i].x.first == vert[i + 1].x.first &&
            vert[i].y.second >= vert[i + 1].y.first)
            if (vert[i].x.second != vert[i + 1].x.second)
            {
                swap(vert[i].y.second, vert[i + 1].y.first);
                if (vert[i + 1].y.first > vert[i + 1].y.second)
                    swap(vert[i + 1].y.first, vert[i + 1].y.second);
            }
            else
            {
                vert[i + 1].y.first = vert[i].y.first;
                vert[i + 1].y.second = max(vert[i + 1].y.second, vert[i].y.second);
                continue;
            }
        int start = vert[i].y.first, end = vert[i].y.second;
        for (auto item : on)
        {
            if (item.first.first > end)
                break;
            if (item.first.first > start)
                ans += item.first.first - start;
            start = max(start, item.first.second);
        }
        ans += max(end - start, 0);
    }

    // delete and return
    delete[] vert;
    return ans;
}

int main(void)
{
    // read
    cin >> numRects;
    rect *rects = new rect[numRects];
    for (int i = 0; i < numRects; i++)
        cin >> rects[i].first.x >> rects[i].first.y >> rects[i].second.x >> rects[i].second.y;

    // sweep both hori and vert
    int parameter = sweepLine(rects);
    for (int i = 0; i < numRects; i++)
    {
        swap(rects[i].first.x, rects[i].first.y);
        swap(rects[i].second.x, rects[i].second.y);
    }
    parameter += sweepLine(rects);

    // print and delete
    cout << parameter << endl;
    delete[] rects;
    return 0;
}