// All but one case - need O(nlgn)
#include <iostream>
#include <algorithm>
#include <vector>
using namespace std;

struct test
{
    long long num, den;
    long long modeDiff(const test &t) const { return num * t.den - t.num * den; }
    bool operator<(const test &t) const { return modeDiff(t) < 0; }
    test operator+(const test &t) const { return test(num + t.num, den + t.den); }
    test operator-(const test &t) const { return test(num - t.num, den - t.den); }
    test(long long n, long long d) : num(n), den(d) {}
    test() = default;
};

int main(void)
{
    int numTests;
    cin >> numTests;
    test *tests = new test[numTests];
    for (int i = 0; i < numTests; i++)
        cin >> tests[i].num >> tests[i].den;
    sort(tests, tests + numTests);
    test teacher(0, 0);
    for (int i = 0; i < numTests; i++)
        teacher = teacher + tests[i];
    vector<int> answers;
    for (int i = 0; i < numTests - 1; i++)
    {
        teacher = teacher - tests[i];
        int bestRem = 0, worstAdd = i + 1;
        for (int j = 1; j <= i; j++)
            if (tests[j].modeDiff(teacher) >
                tests[bestRem].modeDiff(teacher))
                bestRem = j;
        for (int j = i + 2; j < numTests; j++)
            if (tests[j].modeDiff(teacher) <
                tests[worstAdd].modeDiff(teacher))
                worstAdd = j;
        if (teacher < teacher + tests[bestRem] - tests[worstAdd])
            answers.push_back(i + 1);
    }
    cout << answers.size() << endl;
    for (int val : answers)
        cout << val << endl;
    delete[] tests;
    return 0;
}