#include <iostream>
#include <vector>
using namespace std;

using pii = pair<int, int>;
#define MOD 1000000007
#define LARGEST 1000000000

int ways(int length, int minVal, int window)
{
    if (length == 0)
        return 1;
    long long *DP = new long long[length + 2];
    DP[0] = 1;
    DP[1] = 1;
    for (int i = 2; i <= min(window, length); i++)
        DP[i] = (DP[i - 1] * (LARGEST - minVal + 1)) % MOD;
    if (length < window)
        return (DP[length] * (LARGEST - minVal + 1)) % MOD;
    long long valsPowWindow = 1;
    for (int i = 1; i < window; i++)
        valsPowWindow = (valsPowWindow * (LARGEST - minVal)) % MOD;
    for (int i = window + 1; i <= length + 1; i++)
    {
        DP[i] = DP[i - 1];
        DP[i] = (MOD + DP[i] - ((long long)DP[i - window - 1] * valsPowWindow) % MOD) % MOD;
        DP[i] = ((long long)DP[i] * (LARGEST - minVal)) % MOD;
        DP[i] = (DP[i] + DP[i - 1]) % MOD;
    }
    return DP[length + 1];
}

int main(void)
{
    //read
    int trainSize, window;
    cin >> trainSize >> window;
    vector<pii> mins;
    mins.push_back(pii(0, 0));
    int minimum;
    for (int i = window - 1; i < trainSize; i++)
    {
        cin >> minimum;
        if (mins.back().first == minimum)
            mins.back().second++;
        else
            mins.push_back(pii(minimum, 1));
    }
    mins.push_back(pii(0, 0));

    //ans: multiply sections
    long long ans = 1, length;
    for (int i = 1; i < mins.size() - 1; i++)
    {
        int figuredOut = (mins[i + 1].first > mins[i].first) + (mins[i - 1].first > mins[i].first);
        if (figuredOut == 2)
            length = max(0, mins[i].second - window - 1);
        else if (figuredOut == 0)
            length = mins[i].second + window - 1;
        else
            length = mins[i].second - 1;
        ans = (ans * ways(length, mins[i].first, window)) % MOD;
    }

    //print
    cout << ans << endl;
    return 0;
}