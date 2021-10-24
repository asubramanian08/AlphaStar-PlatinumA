#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <set>
#include <algorithm>
using namespace std;

// hash code segment
#define MULT 3391
#define MOD 1000000007
vector<long long> varHash;
map<string, int> varNum;
void nexthash(string code[3], int index, long long &hash)
{
    for (int i = 0; i < 3; i++)
    {
        if (varNum.find(code[i]) == varNum.end())
        {
            varNum[code[i]] = varNum.size();
            varHash.push_back(0);
        }
        varHash[varNum[code[i]]] += index * 2 + (i > 0) + 1;
        varHash[varNum[code[i]]] *= MULT;
        varHash[varNum[code[i]]] %= MOD;
    }
    hash = ((hash + varHash[varNum[code[0]]]) * MULT) % MOD;
    if (varHash[varNum[code[1]]] < varHash[varNum[code[2]]])
    {
        hash = ((hash + varHash[varNum[code[1]]]) * MULT) % MOD;
        hash = ((hash + varHash[varNum[code[2]]]) * MULT) % MOD;
    }
    else
    {
        hash = ((hash + varHash[varNum[code[2]]]) * MULT) % MOD;
        hash = ((hash + varHash[varNum[code[1]]]) * MULT) % MOD;
    }
}

int main(void)
{
    // read
    int rLines, hLines;
    cin >> rLines >> hLines;
    char junk;
    string(*rCode)[3] = new string[rLines][3];
    for (int i = 0; i < rLines; i++)
        cin >> rCode[i][0] >> junk >> rCode[i][1] >> junk >> rCode[i][2];
    string(*hCode)[3] = new string[hLines][3];
    for (int i = 0; i < hLines; i++)
        cin >> hCode[i][0] >> junk >> hCode[i][1] >> junk >> hCode[i][2];

    // hash all rCode code segments
    set<long long> segHashes;
    for (int i = 0; i < rLines; i++)
    {
        varHash.clear();
        varNum.clear();
        long long currHash = 0;
        for (int j = i; j < rLines; j++)
        {
            nexthash(rCode[j], j - i, currHash);
            segHashes.insert(currHash);
        }
    }

    // matches in hCode
    int maxLen = 0;
    for (int i = 0; i < hLines; i++)
    {
        varHash.clear();
        varNum.clear();
        long long currHash = 0;
        for (int j = i; j < hLines; j++)
        {
            nexthash(hCode[j], j - i, currHash);
            if (segHashes.find(currHash) != segHashes.end())
                maxLen = max(maxLen, j - i + 1);
            else
                break;
        }
    }

    // print and delete
    cout << maxLen << endl;
    delete[] rCode;
    delete[] hCode;
    return 0;
}