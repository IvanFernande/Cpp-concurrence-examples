# Multithreaded Car Charging Simulation

This program simulates a multithreaded environment where multiple cars compete for a limited number of charging stations. It demonstrates the use of threading, mutex locks, and condition variables for resource synchronization in C++.

## Features

1. **Threading with `pthread`**:
   - Each car is represented by a thread, simulating its behavior as it waits for and occupies a charging station.
   
2. **Synchronization**:
   - A `pthread_mutex_t` ensures safe access to shared resources.
   - A `pthread_cond_t` allows threads to wait for available charging stations.

3. **Resource Management**:
   - A limited number of charging stations (`NUM_STATIONS`) are managed to prevent overuse.
   - Cars wait if all stations are occupied and proceed when one becomes available.

4. **Infinite Simulation**:
   - The program runs indefinitely, simulating a continuous flow of cars charging at the stations.

## Program Workflow

1. **Initialization**:
   - Mutex and condition variables are initialized.
   - Each car thread is created and begins execution.

2. **Car Behavior**:
   - Cars wait for a station to become free.
   - Upon accessing a station:
     - The car "charges" for a simulated period (`std::this_thread::sleep_for`).
     - The station is released, and other waiting cars are notified.

3. **Thread Management**:
   - Threads run in an infinite loop to simulate ongoing behavior.

## Compilation and Execution

### Compile the Program
g++ -std=c++11 -pthread -o car_charging_sim car_charging_sim.cpp

### Run the Program
./car_charging_sim

## Code Highlights

- **Threading**:
  - Uses `pthread_create` and `pthread_join` to manage car threads.

- **Synchronization**:
  - Mutex (`pthread_mutex_t`) protects shared variables like `OCCUPIED_STATIONS`.
  - Condition variables (`pthread_cond_t`) manage car waiting logic.

- **Simulated Charging**:
  - Charging duration varies for each car, adding realism to the simulation.

## Learning Objectives

- Understand multithreading and synchronization using `pthread` in C++.
- Learn to use mutex locks and condition variables for resource management.
- Explore strategies for simulating real-world resource contention scenarios.

## Key Concepts

- **Mutex Locks**: Prevent race conditions when accessing shared resources.
- **Condition Variables**: Synchronize threads waiting for resources.
- **Thread Management**: Efficiently handle concurrent tasks using `pthread`.
