#include <iostream>
#include <vector>
using namespace std;

#define MAX_COWS 100000

int main(void)
{
    // read
    int numCows;
    cin >> numCows;
    int *left = new int[numCows];
    for (int i = 0; i < numCows; i++)
        cin >> left[i];
    int *right = new int[numCows];
    for (int i = 0; i < numCows; i++)
        cin >> right[i];
    for (int i = 0; i < numCows; i++)
        left[i]--, right[i]--;

    // crossing pairs (right pos)
    vector<int> *friendly = new vector<int>[numCows];
    for (int i = 0; i < numCows; i++)
        for (int j = max(right[i] - 4, 0); j <= min(right[i] + 4, numCows - 1); j++)
            friendly[j].push_back(i);
    vector<int> crossings;
    for (int i = 0; i < numCows; i++)
        for (auto j = friendly[left[i]].rbegin(); j != friendly[left[i]].rend(); j++)
            crossings.push_back(*j);

    // longest increasing subsequence: crossings
    vector<int> DP; // DP[i]: end of LIS of length i
    for (int i = 0; i < crossings.size(); i++)
    {
        auto pos = lower_bound(DP.begin(), DP.end(), crossings[i]);
        if (pos == DP.end())
            DP.push_back(crossings[i]);
        else
            *pos = crossings[i];
    }

    // print and delete
    cout << DP.size() << endl;
    delete[] left;
    delete[] right;
    delete[] friendly;
    return 0;
}