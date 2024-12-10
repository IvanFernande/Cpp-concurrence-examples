# Readers-Writers Problem Simulation

This program simulates the classic **Readers-Writers Problem** using multithreading in C++ with `pthread`. It ensures proper synchronization to handle concurrent access to shared resources while prioritizing writers over readers when necessary.

## Features

1. **Threading with `pthread`**:
   - Separate threads simulate readers and writers.
   - Threads execute independently while respecting synchronization rules.

2. **Synchronization**:
   - **Mutex (`pthread_mutex_t`)**:
     Protects shared variables like the number of active readers and writer states.
   - **Condition Variables (`pthread_cond_t`)**:
     Coordinates thread actions:
     - `condReader`: Signals when readers can proceed.
     - `condWriter`: Signals when writers can proceed.

3. **Prioritization**:
   - Writers are prioritized over readers when waiting to access the shared resource.

4. **Dynamic Simulation**:
   - Readers and writers run indefinitely, simulating continuous access to the shared resource.

## Program Workflow

1. **Initialization**:
   - Mutex and condition variables are initialized.
   - Threads are created for readers and writers.

2. **Writer Behavior**:
   - Waits if there are active readers or another active writer.
   - Signals other writers first if waiting, else broadcasts to readers after finishing.

3. **Reader Behavior**:
   - Waits if there are waiting writers or an active writer.
   - Signals a waiting writer if it is the last active reader.

4. **Synchronization**:
   - Ensures mutual exclusion when modifying shared variables.
   - Prevents race conditions while prioritizing writers over readers.

## Compilation and Execution

### Compile the Program
g++ -std=c++11 -pthread -o readers_writers readers_writers.cpp

### Run the Program
./readers_writers

## Code Highlights

- **Thread Synchronization**:
  - Mutex ensures safe access to shared resources.
  - Condition variables manage thread coordination.
- **Writer Priority**:
  - Writers are given priority over readers to ensure timely updates to the shared resource.
- **Dynamic Simulation**:
  - Readers and writers continuously read and write to the shared resource, simulating real-world scenarios.

## Learning Objectives

- Understand the Readers-Writers Problem and its synchronization challenges.
- Learn to use mutexes and condition variables for thread-safe resource management.
- Explore prioritization techniques for handling concurrent access.

## Key Concepts

- **Race Condition Prevention**:
  - Mutex locks ensure no two threads modify shared variables simultaneously.
- **Thread Coordination**:
  - Readers and writers synchronize access using condition variables.
- **Writer Priority**:
  - Waiting writers are given priority over readers to ensure fairness.

