#include <iostream>
#include <string>
#include <deque>
using namespace std;

int main(void)
{
    // read
    int numPastures, maxDistrict;
    cin >> numPastures >> maxDistrict;
    string line;
    cin >> line;

    // prefix sum
    using pii = pair<int, int>;
    pii *prefixSum = new pii[numPastures + 1];
    prefixSum[0] = pii(0, 0);
    for (int i = 1; i <= numPastures; i++)
        prefixSum[i] = pii(prefixSum[i - 1].first + int(line[i - 1] == 'H'),
                           prefixSum[i - 1].second + int(line[i - 1] == 'G'));

    // Sliding window: min tie or G till i
    deque<pii> window;
    window.push_back(pii(-1, 0));
    auto better = [&prefixSum](pii p1, pii p2)
    {
        if (p1.second != p2.second)
            return p1.second < p2.second;
        int hVals = prefixSum[p1.first + 1].first - prefixSum[p2.first + 1].first;
        int gVals = prefixSum[p1.first + 1].second - prefixSum[p2.first + 1].second;
        return hVals <= gVals;
    };
    pii curr; // will be ans
    for (int i = 0; i < numPastures; i++)
    {
        while (window.front().first < i - maxDistrict)
            window.pop_front();
        int numH = prefixSum[i + 1].first - prefixSum[window.front().first + 1].first;
        int numG = prefixSum[i + 1].second - prefixSum[window.front().first + 1].second;
        curr = pii(i, window.front().second + (numH <= numG));
        while (!window.empty() && better(curr, window.back()))
            window.pop_back();
        window.push_back(curr);
    }

    // print and delete
    cout << curr.second << endl;
    delete[] prefixSum;
    return 0;
}