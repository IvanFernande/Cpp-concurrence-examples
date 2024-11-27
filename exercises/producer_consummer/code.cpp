#include <iostream>
#include <thread>
#include <chrono>
#include <queue>

#include <pthread.h>

using namespace std;

// Constants and Global Variables
int BUFFER_SIZE = 5;
int counter = 0;
queue<int> buffer;

pthread_mutex_t bufferMutex;
pthread_cond_t bufferFull;
pthread_cond_t bufferEmpty;

// Producer Function
void* produce(void* arg) {
    int id = *(int*) arg;
    while (true) {
        int item = ++counter;

        pthread_mutex_lock(&bufferMutex);
        while (buffer.size() == BUFFER_SIZE) {
            cout << "Buffer is full" << endl;
            pthread_cond_wait(&bufferEmpty, &bufferMutex);
        }

        buffer.push(item);
        cout << "Producer " << id << " has added " << item << " to the buffer" << endl;
        cout << "Buffer size: " << buffer.size() << endl;

        pthread_cond_signal(&bufferFull);
        pthread_mutex_unlock(&bufferMutex);

        this_thread::sleep_for(chrono::milliseconds(500));
    }

    return nullptr;
}

// Consumer Function
void* consume(void* arg) {
    int id = *(int*) arg;

    while (true) {
        pthread_mutex_lock(&bufferMutex);
        while (buffer.empty()) {
            cout << "Buffer is empty" << endl;
            pthread_cond_wait(&bufferFull, &bufferMutex);
        }

        int item = buffer.front();
        buffer.pop();
        cout << "Consumer " << id << " has consumed " << item << " from the buffer" << endl;
        cout << "Buffer size: " << buffer.size() << endl;

        pthread_cond_signal(&bufferEmpty);
        pthread_mutex_unlock(&bufferMutex);

        this_thread::sleep_for(chrono::milliseconds(600));
    }
    return nullptr;
}

int main() {
    // Initialize mutex and condition variables
    pthread_mutex_init(&bufferMutex, nullptr);
    pthread_cond_init(&bufferEmpty, nullptr);
    pthread_cond_init(&bufferFull, nullptr);

    const int NUM_PRODUCERS = 3;
    const int NUM_CONSUMERS = 2;

    pthread_t producers[NUM_PRODUCERS];
    pthread_t consumers[NUM_CONSUMERS];
    int ids[NUM_PRODUCERS + NUM_CONSUMERS];

    // Create Producer Threads
    for (int i = 0; i < NUM_PRODUCERS; i++) {
        ids[i] = i;
        if (pthread_create(&producers[i], nullptr, produce, &ids[i]) != 0) {
            cerr << "Failed to create producer thread " << i << endl;
            return 1;
        }
    }

    // Create Consumer Threads
    for (int i = 0; i < NUM_CONSUMERS; i++) {
        ids[i + NUM_PRODUCERS] = i;
        if (pthread_create(&consumers[i], nullptr, consume, &ids[i + NUM_PRODUCERS]) != 0) {
            cerr << "Failed to create consumer thread " << i << endl;
            return 1;
        }
    }

    // Wait for Producer Threads to Finish
    for (int i = 0; i < NUM_PRODUCERS; i++) {
        pthread_join(producers[i], nullptr);
    }

    // Wait for Consumer Threads to Finish
    for (int i = 0; i < NUM_CONSUMERS; i++) {
        pthread_join(consumers[i], nullptr);
    }

    // Destroy mutex and condition variables
    pthread_mutex_destroy(&bufferMutex);
    pthread_cond_destroy(&bufferEmpty);
    pthread_cond_destroy(&bufferFull);

    return 0;
}
