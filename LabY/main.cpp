#include <iostream>
#include <pthread.h> 

#define NUM_THREADS 6

class semaphore { 
private: 
    int value;

public: 
    void P() { 
        while (value == 0) { 
            // Wait
            continue;
        }
        value--;
    }

    void V() { 
        value++;
    }

    semaphore(int v) : value(v) {} 

    // semahore inital value is 1.
    semaphore() : value(1) {} 

};

semaphore agentSem; 

// The actual smoker semaphores. 
semaphore tobaccoSmokerSem(0), matchSmokerSem(0), paperSmokerSem(0);

// General mutex for the isIngredient boolean values (i think)?
semaphore mutex;

// protects the stuff on the table? not sure
semaphore tobacco(0), paper(0), match(0);

// is the ingredent on the table
bool isTobacco = false;
bool isPaper = false;
bool isMatch = false;


void* tobacco_smoker(void* arg) { 

}

void* match_smoker(void* arg) { 

}

void* paper_smoker(void* arg) { 

}

void* tobacco_pusher(void* arg) { 
    int counter = 12;
    while (counter > 0) { 
        tobacco.P();
        mutex.P();
        if (isPaper) { 
            isPaper = false;
            // Wake up the match smoker
            matchSmokerSem.V();
        } else if (isMatch) { 
            isMatch = false;
            // Wake up the paper smoker 
            paperSmokerSem.V();
        } else { 
            isTobacco = true;
        }
        mutex.V();
        counter--;
    }
}

void* paper_pusher(void* arg) { 
    int counter = 12;
    while (counter > 0) { 
        paper.P();
        mutex.P();
        if (isTobacco) { 
            isTobacco = false;
            matchSmokerSem.V();
        } else if (isMatch) { 
            isMatch = false;
            tobaccoSmokerSem.V();
        } else { 
            isPaper = true;
        }
        mutex.V();
        counter--;
    }
}

void* match_pusher(void* arg) { 
    int counter = 12;
    while (counter > 12) { 
        match.P();
        mutex.P();
        if (isTobacco) { 
            isTobacco = false;
            paperSmokerSem.V();
        } else if (isPaper) { 
            isPaper = false;
            tobaccoSmokerSem.V();
        } else { 
            isMatch = true;
        }
        mutex.V();
        counter--;
    }
}

// Agents produce the other two ingredients needed.
// in the case of paper_agent, it produces tobacco and matches
void* paper_agent(void* args) { 
    int counter = 6;
    while (counter > 0) { 
        agentSem.P();
        tobacco.V();
        match.V();
        counter--;
    }
}

void* match_agent(void* arg) { 
    int counter = 6;
    while (counter > 0) { 
        agentSem.P();
        tobacco.V();
        paper.V();
        counter--;
    }
}

void* tobacco_agent(void* arg) { 

    agentSem.P();
    paper.V();
    match.V();
}

int main() { 

    // Smoker Threads
    pthread_t tobaccoSmoker;
    pthread_t paperSmoker;
    pthread_t matchSmoker;

    // Agent Threads
    pthread_t matchAgent;
    pthread_t tobaccoAgent;
    pthread_t paperAgent;

    // Pusher Threads
    pthread_t paperPusher;
    pthread_t tobaccoPusher;
    pthread_t matchPusher;

    pthread_create(&tobaccoSmoker, nullptr, tobacco_smoker, nullptr);
    pthread_create(&paperSmoker, nullptr, paper_smoker, nullptr);
    pthread_create(&matchSmoker, nullptr, match_smoker, nullptr);
    
    pthread_create(&tobaccoAgent, nullptr, tobacco_agent, nullptr);
    pthread_create(&paperAgent, nullptr, paper_agent, nullptr);
    pthread_create(&matchAgent, nullptr, match_agent, nullptr);
    
    pthread_create(&tobaccoPusher, nullptr, tobacco_pusher, nullptr);
    pthread_create(&paperPusher, nullptr, paper_pusher, nullptr);
    pthread_create(&matchPusher, nullptr, match_pusher, nullptr);
    
    pthread_join(tobaccoSmoker, nullptr);
    pthread_join(paperSmoker, nullptr);
    pthread_join(matchSmoker, nullptr);
    
    pthread_join(tobaccoAgent, nullptr);
    pthread_join(paperAgent, nullptr);
    pthread_join(matchAgent, nullptr);
    
    pthread_join(tobaccoPusher, nullptr);
    pthread_join(paperPusher, nullptr);
    pthread_join(matchPusher, nullptr);
    
    return 0;

}