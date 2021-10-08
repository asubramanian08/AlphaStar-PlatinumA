#include <iostream>
#include <algorithm>
#include <vector>
#include <numeric>
using namespace std;

using pll = pair<long long, long long>;
long long orient(pll p1, pll p2, pll p3)
{
    return (p2.first - p1.first) * (p3.second - p1.second) -
           (p2.second - p1.second) * (p3.first - p1.first);
}
struct world
{
    pll point, goal;
};
struct frac
{
    long long numer, denom;
    //NA if value < 0
    bool operator<(const frac &f) const
    {
        long long num1 = numer * f.denom;
        long long num2 = f.numer * denom;
        if (num1 < 0 || num2 < 0)
            return num1 > num2;
        return num1 < num2;
    }
    void simplify(void)
    {
        long long GCD = gcd(numer, denom);
        numer /= GCD;
        denom /= GCD;
        if (denom < 0)
        {
            numer *= -1;
            denom *= -1;
        }
    }
};
frac slope(pll p1, pll p2)
{
    frac slp;
    slp.numer = p1.second - p2.second;
    slp.denom = p1.first - p2.first;
    slp.simplify();
    return slp;
}

int main(void)
{
    //read
    int numWorlds, worldNum;
    cin >> numWorlds;
    world *worlds = new world[numWorlds];
    for (int i = 0; i < numWorlds; i++)
    {
        worlds[i].point.first = i;
        cin >> worlds[i].point.second;
    }
    for (int i = 0; i < numWorlds; i++)
    {
        cin >> worldNum;
        worlds[i].goal = worlds[worldNum - 1].point;
    }

    //setup
    frac *answer = new frac[numWorlds];
    sort(worlds, worlds + numWorlds, [](world p1, world p2)
         { return p1.point.second < p2.point.second ||
                  (p1.point.second == p2.point.second &&
                   p1.point.first < p2.point.first); });
    int lo, hi;

    //left/lower hull
    vector<pll> hull;
    for (int i = 0; i < numWorlds; i++)
    {
        //form answer
        if (worlds[i].goal.second < worlds[i].point.second)
            goto updateLeftHull;
        answer[worlds[i].point.first] = slope(worlds[i].point, worlds[i].goal);
        if (hull.empty())
            goto updateLeftHull;
        lo = -1, hi = hull.size() - 1;
        while (hi - lo > 1)
            if (orient(hull[(lo + hi) / 2], hull[(lo + hi) / 2 + 1], worlds[i].goal) < 0)
                lo = (lo + hi) / 2;
            else
                hi = (lo + hi) / 2;
        if (hull[hi].first > worlds[i].point.first ||
            slope(hull[hi], worlds[i].goal) < slope(hull[hi], worlds[i].point))
            goto updateLeftHull;
        answer[worlds[i].point.first] = min(answer[worlds[i].point.first],
                                            slope(hull[hi], worlds[i].goal));

    updateLeftHull: //update hull
        while (hull.size() >= 2 && orient(hull[hull.size() - 2], hull[hull.size() - 1], worlds[i].point) >= 0)
            hull.pop_back();
        hull.push_back(worlds[i].point);
    }

    //right/upper hull
    hull.clear();
    for (int i = numWorlds - 1; i >= 0; i--)
    {
        //form answer
        if (worlds[i].goal.second > worlds[i].point.second)
            goto updateRightHull;
        answer[worlds[i].point.first] = slope(worlds[i].point, worlds[i].goal);
        if (hull.empty())
            goto updateRightHull;
        lo = -1, hi = hull.size() - 1;
        while (hi - lo > 1)
            if (orient(hull[(lo + hi) / 2], hull[(lo + hi) / 2 + 1], worlds[i].goal) < 0)
                lo = (lo + hi) / 2;
            else
                hi = (lo + hi) / 2;
        if (hull[hi].first < worlds[i].point.first ||
            slope(hull[hi], worlds[i].goal) < slope(hull[hi], worlds[i].point))
            goto updateRightHull;
        answer[worlds[i].point.first] = min(answer[worlds[i].point.first],
                                            slope(hull[hi], worlds[i].goal));

    updateRightHull: //update hull
        while (hull.size() >= 2 && orient(hull[hull.size() - 2], hull[hull.size() - 1], worlds[i].point) >= 0)
            hull.pop_back();
        hull.push_back(worlds[i].point);
    }

    //print and delete
    for (int i = 0; i < numWorlds; i++)
        if (answer[i].numer < 0)
            cout << -1 << endl;
        else
            cout << answer[i].numer << '/' << answer[i].denom << endl;
    delete[] worlds;
    delete[] answer;
    return 0;
}