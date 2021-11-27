// NOT DONE
#include <iostream>
#include <vector>
#include <set>
using namespace std;

using pii = pair<long long, long long>;
#define BASE 29
#define MOD 1000000007
#define MAX_SIDE 5

void rotations(const vector<int> &piece, vector<vector<int> > &pieces)
{
    int ref = pieces.size();
    pieces.push_back(piece);
    for (int i = 0; i < 3; i++)
    {
        pieces.push_back(vector<int>(MAX_SIDE * MAX_SIDE));
        for (int j = 0; j < MAX_SIDE; j++)
            for (int k = 0; k < MAX_SIDE; k++)
                pieces.back()[j * MAX_SIDE + k] = pieces[ref + i][k * MAX_SIDE + (MAX_SIDE - 1 - j)];
    }
    for (int i = 0; i < 4; i++)
    {
        pieces.push_back(vector<int>(MAX_SIDE * MAX_SIDE));
        for (int j = 0; j < MAX_SIDE; j++)
            for (int k = 0; k < MAX_SIDE; k++)
                pieces.back()[j * MAX_SIDE + k] = pieces[ref + i][(MAX_SIDE - 1 - j) * MAX_SIDE + k];
    }
}
int leftmost(int *main, int *sub1, int *sub2)
{
    int lo = 0, hi = MAX_SIDE * MAX_SIDE - 1;
    if (main[0] - sub1[0] - sub2[0] > 0)
        return 0;
    while (hi - lo > 1)
    {
        int mid = (lo + hi) / 2;
        if (main[mid] - sub1[mid] - sub2[mid] > 0)
            hi = mid;
        else
            lo = mid;
    }
    return hi;
}

int main(void)
{
    // read
    int numPieces;
    cin >> numPieces;
    numPieces++;
    vector<vector<int> > pieces;
    for (int i = 0; i < numPieces; i++)
    {
        pii dimensions;
        cin >> dimensions.first >> dimensions.second;
        vector<int> currPiece(MAX_SIDE * MAX_SIDE);
        for (int j = 0; j < dimensions.first; j++)
            for (int k = 0; k < dimensions.second; k++)
            {
                char letter;
                cin >> letter;
                currPiece[j * MAX_SIDE + k] = (letter == '.' ? 0 : letter - 'a' + 1);
            }
        /*if (i != 0)
            rotations(currPiece, pieces);
        else*/
        pieces.push_back(currPiece);
    }

    // prefix sums (finding left most element)
    int **prefixSums = new int *[pieces.size()];
    for (int i = 0; i < pieces.size(); i++)
    {
        prefixSums[i] = new int[2 * MAX_SIDE * MAX_SIDE];
        for (int j = 0; j < MAX_SIDE * MAX_SIDE; j++)
            prefixSums[i][j] = (j != 0 ? prefixSums[i][j - 1] : 0) +
                               (pieces[i][j] ? 1 : 0);
        for (int j = MAX_SIDE * MAX_SIDE; j < 2 * MAX_SIDE * MAX_SIDE; j++)
            prefixSums[i][j] = prefixSums[i][j - 1];
    }

    // hashes
    long long *hashes = new long long[pieces.size()];
    for (int i = 0; i < pieces.size(); i++)
    {
        hashes[i] = 0;
        for (int j = 0; j < MAX_SIDE * MAX_SIDE; j++)
            hashes[i] = ((hashes[i] + pieces[i][j]) * BASE) % MOD;
    }

    // possible thirds
    long long *base_pow = new long long[MAX_SIDE * MAX_SIDE];
    base_pow[0] = 1;
    for (int i = 1; i < MAX_SIDE * MAX_SIDE; i++)
        base_pow[i] = (base_pow[i - 1] * BASE) % MOD;
    auto hashCompare = [base_pow](pii hash1, pii hash2)
    {
        if (hash1.second > hash2.second)
            swap(hash1, hash2);
        hash2.first = (hash2.first * base_pow[hash2.second - hash1.second]) % MOD;
        if (hash1.second > hash2.second)
            swap(hash1, hash2);
        return hash1.first < hash2.first;
    };
    set<pii, decltype(hashCompare)> thirds(hashCompare);
    int *zeros = new int[MAX_SIDE * MAX_SIDE];
    for (int i = 0; i < MAX_SIDE * MAX_SIDE; i++)
        zeros[i] = 0;
    for (int i = 1; i < pieces.size(); i++)
        for (int j = 1; j < pieces.size(); j++)
        {
            // if ((i - 1) / 8 == (j - 1) / 8)
            //     continue;
            long long currHash = hashes[0];
            int startI = leftmost(prefixSums[i], zeros, zeros);
            int startJ = leftmost(prefixSums[j], zeros, zeros);
            int start1 = leftmost(prefixSums[0], zeros, zeros);
            int start2 = leftmost(prefixSums[0] + start1, prefixSums[i] + startI, zeros) + start1;
            int start3 = leftmost(prefixSums[0] + start1, prefixSums[i] + startI, prefixSums[j] + startJ) + start1;
            // BASE POW IS THE PROBLEM -> start I and start J
            currHash = (currHash - (hashes[i] * base_pow[start1]) % MOD + MOD) % MOD;
            currHash = (currHash - (hashes[j] * base_pow[start2]) % MOD + MOD) % MOD;
            thirds.insert(pii(currHash, start3));
        }

    // print and delete
    int ans = 0;
    for (int i = 0; i < pieces.size(); i++)
        ans += thirds.find(pii(hashes[i], leftmost(prefixSums[i], zeros, zeros))) != thirds.end();
    cout << ans << endl;
    for (int i = 0; i < pieces.size(); i++)
        delete[] prefixSums[i];
    delete[] prefixSums;
    delete[] hashes;
    delete[] base_pow;
    delete[] zeros;
    return 0;
}