// Space Table: Too much memory for 1 case
#include <iostream>
#include <cmath>
#include <vector>
#include <map>
using namespace std;

#define rangeMin(start, end, log) min(st[start][log], st[end - (1 << log) + 1][log])
#define queryLog(start, end) log2(end - start + 1)

int main(void)
{
    // read
    int length;
    cin >> length;
    int *biosequence = new int[length];
    for (int i = 0; i < length; i++)
        cin >> biosequence[i];

    // RMQ: sparse table
    int lenLog = log2(length) + 1;
    int **st = new int *[length];
    for (int i = 0; i < length; i++)
    {
        st[i] = new int[lenLog];
        st[i][0] = biosequence[i];
    }
    for (int j = 1; j < lenLog; j++)
        for (int i = 0; i + (1 << j) <= length; i++)
            st[i][j] = min(st[i][j - 1], st[i + (1 << (j - 1))][j - 1]);

    // framed intervals: monetized queue
    using pii = pair<int, int>;
    vector<pii> intervals;
    vector<int> mq /*monetize queue*/;
    map<int, vector<int> > possibleEnds;
    for (int i = length - 1; i >= 0; i--)
    {
        // interval starting at i
        int currRef = biosequence[i] - i;
        int end = possibleEnds[currRef].empty() ? i : possibleEnds[currRef].back();
        int currLog = queryLog(i, end);
        if (end != i && rangeMin(i, end, currLog) == biosequence[i])
            intervals.push_back(pii(i, end));
        // update monetized queue
        while (!mq.empty() && biosequence[mq.back()] < biosequence[i])
        {
            possibleEnds[biosequence[mq.back()] - mq.back()].pop_back();
            mq.pop_back();
        }
        mq.push_back(i);
        possibleEnds[currRef].push_back(i);
    }

    // empodia
    vector<pii> empodia;
    int minEnd = intervals.front().second + 1;
    for (int i = 0; i < intervals.size(); minEnd = min(minEnd, intervals[i].second), i++)
        if (intervals[i].second < minEnd)
            empodia.push_back(intervals[i]);

    // print and delete
    cout << empodia.size() << endl;
    for (int i = empodia.size() - 1; i >= 0; i--)
        cout << (empodia[i].first + 1) << ' '
             << (empodia[i].second + 1) << endl;
    delete[] biosequence;
    for (int i = 0; i < length; i++)
        delete[] st[i];
    delete[] st;
    return 0;
}