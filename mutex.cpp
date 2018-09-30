#include <iostream>
#include <vector>
#include <thread>
using namespace std;

// to test: for i in {1..1000}; do ./a.out; done | sort | uniq -c

// no guarantee that the threads don't interleave
// will be errors via race condition
void square_with_race_condition(int &accum, int x)
{
    accum += x * x;
}

// computations can occur concurrently, block on summing the accum
void square_with_mutex(int &accum, mutex &accum_mutex, int x)
{
    int temp = x * x;
    accum_mutex.lock();
    accum += temp;
    accum_mutex.unlock();
}

int main()
{
    mutex accum_mutex;
    int accum = 0;

    // an empty vector of threads
    vector<thread> threads;

    // for 1 .. 20, calculate the sum to accum concurrently
    for (int i = 1; i <= 20; i++)
    {
        threads.push_back(thread(&square_with_mutex, ref(accum), ref(accum_mutex), i));
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