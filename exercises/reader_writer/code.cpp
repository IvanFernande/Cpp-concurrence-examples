#include <iostream>
#include <pthread.h>
#include <thread>
#include <chrono>

using namespace std;

// Mutex and Condition Variables
pthread_mutex_t counterMutex;
pthread_cond_t condReader;
pthread_cond_t condWriter;

// Shared Variables
int writersWaiting = 0;
int activeReaders = 0;
bool activeWriter = false;

// Writer Function
void* writer(void* arg) {
    int id = *(int*)arg;

    while (true) {
        // Acquire mutex before modifying shared variables
        pthread_mutex_lock(&counterMutex);
        writersWaiting++;

        // Wait if there are active readers or another writer is active
        while (activeReaders > 0 || activeWriter) {
            pthread_cond_wait(&condWriter, &counterMutex);
        }

        writersWaiting--;
        activeWriter = true;
        pthread_mutex_unlock(&counterMutex);

        // Writing Section
        cout << "Writer " << id << ": is writing" << endl;
        this_thread::sleep_for(chrono::milliseconds(400));

        // Acquire mutex to update shared variables after writing
        pthread_mutex_lock(&counterMutex);
        activeWriter = false;

        // Prioritize waiting writers over readers
        if (writersWaiting > 0) {
            pthread_cond_signal(&condWriter);
        } else {
            pthread_cond_broadcast(&condReader);
        }

        pthread_mutex_unlock(&counterMutex);

        cout << "Writer " << id << ": has stopped writing" << endl;
        this_thread::sleep_for(chrono::milliseconds(500));
    }

    return nullptr;
}

// Reader Function
void* reader(void* arg) {
    int id = *(int*)arg;

    while (true) {
        // Acquire mutex before modifying shared variables
        pthread_mutex_lock(&counterMutex);

        // Wait if there are waiting writers or an active writer
        while (writersWaiting > 0 || activeWriter) {
            pthread_cond_wait(&condReader, &counterMutex);
        }

        activeReaders++;
        pthread_mutex_unlock(&counterMutex);

        // Reading Section
        cout << "Reader " << id << ": is reading" << endl;
        this_thread::sleep_for(chrono::milliseconds(400));

        // Acquire mutex to update shared variables after reading
        pthread_mutex_lock(&counterMutex);
        activeReaders--;

        // If no more active readers, signal a waiting writer
        if (activeReaders == 0) {
            pthread_cond_signal(&condWriter);
        }

        pthread_mutex_unlock(&counterMutex);

        cout << "Reader " << id << ": has stopped reading" << endl;
        this_thread::sleep_for(chrono::milliseconds(500));
    }

    return nullptr;
}

int main() {
    // Initialize mutex and condition variables
    pthread_mutex_init(&counterMutex, nullptr);
    pthread_cond_init(&condReader, nullptr);
    pthread_cond_init(&condWriter, nullptr);

    const int NUM_READERS = 3;
    const int NUM_WRITERS = 2;

    pthread_t readers[NUM_READERS];
    pthread_t writersThreads[NUM_WRITERS];
    int readerIds[NUM_READERS] = {1, 2, 3};
    int writerIds[NUM_WRITERS] = {1, 2};

    // Create Reader Threads
    for (int i = 0; i < NUM_READERS; i++) {
        if (pthread_create(&readers[i], nullptr, reader, &readerIds[i]) != 0) {
            cerr << "Failed to create reader thread " << i + 1 << endl;
            return 1;
        }
    }

    // Create Writer Threads
    for (int i = 0; i < NUM_WRITERS; i++) {
        if (pthread_create(&writersThreads[i], nullptr, writer, &writerIds[i]) != 0) {
            cerr << "Failed to create writer thread " << i + 1 << endl;
            return 1;
        }
    }

    // Wait for Reader Threads to Finish
    for (int i = 0; i < NUM_READERS; i++) {
        pthread_join(readers[i], nullptr);
    }

    // Wait for Writer Threads to Finish
    for (int i = 0; i < NUM_WRITERS; i++) {
        pthread_join(writersThreads[i], nullptr);
    }

    // Destroy mutex and condition variables
    pthread_mutex_destroy(&counterMutex);
    pthread_cond_destroy(&condReader);
    pthread_cond_destroy(&condWriter);

    return 0;
}
