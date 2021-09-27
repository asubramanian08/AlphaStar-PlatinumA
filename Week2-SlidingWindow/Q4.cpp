#include <iostream>
#include <algorithm>
#include <vector>
#include <list>
#include <map>
using namespace std;

using pii = pair<int, int>;
#define MAX_COWS 100010
#define BREEDS 8

int firstBit(int mask)
{
    for (int i = 0; i < BREEDS; i++)
        if (mask & (1 << i))
            return i;
    return BREEDS;
}
vector<int> *usefulMasks(int numCows, pii *cows, bool dir)
{
    vector<int> *masks = new vector<int>[numCows];
    list<int> breedOrder;
    for (int i = (dir ? 0 : numCows - 1); (dir ? i < numCows : i >= 0); (dir ? i++ : i--))
    {
        breedOrder.push_front(cows[i].second);
        int currMask = 0;
        list<int>::iterator iter = breedOrder.begin();
        while (iter != breedOrder.end())
            if (currMask & (1 << *iter))
                iter = breedOrder.erase(iter);
            else
            {
                masks[i].push_back(currMask |= (1 << *iter));
                iter++;
            }
    }
    return masks;
}

int main(void)
{
    //read
    int numCows, minCows;
    cin >> numCows >> minCows;
    pii *cows = new pii[numCows];
    for (int i = 0; i < numCows; i++)
        cin >> cows[i].first >> cows[i].second;
    sort(cows, cows + numCows);

    //breed prefix sum
    int prefixSum[MAX_COWS][BREEDS] = {0};
    for (int i = 1; i <= numCows; i++)
    {
        for (int j = 0; j < BREEDS; j++)
            prefixSum[i][j] = prefixSum[i - 1][j];
        prefixSum[i][--cows[i - 1].second]++;
    }

    //useful masks for a cow
    vector<int> *toQuery = usefulMasks(numCows, cows, true);
    vector<int> *toStore = usefulMasks(numCows, cows, false);

    //set mask -> dist
    int maxDist = -1;
    vector<int> v(BREEDS + 1);
    map<vector<int>, int> m;
    for (int oper = 0; oper < 2; oper++)
        for (int cow = 0; cow < numCows; cow++)
            for (int minOn = minCows; minOn < BREEDS; minOn++)
            {
                if ((oper ? toQuery : toStore)[cow].size() < minOn)
                    continue;
                int mask = v[BREEDS] = (oper ? toQuery : toStore)[cow][minOn - 1];
                int firstBreed = firstBit(mask);
                for (int i = 0; i < BREEDS; i++)
                    if (mask & (1 << i))
                        v[i] = prefixSum[cow + oper][i] - prefixSum[cow + oper][firstBreed];
                    else
                        v[i] = prefixSum[cow + oper][i];
                if (oper == 0 && m.find(v) == m.end())
                    m[v] = cows[cow].first;
                if (oper == 1 && m.find(v) != m.end())
                    maxDist = max(maxDist, cows[cow].first - m[v]);
            }

    //print and delete
    cout << maxDist << endl;
    delete[] cows;
    return 0;
}