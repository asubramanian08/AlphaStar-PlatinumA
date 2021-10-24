#include <iostream>
#include <string>
using namespace std;

#define BASE 29
#define MOD 1000000007
#define NA -1

int main(void)
{
    // read
    string text, remove;
    cin >> text >> remove;

    // linked list - text based
    int *next = new int[text.size()];
    int *prev = new int[text.size()];
    for (int i = 0; i < text.size(); i++)
    {
        next[i] = i == text.size() - 1 ? NA : i + 1;
        prev[i] = i == 0 ? NA : i - 1;
    }
    auto delsstr = [next, prev](int start, int end)
    {
        if (prev[start] != NA)
            next[prev[start]] = next[end];
        if (next[end] != NA)
            prev[next[end]] = prev[start];
        prev[start] = 0; // not NA -> can't be start of linked list
    };

    // hash functions: rehash, nextHash
    auto rehash = [&](int &start, int &end)
    {
        start = end;
        for (int i = 1; i < remove.size(); i++)
            if (prev[start] != NA)
                start = prev[start];
        long long hash = 0;
        for (int iter = start, i = 0; i < remove.size(); i++, iter = next[iter])
        {
            if (iter == NA)
                return (long long)NA;
            hash = (hash + text[iter] - 'a') % MOD;
            hash = (hash * BASE) % MOD;
            end = iter; // set end (to early end)
        }
        return hash;
    };
    long long b_pow_n = 1;
    for (int i = 0; i < remove.size(); i++)
        b_pow_n = (b_pow_n * BASE) % MOD;
    auto nextHash = [b_pow_n](long long hash, int del, int add)
    {
        hash = (hash - (del * b_pow_n) % MOD + MOD) % MOD;
        hash = (((hash + add) % MOD) * BASE) % MOD;
        return hash;
    };
    long long rHash = 0;
    for (char ch : remove)
    {
        rHash = (rHash + ch - 'a') % MOD;
        rHash = (rHash * BASE) % MOD;
    }

    // run: roll hash, remove matches
    int hashStart = 0, hashEnd = remove.size() - 1;
    long long hash = rHash;
    while (hashEnd != NA)
    {
        if (hash == rHash)
            hash = rehash(hashStart, hashEnd);
        else
            hash = nextHash(hash, text[prev[hashStart]] - 'a', text[hashEnd] - 'a');
        if (hash == rHash)
        {
            delsstr(hashStart, hashEnd);
            hashEnd = next[hashEnd];
        }
        else
        {
            hashStart = next[hashStart];
            hashEnd = next[hashEnd];
        }
    }

    // print and delete
    int start;
    for (start = 0; start < text.size(); start++)
        if (prev[start] == NA)
            break;
    for (int iter = start; iter != NA; iter = next[iter])
        cout << text[iter];
    cout << endl;
    delete[] next;
    delete[] prev;
    return 0;
}