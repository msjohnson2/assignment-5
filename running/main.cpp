#include"tmsarray.hpp"
#include<iostream>

using std::cout;
using std::endl;


int main()
{
    TMSArray<double> arr(1);

    cout << arr.size() << endl;

    arr.resize(20);

    arr.insert(arr.begin() + 3, 8);
    arr.push_back(3);

    cout << arr.size() << endl;

    for(auto x: arr)
    {
        cout << x;
    }
    cout << endl;

    arr.erase(arr.begin()+3);
    arr.pop_back();

    cout << arr.size() << endl;

    for(auto x: arr)
    {
        cout << x;
    }

    cout << endl << arr[22] << endl;



    return 0;
}