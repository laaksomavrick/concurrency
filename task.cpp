#include <iostream>
#include <future>
#include <chrono>
#include <vector>
using namespace std;

// to test: for i in {1..1000}; do ./a.out; done | sort | uniq -c

int square(int x)
{
    return x * x;
}

int main()
{
    int accum = 0;

    vector<future<int>> promises;

    for (int i = 1; i <= 20; i++)
    {
        promises.push_back(async(&square, i));
    }

    for (auto &promise : promises)
    {
        accum += promise.get();
    }

    cout << "accum = " << accum << endl;
    return 0;
}