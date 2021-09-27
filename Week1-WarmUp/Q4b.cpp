//fix: break apart a rock's range if it crosses fence 0
#include <iostream>
#include <algorithm>
#include <vector>
#include <cmath>
using namespace std;

using pii = pair<int, int>;
#define x first
#define y second
#define MAX_DEG 360
using pib = pair<int, bool>;
pii FJ;

double degrees(double yCord, double xCord)
{
    double rad = atan2(yCord - FJ.y, xCord - FJ.x);
    double deg = rad * 180 / M_PI;
    if (deg < 0)
        deg += MAX_DEG;
    return deg;
}
double earlierDeg(double deg1, double deg2)
{
    double dist = deg2 - deg1;
    if (dist < 0)
        dist += MAX_DEG;
    if (dist < MAX_DEG / 2)
        return deg1;
    return deg2;
}
double laterDeg(double deg1, double deg2) { return earlierDeg(deg1, deg2) == deg1 ? deg2 : deg1; }

int main(void)
{
    //read general
    int fenceLen, numRocks;
    cin >> fenceLen >> numRocks;
    cin >> FJ.x >> FJ.y;

    //fence degree values
    double *fence = new double[4 * fenceLen];
    for (int i = 0; i < fenceLen; i++)
    {
        fence[0 * fenceLen + i] = degrees(0, 0 + i);
        fence[1 * fenceLen + i] = degrees(0 + i, fenceLen);
        fence[2 * fenceLen + i] = degrees(fenceLen - i, 0);
        fence[3 * fenceLen + i] = degrees(fenceLen, fenceLen - i);
    }
    sort(fence, fence + 4 * fenceLen);

    //rock fence covered
    vector<pib> covered;
    double startDeg, endDeg, deg;
    int numVerticies, xCord, yCord;
    for (int i = 0; i < numRocks; i++)
    {
        cin >> numVerticies;
        for (int j = 0; j < numVerticies; j++)
        {
            cin >> xCord >> yCord;
            deg = degrees(yCord, xCord);
            if (j == 0)
                startDeg = endDeg = deg;
            startDeg = earlierDeg(startDeg, deg);
            endDeg = laterDeg(endDeg, deg);
        }
        int min = lower_bound(fence, fence + 4 * fenceLen, startDeg) - fence;
        int max = upper_bound(fence, fence + 4 * fenceLen, endDeg) - fence - 1;
        covered.push_back(pib(min, true));
        covered.push_back(pib(max, false));
        if (min > max)
        {
            covered.push_back(pib(fenceLen * 4 - 1, false));
            covered.push_back(pib(0, true));
        }
    }
    covered.push_back(pib(fenceLen * 4, false));
    sort(covered.begin(), covered.end());

    //sweep circle - viewable fence
    int open = 0, ans = 0, last = -1;
    for (int i = 0; i < covered.size(); i++)
    {
        if (open == 0 && last != covered[i].first)
            ans += (covered[i].first - last - 1 + 4 * fenceLen) % (4 * fenceLen);
        covered[i].second ? open++ : open--;
        last = covered[i].first;
    }

    //print and delete
    cout << ans << endl;
    delete[] fence;
    return 0;
}