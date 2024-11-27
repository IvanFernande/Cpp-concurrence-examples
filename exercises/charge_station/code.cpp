#include <iostream>
#include <thread>
#include <chrono>

#include <pthread.h>

using namespace std;

// Constants and Global Variables
const int NUM_CARS = 10;
const int NUM_STATIONS = 3;
pthread_mutex_t resourceMutex;
pthread_cond_t stationFree;

int OCCUPIED_STATIONS = 0;

// Function to simulate car charging
void* charge(void* arg) 
{
    while(true) 
    {
        int carId = *(int*) arg;
        cout << "Car " << carId << ": waiting for a station." << endl;

        // Lock the mutex before accessing shared resources
        pthread_mutex_lock(&resourceMutex);
        
        // Wait while all stations are occupied
        while (OCCUPIED_STATIONS >= NUM_STATIONS)
        {
            pthread_cond_wait(&stationFree, &resourceMutex);
        }

        // A station is now free for this car
        cout << "Car " << carId << ": entered a station." << endl;
        OCCUPIED_STATIONS++;

        // Unlock the mutex after modifying shared resources
        pthread_mutex_unlock(&resourceMutex);

        // Simulate charging time
        cout << "Car " << carId << ": charging at a station." << endl;
        this_thread::sleep_for(chrono::milliseconds(1000 + (200 * carId)));
        cout << "Car " << carId << ": finished charging." << endl;

        // Lock the mutex to update shared resources
        pthread_mutex_lock(&resourceMutex);

        // Release the station
        OCCUPIED_STATIONS--;
        pthread_cond_signal(&stationFree);
        
        // Unlock the mutex after updating shared resources
        pthread_mutex_unlock(&resourceMutex);
        
        cout << "Car " << carId << ": leaving a station." << endl;
        this_thread::sleep_for(chrono::milliseconds(1000));
    
    }
    return nullptr;

}

int main() {
    // Initialize mutex and condition variable
    pthread_mutex_init(&resourceMutex, nullptr);
    pthread_cond_init(&stationFree, nullptr);

    // Arrays to hold thread IDs and car IDs
    int carIds[NUM_CARS];
    pthread_t cars[NUM_CARS];

    // Create threads for each car
    for (int i = 0; i < NUM_CARS; i++)
    {
        carIds[i] = i;
        if (pthread_create(&cars[i], nullptr, charge, &carIds[i]) != 0) {
            cerr << "Failed to create thread for Car " << i << endl;
            return 1;
        }
    }

    // Wait for all car threads to finish (they run indefinitely in this case)
    for (int i = 0; i < NUM_CARS; i++)
    {
        pthread_join(cars[i], nullptr);
    }

    // Destroy mutex and condition variable
    pthread_mutex_destroy(&resourceMutex);
    pthread_cond_destroy(&stationFree);

    return 0;
}
