#include <iostream>
#include <future>
#include <chrono>
#include <vector>
using namespace std;

// ideally, you should only ever run as many computationally expensive threads
// as there are cores in your computer, else you'll get (expensive) context switches

// to test: for i in {1..1000}; do ./a.out; done | sort | uniq -c

int square(int x)
{
    this_thread::sleep_for(chrono::milliseconds(100));
    return x * x;
}

int main()
{
    int accum = 0;

    vector<future<int>> promises;

    for (int i = 1; i <= 20; i++)
    {
        // async in c++ can either be deferred or truly async
        // we're forcing async here
        promises.push_back(async(launch::async, &square, i));
    }

    for (auto &promise : promises)
    {
        accum += promise.get();
    }

    cout << "accum = " << accum << endl;
    return 0;
}