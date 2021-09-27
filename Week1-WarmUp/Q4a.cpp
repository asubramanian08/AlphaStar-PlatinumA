//sumbitted 19/20 test 10 was 23 not 0
#include <iostream>
#include <algorithm>
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
    pib *covered = new pib[numRocks * 2];
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
        // auto pos = lower_bound(fence, fence + 4 * fenceLen, startDeg);
        // val1 = (pos != fence + 4 * fenceLen) ? pos - fence : 0;
        // pos = upper_bound(fence, fence + 4 * fenceLen, endDeg);
        // val2 = (pos != fence) ? pos - fence - 1 : 4 * fenceLen - 1;
        covered[0 * numRocks + i] = pib(lower_bound(fence, fence + 4 * fenceLen, startDeg) - fence, true);
        covered[1 * numRocks + i] = pib(upper_bound(fence, fence + 4 * fenceLen, endDeg) - fence - 1, false);
    }
    sort(covered, covered + numRocks * 2);

    //sweep circle - viewable fence
    int open = 0, ans = 0, last;
    for (int run = 0; run < 2; run++)
        for (int i = 0; i < numRocks * 2; i++)
        {
            if (run == 1 && open == 0 && last != covered[i].first)
                ans += (covered[i].first - last - 1 + 4 * fenceLen) % (4 * fenceLen);
            if (covered[i].second)
                open++;
            else if (open > 0)
                open--;
            last = covered[i].first;
        }

    //print and delete
    cout << ans << endl;
    delete[] fence;
    delete[] covered;
    return 0;
}