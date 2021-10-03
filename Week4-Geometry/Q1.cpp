#include <iostream>
#include <algorithm>
using namespace std;

#define MAX_TREES 300
using pll = pair<long long, long long>;
long long orient(pll p1, pll p2, pll p3)
{
    return (p2.first - p1.first) * (p3.second - p1.second) -
           (p2.second - p1.second) * (p3.first - p1.first);
}

int main(void)
{
    int numTrees;
    cin >> numTrees;
    pll *trees = new pll[numTrees];
    for (int i = 0; i < numTrees; i++)
        cin >> trees[i].first >> trees[i].second;
    sort(trees, trees + numTrees);

    //trees underneath a line
    int beneathCount[MAX_TREES][MAX_TREES] = {0};
    for (int i = 0; i < numTrees; i++)
        for (int j = i + 1; j < numTrees; j++)
            for (int k = 0; k < numTrees; k++)
                if (k != i && k != j &&
                    trees[i].first <= trees[k].first &&
                    trees[k].first < trees[j].first &&
                    orient(trees[i], trees[j], trees[k]) < 0)
                    beneathCount[i][j] = ++beneathCount[j][i];

    //triangles (# triangles for # trees inside)
    int *triangles = new int[numTrees];
    for (int i = 0; i < numTrees; i++)
        triangles[i] = 0;
    for (int i = 0; i < numTrees; i++)
        for (int j = i + 1; j < numTrees; j++)
            for (int k = i + 1; k < j; k++)
                if (trees[i].first == trees[k].first)
                    triangles[beneathCount[k][j] - beneathCount[i][j] - 1]++;
                else if (trees[k].first == trees[j].first)
                    triangles[beneathCount[i][j] - beneathCount[i][k]]++;
                else if (orient(trees[i], trees[j], trees[k]) < 0)
                    triangles[beneathCount[i][j] - beneathCount[i][k] - beneathCount[k][j] - 1]++;
                else
                    triangles[beneathCount[i][k] + beneathCount[k][j] - beneathCount[i][j]]++;

    //print and delete
    for (int i = 0; i < numTrees - 2; i++)
        cout << triangles[i] << endl;
    delete[] trees;
    delete[] triangles;
    return 0;
}