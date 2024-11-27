#include <iostream>
#include <thread>
#include <chrono>
#include <queue>

#include <pthread.h>

using namespace std;

// Constants and Global Variables
const int NUM_PHILOSOPHERS = 5;
pthread_mutex_t forkMutex[NUM_PHILOSOPHERS]; 

// Philosopher Function
void* philosopherFunc(void* arg) {
    while(true){
        int id = *(int*) arg;

        cout << "Philosopher " << id << " is thinking." << endl;
        this_thread::sleep_for(chrono::milliseconds(500));

        if (id % 2 == 0) 
        {
            // Pick up left fork
            pthread_mutex_lock(&forkMutex[id]);
            cout << "Philosopher " << id << " picked up fork " << id << endl;

            // Pick up right fork
            pthread_mutex_lock(&forkMutex[(id + 1) % NUM_PHILOSOPHERS]);
            cout << "Philosopher " << id << " picked up fork " << (id + 1) % NUM_PHILOSOPHERS << endl;
        }
        else if (id == 4)
        {
            // Special case for philosopher 4 to prevent deadlock
            // Pick up right fork first
            pthread_mutex_lock(&forkMutex[(id + 1) % NUM_PHILOSOPHERS]);
            cout << "Philosopher " << id << " picked up fork " << (id + 1) % NUM_PHILOSOPHERS << endl;

            // Then pick up left fork
            pthread_mutex_lock(&forkMutex[id]);
            cout << "Philosopher " << id << " picked up fork " << id << endl;
        }
        else 
        {
            // Pick up right fork first for odd-numbered philosophers (except philosopher 4)
            pthread_mutex_lock(&forkMutex[(id + 1) % NUM_PHILOSOPHERS]);
            cout << "Philosopher " << id << " picked up fork " << (id + 1) % NUM_PHILOSOPHERS << endl;

            // Then pick up left fork
            pthread_mutex_lock(&forkMutex[id]);
            cout << "Philosopher " << id << " picked up fork " << id << endl;
        }

        // Eating
        cout << "Philosopher " << id << " is eating." << endl;
        this_thread::sleep_for(chrono::milliseconds(100 + rand() % 100));

        // Put down forks
        pthread_mutex_unlock(&forkMutex[id]);
        pthread_mutex_unlock(&forkMutex[(id + 1) % NUM_PHILOSOPHERS]);
        cout << "Philosopher " << id << " put down both forks." << endl;

        // Thinking again
        this_thread::sleep_for(chrono::milliseconds(100 + rand() % 100));
    }

    return nullptr;
}

int main() {
    pthread_t philosophers[NUM_PHILOSOPHERS];
    int ids[NUM_PHILOSOPHERS];

    // Initialize mutexes for each fork
    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        pthread_mutex_init(&forkMutex[i], nullptr);
    } 

    // Create philosopher threads
    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        ids[i] = i;
        if (pthread_create(&philosophers[i], nullptr, philosopherFunc, &ids[i]) != 0) {
            cerr << "Failed to create philosopher thread " << i << endl;
            return 1;
        }
    }

    // Wait for philosopher threads to finish (they run indefinitely in this case)
    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        pthread_join(philosophers[i], nullptr);
    }

    // Destroy mutexes
    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        pthread_mutex_destroy(&forkMutex[i]);
    }

    return 0;
}
