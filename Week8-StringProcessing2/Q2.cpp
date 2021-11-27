#include <iostream>
#include <string>
#include <algorithm>
using namespace std;

#define MAX_LEN 200000
#define MAX_LOG 18
#define mp make_pair
using pii = pair<int, int>;
int logInt2(int num)
{
    int ans = 1;
    while ((num >>= 1) != 0)
        ans++;
    return ans;
}
int suffixArray[MAX_LOG][MAX_LEN];
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
            location[j].first.second = suffixArray[i - 1][(j + dist) % str.size()];
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
    int strLen;
    cin >> strLen;
    string str;
    for (int i = 0; i < strLen; i += 72)
    {
        string temp;
        cin >> temp;
        str += temp;
    }

    // suffix array
    int ansRow = initSuffixArray(str);
    int firstPos = 0;
    for (int i = 1; i < strLen; i++)
        if (suffixArray[ansRow][firstPos] > suffixArray[ansRow][i])
            firstPos = i;

    // print
    cout << firstPos << endl;
    return 0;
}