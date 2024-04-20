#include <iostream>
#include <thread>
#include <atomic>
#include <mutex>
#include <condition_variable>
using namespace std;
using namespace std::chrono;
using namespace std::this_thread;
mutex mtx;
condition_variable cv;
bool ready_first = true;
atomic<int> var(0);
const int total_iterations = 500;
void increment(int id, bool ready) {
    for (int i = 0; i < total_iterations; ++i) {
        unique_lock<mutex> lock(mtx);
        cv.wait(lock, [ready] {return ready_first == ready; });
        var++;
        cout << "Thread " << id << " incremented var to " << var << endl;
        ready_first = !ready_first;
        cv.notify_all();
    }
}
int main() {
    thread t1(increment, 1, true);
    thread t2(increment, 2, false);
    t1.join();
    t2.join();
    cout << "Final value of var: " << var << endl;
}
