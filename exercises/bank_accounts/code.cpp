#include <iostream>
#include <thread>
#include <chrono>
#include <random>
#include <vector>
#include <mutex>
#include <condition_variable>
#include <atomic>

// Semaphore implementation using C++11
class Semaphore {
public:
    Semaphore(int count = 0)
        : count_(count) {}

    void notify() {
        std::unique_lock<std::mutex> lock(mtx_);
        count_++;
        cv_.notify_one();
    }

    void wait() {
        std::unique_lock<std::mutex> lock(mtx_);
        cv_.wait(lock, [&]() { return count_ > 0; });
        count_--;
    }

private:
    std::mutex mtx_;
    std::condition_variable cv_;
    int count_;
};

const int NUM_ACCOUNTS = 5;
const int NUM_CUSTOMERS = 10;
const int MAX_TRANSFERS = 100; // Maximum transfers before stopping

std::mutex account_mutexes[NUM_ACCOUNTS];
int account_balances[NUM_ACCOUNTS];
Semaphore access_sem(NUM_ACCOUNTS - 1); // Limit concurrent accesses
std::atomic<bool> running(true); // Flag to control thread execution

// Function to perform transfers
void transfer(int customer_id, int max_transfers) {
    // Each thread has its own random number generator
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> account_id_dist(0, NUM_ACCOUNTS - 1);
    std::uniform_int_distribution<> transfer_amount_dist(500, 1000);

    int transfer_count = 0;

    while (running && transfer_count < max_transfers) {
        access_sem.wait();

        std::cout << "Customer " << customer_id << ": accessed transfers" << std::endl;

        // Select two distinct accounts
        int from_account = account_id_dist(gen);
        int to_account = account_id_dist(gen);
        while (to_account == from_account) {
            to_account = account_id_dist(gen);
        }

        // Lock accounts in ascending order to prevent deadlock
        int first_lock = std::min(from_account, to_account);
        int second_lock = std::max(from_account, to_account);

        {
            std::lock_guard<std::mutex> lock1(account_mutexes[first_lock]);
            std::lock_guard<std::mutex> lock2(account_mutexes[second_lock]);

            std::cout << "Customer " << customer_id << ": transferring from Account " << from_account
                      << " to Account " << to_account << std::endl;

            // Determine transfer amount
            int transfer_amount = transfer_amount_dist(gen);
            // Ensure sufficient balance
            while (transfer_amount > account_balances[from_account]) {
                transfer_amount = transfer_amount_dist(gen);
            }

            // Display balances before transfer
            std::cout << "----------- Balances Before Transfer -----------" << std::endl;
            std::cout << "Account " << from_account << ": " << account_balances[from_account] << std::endl;
            std::cout << "Account " << to_account << ": " << account_balances[to_account] << std::endl;
            std::cout << "-----------------------------------------------" << std::endl;

            // Perform transfer
            account_balances[from_account] -= transfer_amount;
            account_balances[to_account] += transfer_amount;

            // Display balances after transfer
            std::cout << "Transfer of " << transfer_amount << " completed successfully" << std::endl;
            std::cout << "----------- Balances After Transfer -----------" << std::endl;
            std::cout << "Account " << from_account << ": " << account_balances[from_account] << std::endl;
            std::cout << "Account " << to_account << ": " << account_balances[to_account] << std::endl;
            std::cout << "-----------------------------------------------" << std::endl;

            std::cout << "Customer " << customer_id << ": released Account " << from_account
                      << " and Account " << to_account << std::endl;
        }

        access_sem.notify();

        transfer_count++;

        // Simulate time between transfers
        std::this_thread::sleep_for(std::chrono::milliseconds(1500));
    }

    std::cout << "Customer " << customer_id << ": completed all transfers" << std::endl;
}

int main() {
    // Initialize account balances
    std::random_device rd_init;
    std::mt19937 gen_init(rd_init());
    std::uniform_int_distribution<> account_balance_dist(2000, 10000);

    for (int i = 0; i < NUM_ACCOUNTS; ++i) {
        account_balances[i] = account_balance_dist(gen_init);
    }

    // Display initial account balances
    std::cout << "=========== Initial Account Balances ============" << std::endl;
    for (int i = 0; i < NUM_ACCOUNTS; ++i) {
        std::cout << "Account " << i << ": " << account_balances[i] << std::endl;
    }
    std::cout << "===============================================" << std::endl;

    // Create customer threads
    std::vector<std::thread> customers;
    for (int i = 0; i < NUM_CUSTOMERS; ++i) {
        customers.emplace_back(transfer, i, MAX_TRANSFERS);
    }

    // Optional: Let the simulation run for a certain duration
    // std::this_thread::sleep_for(std::chrono::seconds(30));
    // running = false;

    // Wait for all customers to finish
    for (auto &customer : customers) {
        if (customer.joinable()) {
            customer.join();
        }
    }

    // Display final account balances
    std::cout << "\n=========== Final Account Balances ============" << std::endl;
    for (int i = 0; i < NUM_ACCOUNTS; ++i) {
        std::cout << "Account " << i << ": " << account_balances[i] << std::endl;
    }
    std::cout << "===============================================" << std::endl;

    return 0;
}
