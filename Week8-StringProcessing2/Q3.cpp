#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
#include <queue>
using namespace std;

// suffix array
#define MAX_LEN 100000
#define MAX_LOG 17
#define mp make_pair
using pii = pair<int, int>;
#define NA -1
int logInt2(int num)
{
    int ans = 1;
    while ((num >>= 1) != 0)
        ans++;
    return ans;
}
int suffixArray[MAX_LOG][MAX_LEN];
pii posInfo[MAX_LEN]; // first: left in str; second: str number
int initSuffixArray(const string &str)
{
    int lg = logInt2(str.size());
    pair<pii, int> *location = new pair<pii, int>[str.size()];
    for (int i = 0; i < str.size(); i++)
        suffixArray[0][i] = str[i] - 'a';
    for (int i = 1, dist = 1; i < lg; i++, dist *= 2)
    {
        for (int j = 0; j < str.size(); j++)
        {
            location[j].first.first = suffixArray[i - 1][j];
            location[j].first.second = dist < posInfo[j].first ? suffixArray[i - 1][j + dist] : NA;
            location[j].second = j;
        }
        sort(location, location + str.size());
        for (int j = 0, pos = 0; j < str.size(); j++)
        {
            pos += (j != 0) && (location[j].first != location[j - 1].first);
            suffixArray[i][location[j].second] = pos;
        }
    }
    delete[] location;
    return lg - 1;
}

int main(void)
{
    // read
    int numNames;
    cin >> numNames;
    string names;
    for (int i = 0; i < numNames; i++)
    {
        string currName;
        cin >> currName;
        for (int j = 0; j < currName.size(); j++)
            posInfo[names.size() + j] = mp(currName.size() - j, i);
        names += currName;
    }

    // suffixArray: prev and next suffix from different name
    int ansRow = initSuffixArray(names);
    vector<int> *tempOrder = new vector<int>[names.size()];
    for (int i = 0; i < names.size(); i++)
        tempOrder[suffixArray[ansRow][i]].push_back(i);
    int *orderedSuffix = new int[names.size()];
    queue<int> addTemp;
    for (int i = 0; i < names.size(); i++)
    {
        for (int num : tempOrder[i])
            addTemp.push(num);
        orderedSuffix[i] = addTemp.front();
        addTemp.pop();
    }
    delete[] tempOrder;
    int *prev = new int[names.size()], *next = new int[names.size()], lastSwitch;
    prev[0] = lastSwitch = NA;
    for (int i = 1; i < names.size(); prev[i] = lastSwitch, i++)
        if (posInfo[orderedSuffix[i]].second != posInfo[orderedSuffix[i - 1]].second)
            lastSwitch = i - 1;
    next[names.size() - 1] = lastSwitch = NA;
    for (int i = names.size() - 2; i >= 0; next[i] = lastSwitch, i--)
        if (posInfo[orderedSuffix[i]].second != posInfo[orderedSuffix[i + 1]].second)
            lastSwitch = i + 1;

    // first unique prefix of each suffix
    int *firstUnique = new int[names.size()];
    for (int i = 0; i < names.size(); i++)
        firstUnique[i] = 0;
    for (int i = 0; i < names.size(); i++)
        for (int run = 0; run < 3; run++)
        {

            int pos2 = run ? prev[i] : next[i], pos1 = i;
            pos2 = pos2 == NA ? NA : orderedSuffix[pos2], pos1 = orderedSuffix[pos1];
            if (run == 2)
                if (i == names.size() - 1 || posInfo[pos1].second != posInfo[orderedSuffix[i + 1]].second)
                    pos2 = NA;
                else
                    pos2 = orderedSuffix[i + 1];

            if (pos2 == NA)
                continue;
            int maxLen = min(posInfo[pos1].first, posInfo[pos2].first), len = 0;
            for (int j = ansRow; j >= 0; j--)
                if (suffixArray[j][pos1 + len] == suffixArray[j][pos2 + len])
                    len += 1 << j;
            firstUnique[pos1] = max(firstUnique[pos1], min(len, maxLen));
        }

    // print and delete
    int pos = 0;
    for (int i = 0; i < numNames; i++)
    {
        int nameEnd = pos + posInfo[pos].first, uniqueCt = 0;
        while (pos < nameEnd)
            uniqueCt += posInfo[pos].first - firstUnique[pos], pos++;
        cout << uniqueCt << endl;
    }
    delete[] orderedSuffix;
    delete[] prev;
    delete[] next;
    delete[] firstUnique;
    return 0;
}