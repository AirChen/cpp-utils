#include <gtest/gtest.h>
#include <future>
#include <vector>
#include <chrono>
#include <atomic>
#include "./src/spin_lock.h"

class SpinLockTest : public ::testing::Test {
protected:
    spinlock_mutex lock;
    const int NUM_THREADS = 10;
    const int ITERATIONS_PER_THREAD = 100000;
};

TEST_F(SpinLockTest, ConcurrentIncrement) {
    std::atomic<int> shared_counter(0);
    std::vector<std::future<void>> futures;

    auto start_time = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < NUM_THREADS; ++i) {
        futures.push_back(std::async(std::launch::async, [this, &shared_counter]() {
            for (int j = 0; j < ITERATIONS_PER_THREAD; ++j) {
                lock.lock();
                shared_counter++;
                lock.unlock();
            }
        }));
    }

    // Wait for all threads to complete
    for (auto& future : futures) {
        future.get();
    }

    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);

    EXPECT_EQ(shared_counter, NUM_THREADS * ITERATIONS_PER_THREAD);
    std::cout << "Time taken: " << duration.count() << " ms" << std::endl;
}

TEST_F(SpinLockTest, ConcurrentReadWrite) {
    std::vector<int> shared_vector;
    std::vector<std::future<void>> futures;

    for (int i = 0; i < NUM_THREADS; ++i) {
        futures.push_back(std::async(std::launch::async, [this, &shared_vector, i]() {
            for (int j = 0; j < ITERATIONS_PER_THREAD / 10; ++j) {
                if (j % 2 == 0) {
                    // Write operation
                    lock.lock();
                    shared_vector.push_back(i * 1000 + j);
                    lock.unlock();
                } else {
                    // Read operation
                    lock.lock();
                    if (!shared_vector.empty()) {
                        volatile int value = shared_vector.back();
                        (void)value;  // Prevent optimization
                    }
                    lock.unlock();
                }
            }
        }));
    }

    // Wait for all threads to complete
    for (auto& future : futures) {
        future.get();
    }

    EXPECT_EQ(shared_vector.size(), NUM_THREADS * ITERATIONS_PER_THREAD / 20);
}

TEST_F(SpinLockTest, Fairness) {
    std::atomic<int> shared_counter(0);
    std::vector<std::future<int>> futures;

    for (int i = 0; i < NUM_THREADS; ++i) {
        futures.push_back(std::async(std::launch::async, [this, &shared_counter]() {
            int local_count = 0;
            for (int j = 0; j < ITERATIONS_PER_THREAD / 10; ++j) {
                lock.lock();
                shared_counter++;
                local_count++;
                lock.unlock();
            }
            return local_count;
        }));
    }

    std::vector<int> thread_counts;
    for (auto& future : futures) {
        thread_counts.push_back(future.get());
    }

    int min_count = *std::min_element(thread_counts.begin(), thread_counts.end());
    int max_count = *std::max_element(thread_counts.begin(), thread_counts.end());

    EXPECT_EQ(shared_counter, NUM_THREADS * (ITERATIONS_PER_THREAD / 10));
    std::cout << "Min count: " << min_count << ", Max count: " << max_count << std::endl;
    EXPECT_LT(max_count - min_count, ITERATIONS_PER_THREAD / 100);  // Allowing 1% difference
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}