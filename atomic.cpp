#include <iostream>
#include <vector>
#include <thread>
#include <atomic>
using namespace std;

// to test: for i in {1..1000}; do ./a.out; done | sort | uniq -c

void square(atomic<int> &accum, int x)
{
    accum += x * x;
}

int main()
{
    // atomic container guarantees atomicity w/r/t computations against this value
    atomic<int> accum(0);

    // an empty vector of threads
    vector<thread> threads;

    // for 1 .. 20, calculate the sum to accum concurrently
    for (int i = 1; i <= 20; i++)
    {
        threads.push_back(thread(&square, ref(accum), i));
    }

    // call join() on each thread
    // this is a blocking operation which waits for the thread to finish
    // necessary before printing accum, otherwise a computation may still be happening before
    // printing and exiting
    for (auto &thread : threads)
    {
        thread.join();
    }

    cout << "accum = " << accum << endl;
    return 0;
}