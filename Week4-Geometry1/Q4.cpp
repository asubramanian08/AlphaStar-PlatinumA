#include <iostream>
#include <map>
#include <vector>
#include <algorithm>
#include <set>
#include <queue>
using namespace std;

//BIT or fenwick tree
class BIT
{
private:
    int *bTree;
    int elements;

public:
    BIT(int elem);
    BIT(int elem, int *initialize, int numInit);
    ~BIT() { delete[] bTree; }
    int query(int node);
    void updt(int node, int cng);
};
BIT::BIT(int elem)
{
    elements = elem;
    bTree = new int[elem + 1];
    for (int i = 1; i <= elem; i++)
        bTree[i] = 0;
}
BIT::BIT(int elem, int *initialize, int numInit)
{
    elements = elem;
    bTree = new int[elem + 1];
    for (int i = 1; i <= elem; i++)
        bTree[i] = 0;
    for (int i = 0; i < numInit; i++)
        updt(i, initialize[i]);
}
int BIT::query(int node)
{
    if (node == -1)
        return 0;
    int sum = 0;
    for (int i = node + 1; i > 0; i -= (i & -i))
        sum += bTree[i];
    return sum;
}
void BIT::updt(int node, int cng)
{
    for (int i = node + 1; i <= elements; i += (i & -i))
        bTree[i] += cng;
}

using pii = pair<int, int>;
map<int, vector<pair<int, char> > > byX, byY;
#define mp make_pair
#define RANGE 1000000000

vector<pii> formPath(pii pos, int dir)
{
    vector<pii> path;
    path.push_back(pos);
    while (true)
    {
        //next position
        auto iter = lower_bound((dir % 2 ? byY[pos.second] : byX[pos.first]).begin(),
                                (dir % 2 ? byY[pos.second] : byX[pos.first]).end(),
                                mp(dir % 2 ? pos.first : pos.second, '\0'));
        int loc = iter - (dir % 2 ? byY[pos.second] : byX[pos.first]).begin();
        loc += dir < 2 ? 1 : -1;
        pii nextPos = pos;
        if (loc < 0)
            (dir % 2 ? nextPos.first : nextPos.second) = -(RANGE + 1);
        else if (loc >= (dir % 2 ? byY[pos.second] : byX[pos.first]).size())
            (dir % 2 ? nextPos.first : nextPos.second) = RANGE + 1;
        else
            (dir % 2 ? nextPos.first : nextPos.second) = (dir % 2 ? byY[pos.second] : byX[pos.first])[loc].first;
        path.push_back(nextPos);

        //continue path
        if (loc < 0 || loc >= (dir % 2 ? byY[pos.second] : byX[pos.first]).size())
            break;
        if ((dir % 2 ? byY[pos.second] : byX[pos.first])[loc].second == '/')
            dir = (dir + (dir % 2 != 0 ? 3 : 1)) % 4;
        else if ((dir % 2 ? byY[pos.second] : byX[pos.first])[loc].second == '\\')
            dir = (dir + (dir % 2 == 0 ? 3 : 1)) % 4;
        else
            break;
        pos = nextPos;
    }
    return path;
}

vector<pair<pii, pii> > hor(vector<pii> path)
{
    vector<pair<pii, pii> > segment;
    for (int i = 1; i < path.size(); i++)
        if (path[i].second == path[i - 1].second)
            segment.push_back(mp(path[i - 1], path[i]));
    return segment;
}
vector<pair<pii, pii> > ver(vector<pii> path)
{
    vector<pair<pii, pii> > segment;
    for (int i = 1; i < path.size(); i++)
        if (path[i].first == path[i - 1].first)
            segment.push_back(mp(path[i - 1], path[i]));
    return segment;
}

int sweepLine(vector<pair<pii, pii> > hors, vector<pair<pii, pii> > vers)
{
    //set up
    for (int i = 0; i < hors.size(); i++)
        if (hors[i].first > hors[i].second)
            swap(hors[i].first, hors[i].second);
    for (int i = 0; i < vers.size(); i++)
        if (vers[i].first > vers[i].second)
            swap(vers[i].first, vers[i].second);
    vector<int> ys;
    for (int i = 0; i < hors.size(); i++)
        ys.push_back(hors[i].first.second);
    for (int i = 0; i < vers.size(); i++)
    {
        ys.push_back(vers[i].first.second);
        ys.push_back(vers[i].second.second);
    }
    sort(ys.begin(), ys.end());
    ys.resize(unique(ys.begin(), ys.end()) - ys.begin());
    for (int i = 0; i < hors.size(); i++)
        hors[i].first.second = hors[i].second.second =
            lower_bound(ys.begin(), ys.end(), hors[i].first.second) - ys.begin();
    for (int i = 0; i < vers.size(); i++)
    {
        vers[i].first.second = lower_bound(ys.begin(), ys.end(), vers[i].first.second) - ys.begin();
        vers[i].second.second = lower_bound(ys.begin(), ys.end(), vers[i].second.second) - ys.begin();
    }
    sort(hors.begin(), hors.end());
    sort(vers.begin(), vers.end());
    priority_queue<pii, vector<pii>, greater<pii> > pq;
    int currHor = 0;

    //sweep
    BIT bit(ys.size());
    int ans = 0;
    for (pair<pii, pii> &verLine : vers)
    {
        while (currHor < hors.size() && hors[currHor].first.first < verLine.first.first)
        {
            pq.push(hors[currHor].second);
            bit.updt(hors[currHor].first.second, 1);
            currHor++;
        }
        while (!pq.empty() && pq.top().first <= verLine.first.first)
        {
            bit.updt(pq.top().second, -1);
            pq.pop();
        }
        ans += bit.query(verLine.second.second) - bit.query(verLine.first.second - 1);
    }
    return ans;
}

int main(void)
{
    //read
    int numMirrors;
    pii barn;
    cin >> numMirrors >> barn.first >> barn.second;
    for (int i = 0; i < numMirrors; i++)
    {
        pii pos;
        char mirror;
        cin >> pos.first >> pos.second >> mirror;
        byX[pos.first].push_back(mp(pos.second, mirror));
        byY[pos.second].push_back(mp(pos.first, mirror));
    }
    byX[barn.first].push_back(mp(barn.second, 'B'));
    byY[barn.second].push_back(mp(barn.first, 'B'));
    byX[0].push_back(mp(0, 'L'));
    byY[0].push_back(mp(0, 'L'));

    //sort positions
    for (auto &vec : byX)
        sort(vec.second.begin(), vec.second.end());
    for (auto &vec : byY)
        sort(vec.second.begin(), vec.second.end());

    //form the paths (laser + 4 directions of barn)
    vector<pii> laserPath = formPath(pii(0, 0), 0);
    vector<pii> barnPath[4];
    for (int i = 0; i < 4; i++)
        barnPath[i] = formPath(barn, i);

    //print
    int ans = 0;
    for (int i = 0; i < 4; i++)
    {
        int currAns = sweepLine(hor(laserPath), ver(barnPath[i])) +
                      sweepLine(hor(barnPath[i]), ver(laserPath));
        if (barnPath[i].front() == barnPath[i].back())
            ans += currAns;
        else
            ans += 2 * currAns;
    }
    cout << (ans / 2) << endl;
    return 0;
}