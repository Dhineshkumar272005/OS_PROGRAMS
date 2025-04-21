#include <iostream>
#include <thread>
#include <mutex>
using namespace std;

int flag[2];
int turn;
const int MAX = 1e9;
int ans = 0;

void lock_init() {
    flag[0] = flag[1] = 0;
    turn = 0;
}

void lock(int self) {
    flag[self] = 1; // Indicate that this thread wants to enter the critical section
    turn = 1 - self; // Set the turn to the other thread
    // Wait until the other thread is not interested or it's this thread's turn
    while (flag[1 - self] == 1 && turn == 1 - self);
}

void unlock(int self) {
    flag[self] = 0; // Indicate that this thread is leaving the critical section
}

void func(int self) {
    cout << "Thread Entered: " << self << endl;
    lock(self); // Acquire the lock
    for (int i = 0; i < MAX; i++) {
        ans++; // Critical section
    }
    unlock(self); // Release the lock
}

int main() {
    lock_init(); // Initialize the lock before creating threads

    thread t1(func, 0);
    thread t2(func, 1);

    t1.join(); // Wait for thread 1 to finish
    t2.join(); // Wait for thread 2 to finish

    cout << "Actual Count: " << ans << " | Expected Count: " << MAX * 2 << endl;
    return 0;
}
