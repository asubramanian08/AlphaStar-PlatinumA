#include <iostream>
#include <string>
#include <queue>
#include <vector>
using namespace std;

#define MAX_LETTERS 52
#define MAX_RMC 1000
#define mp make_pair

int main(void)
{
    //read
    int numCodes;
    cin >> numCodes;
    string *codeWords = new string[numCodes];
    for (int i = 0; i < numCodes; i++)
        cin >> codeWords[i];
    string text;
    cin >> text;

    //convert the chars to nums
    for (int i = 0; i < numCodes; i++)
        for (int j = 0; j < codeWords[i].size(); j++)
            if ('a' <= codeWords[i][j] && codeWords[i][j] <= 'z')
                codeWords[i][j] -= 'a';
            else
                codeWords[i][j] -= 'A' - MAX_LETTERS / 2;
    for (int i = 0; i < text.size(); i++)
        if ('a' <= text[i] && text[i] <= 'z')
            text[i] -= 'a';
        else
            text[i] -= 'A' - MAX_LETTERS / 2;

    //RMC starts
    using pii = pair<int, int>;
    queue<pii> nextLetter[MAX_LETTERS];
    int *wordIndex = new int[numCodes];
    for (int i = 0; i < numCodes; i++)
        nextLetter[codeWords[i][wordIndex[i] = codeWords[i].size() - 1]].push(mp(i, text.size()));
    vector<pii> starts;
    for (int i = text.size() - 1; i >= 0; i--)
        while (!nextLetter[text[i]].empty() && nextLetter[text[i]].front().second != i)
        {
            int word = nextLetter[text[i]].front().first;
            nextLetter[text[i]].pop();
            if (wordIndex[word] == 0)
            {
                starts.push_back(mp(word, i));
                nextLetter[codeWords[word][wordIndex[word]]].push(mp(word, i));
            }
            else
                nextLetter[codeWords[word][--wordIndex[word]]].push(mp(word, i));
        }

    //RMC ends
    vector<pair<pii, int> > RMCs;
    for (pii start : starts)
        for (int i = start.second, index = 0; i < start.second + MAX_RMC; i++)
            if (text[i] == codeWords[start.first][index] &&
                ++index == codeWords[start.first].size())
            {
                RMCs.push_back(mp(mp(start.second, i), start.first));
                break;
            }

    //DP[index] = max code word length ending at index
    vector<pii> *startLoc = new vector<pii>[text.size()];
    for (pair<pii, int> RMC : RMCs)
        startLoc[RMC.first.second].push_back(mp(RMC.first.first, codeWords[RMC.second].size()));
    int *DP = new int[text.size()];
    DP[0] = 0;
    for (int i = 1; i < text.size(); i++)
    {
        DP[i] = DP[i - 1];
        for (pii start : startLoc[i])
            DP[i] = max(DP[i], DP[start.first] + start.second);
    }

    //print and delete
    cout << DP[text.size() - 1] << endl;
    delete[] codeWords;
    delete[] wordIndex;
    delete[] startLoc;
    delete[] DP;
    return 0;
}