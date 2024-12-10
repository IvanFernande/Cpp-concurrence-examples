# Multithreaded Bank Account Transfer Simulation

This C++ program simulates a multithreaded environment where multiple customers transfer money between bank accounts. It demonstrates concepts like threading, synchronization, semaphores, and atomic flags using modern C++11 constructs.

## Features

1. **Threading**: Multiple customer threads perform concurrent transfers between bank accounts.
2. **Synchronization**: 
   - Uses `std::mutex` to prevent race conditions during account operations.
   - Implements a semaphore (`class Semaphore`) to limit concurrent access.
3. **Deadlock Prevention**: Accounts are locked in a specific order to avoid deadlocks.
4. **Randomized Simulation**: Transfers involve randomly selected accounts and amounts using `std::random`.
5. **Atomic Flag**: Controls the simulation's runtime with an `std::atomic<bool>`.

## Program Workflow

1. **Initialization**:
   - Bank accounts are initialized with random balances.
   - Displays initial account balances.

2. **Customer Threads**:
   - Each thread selects two distinct accounts and transfers a random amount.
   - Ensures sufficient balance in the source account before performing a transfer.
   - Prints detailed logs of each transaction, including balances before and after the transfer.

3. **Concurrency Management**:
   - Limits simultaneous access to accounts using a semaphore.
   - Synchronizes access to shared resources with mutexes.

4. **Completion**:
   - Waits for all customer threads to finish.
   - Displays final account balances after all transfers.

## Compilation and Execution

### Compile the Program
g++ -std=c++11 -pthread -o bank_simulation bank_simulation.cpp

### Run the Program
./bank_simulation

## Code Highlights

- **Semaphore Implementation**:
  A custom semaphore class using `std::mutex` and `std::condition_variable`.
  
- **Deadlock-Free Transfers**:
  Accounts are locked in ascending order to avoid circular dependencies.

- **Randomization**:
  Utilizes `std::random` for realistic and varied simulations.

## Learning Objectives

- Understand multithreading and synchronization in C++.
- Learn how to implement and use semaphores.
- Gain practical experience with mutexes and atomic variables.
- Explore techniques to avoid deadlocks in concurrent systems.
