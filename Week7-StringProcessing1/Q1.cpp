#include <iostream>
#include <set>
using namespace std;

int main(void)
{
    // read
    int numPoints;
    cin >> numPoints;
    using pdd = pair<double, double>;
    pdd *points = new pdd[numPoints];
    for (int i = 0; i < numPoints; i++)
        cin >> points[i].first >> points[i].second;

    // run: all lines, add to set
    set<double> slopes;
    bool vert = false, hori = false;
    for (int i = 0; i < numPoints; i++)
        for (int j = i + 1; j < numPoints; j++)
            if (points[i].first == points[j].first) // vertical
                vert = true;
            else if (points[i].second == points[j].second) // horizontal
                hori = true;
            else // any other case
                slopes.insert((points[i].second - points[j].second) /
                              (points[i].first - points[j].first));

    // print and delete
    cout << (slopes.size() + vert + hori) << endl;
    delete[] points;
    return 0;
}