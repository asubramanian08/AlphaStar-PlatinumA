#include <iostream>
#include <algorithm>
using namespace std;

using pii = pair<int, int>;

int main(void)
{
    // read
    int numEle;
    cin >> numEle;
    pii *arr = new pii[numEle];
    for (int i = 0; i < numEle; i++)
    {
        cin >> arr[i].first;
        arr[i].second = i;
    }
    sort(arr, arr + numEle);

    // loop it becomes a partition
    int *partitionAt = new int[numEle + 1];
    partitionAt[0] = partitionAt[numEle] = 0;
    int maxIndex = arr[0].second;
    for (int i = 1; i < numEle; i++)
    {
        maxIndex = max(maxIndex, arr[i - 1].second);
        partitionAt[i] = maxIndex - i + 1;
    }

    // ans: loops it take to sort
    long long ans = 0, curr;
    for (int i = 0; i < numEle; i++)
    {
        curr = max(partitionAt[i], partitionAt[i + 1]);
        curr += (curr == 0 && numEle > 1) ? 1 : 0;
        ans += curr;
    }

    // print and delete
    cout << ans << endl;
    delete[] arr;
    delete[] partitionAt;
    return 0;
}