# Dining Philosophers Problem Simulation

This program simulates the classic Dining Philosophers Problem using multithreading in C++ with `pthread`. Philosophers alternate between thinking and eating, competing for shared resources (forks) while avoiding deadlocks and resource contention.

## Features

1. **Philosophers and Forks**:
   - Each philosopher is represented by a thread.
   - Forks are represented as mutexes (`pthread_mutex_t`).

2. **Deadlock Prevention**:
   - Implements a strategy where philosophers pick up forks in a specific order.
   - Special handling for the last philosopher to prevent circular wait conditions.

3. **Infinite Simulation**:
   - Philosophers repeatedly think, pick up forks, eat, and then release forks.

## Program Workflow

1. **Initialization**:
   - Forks are initialized as mutexes.
   - Threads are created, each representing a philosopher.

2. **Philosopher Behavior**:
   - **Thinking**: The philosopher waits (simulated with `std::this_thread::sleep_for`).
   - **Eating**: The philosopher locks the mutexes for the two adjacent forks, eats, and then unlocks the mutexes.
   - Fork-picking order is adjusted to prevent deadlock:
     - Even-numbered philosophers pick up their left fork first.
     - Odd-numbered philosophers pick up their right fork first, except the last philosopher who also picks the right fork first.

3. **Termination**:
   - Threads run indefinitely in this simulation.

## Compilation and Execution

### Compile the Program
g++ -std=c++11 -pthread -o dining_philosophers dining_philosophers.cpp

### Run the Program
./dining_philosophers

## Code Highlights

- **Deadlock Avoidance**:
  - Ensures no circular wait by altering fork-picking order for certain philosophers.
- **Thread Synchronization**:
  - Uses `pthread_mutex_t` to manage access to shared forks.
- **Simulation**:
  - Thinking, eating, and fork-picking behaviors are randomized for realism.

## Learning Objectives

- Understand the Dining Philosophers Problem and its challenges.
- Explore techniques to prevent deadlocks in multithreaded systems.
- Learn how to use `pthread` for creating and managing threads in C++.

## Key Concepts

- **Deadlock Prevention**:
  - Altering resource acquisition order prevents circular wait.
- **Resource Contention**:
  - Mutexes ensure safe access to shared forks.

