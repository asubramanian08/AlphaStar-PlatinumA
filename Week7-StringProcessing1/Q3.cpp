#include <iostream>
using namespace std;

int main(void)
{
    // read
    int numStalls, lines;
    cin >> numStalls >> lines;
    int *stalls = new int[numStalls];
    for (int i = 0; i < numStalls; i++)
        stalls[i] = 0;
    for (int i = 0; i < lines; i++)
    {
        long long cows, iters, A, B;
        cin >> cows >> iters >> A >> B;
        for (int j = 1; j <= iters; j++)
            stalls[(A * j + B) % numStalls] += cows;
    }

    // Assign cows
    int toPlace = 0;
    for (int i = 0; i < numStalls * 2; i++)
        if (toPlace != 0 || stalls[i % numStalls] != 0)
        {
            toPlace += stalls[i % numStalls] - 1;
            stalls[i % numStalls] = 1;
        }

    // Earliest stall
    int earliest;
    for (earliest = 0; earliest < numStalls; earliest++)
        if (stalls[earliest] == 0)
            break;

    // print and delete
    cout << earliest << endl;
    delete[] stalls;
    return 0;
}