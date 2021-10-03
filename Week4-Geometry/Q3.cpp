#include <iostream>
#include <cmath>
#include <algorithm>
#include <queue>
using namespace std;

int main(void)
{
    //read
    int numCows, radius;
    cin >> numCows >> radius;
    using pii = pair<int, int>;
    pii *cows = new pii[numCows];
    for (int i = 0; i < numCows; i++)
        cin >> cows[i].first >> cows[i].second;

    //start and end angles
    using pdd = pair<double, double>;
    pdd *angles = new pdd[numCows];
    for (int i = 0; i < numCows; i++)
    {
        double theta1 = atan2(cows[i].second, cows[i].first);
        double theta2 = acos(radius / hypot(cows[i].first, cows[i].second));
        angles[i] = pdd(theta1 - theta2, theta1 + theta2);
        if (angles[i].first < 0)
        {
            angles[i].first += 2 * M_PI;
            angles[i].second += 2 * M_PI;
        }
    }
    sort(angles, angles + numCows);

    //sweep tangent line
    priority_queue<double, vector<double>, greater<double> > ends;
    long long cowPairs = 0;
    for (int run = 0; run < 2; run++)
        for (int i = 0; i < numCows; i++)
        {
            while (!ends.empty() && ends.top() < angles[i].first)
                ends.pop();
            if (run)
                cowPairs += ends.size();
            ends.push(angles[i].second);
            angles[i].first += 2 * M_PI;
            angles[i].second += 2 * M_PI;
        }

    //print and delete
    cout << cowPairs << endl;
    delete[] cows;
    delete[] angles;
    return 0;
}