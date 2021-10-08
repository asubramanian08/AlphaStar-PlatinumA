#include <iostream>
#include <vector>
using namespace std;

using pll = pair<long long, long long>;
#define MULT 100000

//convex hull
long long orient(pll p1, pll p2, pll p3)
{
    return (p2.first - p1.first) * (p3.second - p1.second) -
           (p2.second - p1.second) * (p3.first - p1.first);
}
vector<pll> upper_hull(pll *points, int numPositions)
{
    vector<pll> hull;
    for (int i = 0; i < numPositions; i++)
    {
        while (hull.size() >= 2 && orient(hull[hull.size() - 2], hull[hull.size() - 1], points[i]) >= 0)
            hull.pop_back();
        hull.push_back(points[i]);
    }
    return hull;
}

int main(void)
{
    //read
    int numPositions;
    cin >> numPositions;
    long long *height = new long long[numPositions];
    for (int i = 0; i < numPositions; i++)
        cin >> height[i];

    //upper hull
    pll *points = new pll[numPositions + 2];
    for (int i = 0; i < numPositions; i++)
        points[i + 1] = pll(i, height[i]);
    points[0] = pll(-1, 0);
    points[numPositions + 1] = pll(numPositions, 0);
    vector<pll> hull = upper_hull(points, numPositions + 2);

    //expected
    long long *expected = new long long[numPositions];
    for (int i = 0; i < numPositions; i++)
    {
        int high = lower_bound(hull.begin(), hull.end(), pll(i, 0)) - hull.begin();
        expected[i] = (hull[high].first - i) * hull[high - 1].second +
                      (i - hull[high - 1].first) * hull[high].second;
        expected[i] = ((long double)(expected[i] * MULT) / (hull[high].first - hull[high - 1].first));
    }

    //print and delete
    for (int i = 0; i < numPositions; i++)
        cout << expected[i] << endl;
    delete[] height;
    delete[] points;
    delete[] expected;
    return 0;
}