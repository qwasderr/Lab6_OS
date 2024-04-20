#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>
using namespace std;
using namespace std::chrono;
mutex mtx;
int var = 0;
atomic<int> atomic_var(0);
void increment_mutex(int iterations) {
    for (int i = 0; i < iterations; ++i) {
        mtx.lock();
        var++;
        mtx.unlock();
    }
}
void increment(int iterations) {
    for (int i = 0; i < iterations; ++i) var++;
}
void increment_atomic(int iterations) {
    for (int i = 0; i < iterations; ++i) {
        atomic_var++;
    }
}
int main() {
    int iterations = 1e9;
    auto start = high_resolution_clock::now();
    thread t1(increment_mutex, iterations);
    thread t2(increment_mutex, iterations);
    t1.join();
    t2.join();
    auto finish = high_resolution_clock::now();
    auto duration = duration_cast<nanoseconds>(finish - start);
    cout << "Time (with mutex): " << duration.count() << endl;
    var = 0;
    auto start2 = high_resolution_clock::now();
    thread t3(increment_atomic, iterations);
    thread t4(increment_atomic, iterations);
    t3.join();
    t4.join();
    auto finish2 = high_resolution_clock::now();
    auto duration2 = duration_cast<nanoseconds>(finish2 - start2);
    cout << "Time (with atomic): " << duration2.count() << endl;
    var = 0;
    auto start3 = high_resolution_clock::now();
    thread t5(increment, iterations);
    thread t6(increment, iterations);
    t5.join();
    t6.join();
    auto finish3 = high_resolution_clock::now();
    auto duration3 = duration_cast<nanoseconds>(finish3 - start3);
    cout << "Time (without mutex): " << duration3.count() << endl;
}
