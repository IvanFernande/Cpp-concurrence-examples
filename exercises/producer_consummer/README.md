# Producer-Consumer Problem Simulation

This program simulates the classic Producer-Consumer problem using multithreading in C++ with `pthread`. Producers generate items and add them to a shared buffer, while consumers remove items for processing. The program demonstrates how to manage shared resources in a multithreaded environment using synchronization primitives.

## Features

1. **Threading with `pthread`**:
   - Separate threads represent producers and consumers.
   - Each thread works concurrently to add or remove items from the buffer.

2. **Shared Buffer**:
   - A queue serves as the shared buffer.
   - Access to the buffer is controlled to ensure thread safety.

3. **Synchronization**:
   - Mutex (`pthread_mutex_t`) protects the buffer from race conditions.
   - Condition variables (`pthread_cond_t`) coordinate producer and consumer threads:
     - `bufferEmpty`: Signals when space becomes available in the buffer.
     - `bufferFull`: Signals when items are available for consumption.

4. **Dynamic Simulation**:
   - Producers and consumers run indefinitely, maintaining a continuous flow of items.

## Program Workflow

1. **Initialization**:
   - Mutex and condition variables are initialized.
   - Threads are created for producers and consumers.

2. **Producer Behavior**:
   - Checks if the buffer is full.
   - Waits (`pthread_cond_wait`) for space if the buffer is full.
   - Adds an item to the buffer and signals (`pthread_cond_signal`) consumers.

3. **Consumer Behavior**:
   - Checks if the buffer is empty.
   - Waits (`pthread_cond_wait`) for items if the buffer is empty.
   - Removes an item from the buffer and signals (`pthread_cond_signal`) producers.

4. **Synchronization**:
   - Mutex ensures that only one thread modifies the buffer at a time.
   - Condition variables coordinate actions between threads.

## Compilation and Execution

### Compile the Program
g++ -std=c++11 -pthread -o producer_consumer producer_consumer.cpp

### Run the Program
./producer_consumer

## Code Highlights

- **Thread Management**:
  - Producers and consumers operate independently in separate threads.
- **Synchronization**:
  - Mutex locks ensure exclusive access to shared resources.
  - Condition variables prevent unnecessary spinning and ensure efficient thread coordination.
- **Dynamic Buffer Management**:
  - Producers and consumers interact dynamically with the buffer, maintaining its state within predefined limits.

## Learning Objectives

- Understand the Producer-Consumer problem and its synchronization challenges.
- Learn to use mutexes and condition variables for thread-safe resource management.
- Explore techniques for coordinating producer and consumer threads in real-time systems.

## Key Concepts

- **Race Condition Prevention**:
  - Mutex locks ensure that no two threads access the buffer simultaneously.
- **Thread Coordination**:
  - Condition variables effectively manage the interaction between producers and consumers.

