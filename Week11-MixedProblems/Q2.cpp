#include <iostream>
using namespace std;

#define BIG_NUM 1000000000

int main(void)
{
    // read
    int lineSize;
    cin >> lineSize;
    char *org_line = new char[lineSize];
    for (int i = 0; i < lineSize; i++)
        cin >> org_line[i];

    // form new line
    char *new_line = new char[lineSize];
    int newPos = 0, left = 0, right = lineSize - 1;
    auto addEle = [new_line, org_line, &newPos](int &pos, int change)
    {
        new_line[newPos] = org_line[pos];
        newPos++;
        pos += change;
    };
    while (left < right)
        if (org_line[left] < org_line[right])
            addEle(left, +1);
        else if (org_line[left] > org_line[right])
            addEle(right, -1);
        else
        {
            int left2 = left + 1, right2 = right - 1;
            while (left2 < right2 && org_line[left2] == org_line[right2])
                left2++, right2--;
            org_line[left2] < org_line[right2] ? addEle(left, +1) : addEle(right, -1);
        }
    addEle(left, +1); // last element

    // print and delete
    for (int i = 0; i < lineSize; i++)
    {
        if (i % 80 == 0 && i != 0)
            cout << endl;
        cout << new_line[i];
    }
    cout << endl;
    delete[] org_line;
    delete[] new_line;
    return 0;
}