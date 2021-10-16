#include <iostream>
#include <deque>
using namespace std;

int main(void)
{
    //read
    int numBoxes, maxJump;
    cin >> numBoxes >> maxJump;
    long long *boxes = new long long[numBoxes];
    for (int i = 0; i < numBoxes; i++)
        cin >> boxes[i];

    //prefix sum or positive numbers
    long long *prefixSum = new long long[numBoxes];
    prefixSum[0] = max(0ll, boxes[0]);
    for (int i = 1; i < numBoxes; i++)
        prefixSum[i] = prefixSum[i - 1] + max(boxes[i], 0ll);

    //DP with sliding window
    using pli = pair<long long, int>;
    deque<pli> d;
    d.push_back(pli(0, -1));
    long long bestPivot = 0, insertVal = 0;
    auto rangeSum = [&prefixSum](int start, int end)
    { return (start == -1 ? 0 : prefixSum[start]) - (end == 0 ? 0 : prefixSum[end]); };
    for (int i = 0; i < numBoxes; i++)
    {
        while (!d.empty() && d.front().second + maxJump < i)
            d.pop_front();
        long long curr = d.front().first + boxes[i] + rangeSum(i - 1, d.front().second + 1);
        bestPivot = max(bestPivot, curr);
        if (i != numBoxes - 1)
            bestPivot = max(bestPivot, curr + boxes[i + 1]);
        while (!d.empty() && insertVal > d.back().first + rangeSum(i, d.back().second + 1))
            d.pop_back();
        d.push_back(pli(insertVal, i - 1));
        insertVal = curr + boxes[i + 1];
    }

    //print and delete
    cout << bestPivot << endl;
    delete[] boxes;
    delete[] prefixSum;
    return 0;
}