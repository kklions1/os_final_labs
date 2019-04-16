#include <iostream>
#include <mutex> 

class Semaphore { 
private: 
    int s = 1;

public: 
    void lock() { 
        // If the mutex is currently locked, wait
        while (s == 0) { 
            wait();
        }
        // Lock the critical section 
        s +=1;
    }

    void unlock() { 
        s += 1;
    }

    void wait() { 
        // TODO 
    }
};

int main() { 
    std::cout << "Hello Dickhead\n";

    Semaphore d; 
    d.lock();
    std::cout << "semaphore locked currently\n";
    d.unlock();
    std::cout << "semaphore unlocked\n";

    return 0;

}