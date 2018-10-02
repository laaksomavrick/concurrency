#include <iostream>
#include <chrono>
#include <future>
#include <thread>
#include <condition_variable>
#include <mutex>
#include <queue>
using namespace std;

// 2 threads
// one thread is producing values, the other is consuming values
// we want the consumer thread to wait until the producer is done producing values
// in order to avoid corrupted data (ie push and pop at the same time)
// int c is acting as a proof of mutual exclusion between the threads

int main()
{
    int c = 0;
    bool done = false;

    mutex m;
    condition_variable notify;

    queue<int> messages;

    thread producer([&]() {
        for (int i = 0; i < 500; ++i)
        {
            messages.push(i);
            c++;
        }

        done = true;
        // unblock the thread waiting for this condition variable
        notify.notify_one();
    });

    thread consumer([&]() {
        // lock the mutuex
        unique_lock<mutex> lock(m);
        while (!done)
        {
            // block until notify is toggled
            notify.wait(lock);
            while (!messages.empty())
            {
                messages.pop();
                c--;
            }
        }
    });

    producer.join();
    consumer.join();
    cout << "Net: " << c << endl;
    return 0;
}