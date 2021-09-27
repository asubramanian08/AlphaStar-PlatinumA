#include <iostream>
using namespace std;

int main(void)
{
    //read
    int numBayles;
    cin >> numBayles;
    long long *bayles = new long long[numBayles];
    for (int i = 0; i < numBayles; i++)
        cin >> bayles[i];

    //greedy (first think DP)
    long long cow1 = 0, cow2 = 0, temp;
    for (int i = numBayles - 1; i >= 0; i--)
    {
        if (cow1 > bayles[i] + cow2)
            continue;
        temp = cow1;
        cow1 = bayles[i] + cow2;
        cow2 = temp;
    }

    //print and delete
    cout << cow1 << ' ' << cow2 << endl;
    delete[] bayles;
    return 0;
}