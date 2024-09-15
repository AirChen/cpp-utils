#include <gtest/gtest.h>
#include "src/threadsafe_queue.h"
#include <thread>
#include <vector>

class ThreadsafeQueueTest : public ::testing::Test {
protected:
    threadsafe_queue<int> queue;
};

TEST_F(ThreadsafeQueueTest, PushAndTryPop) {
    queue.push(1);
    queue.push(2);
    queue.push(3);

    int value;
    EXPECT_TRUE(queue.try_pop(value));
    EXPECT_EQ(value, 1);
    EXPECT_TRUE(queue.try_pop(value));
    EXPECT_EQ(value, 2);
    EXPECT_TRUE(queue.try_pop(value));
    EXPECT_EQ(value, 3);
    EXPECT_FALSE(queue.try_pop(value));
}

TEST_F(ThreadsafeQueueTest, EmptyCheck) {
    EXPECT_TRUE(queue.empty());
    queue.push(1);
    EXPECT_FALSE(queue.empty());
    int value;
    queue.try_pop(value);
    EXPECT_TRUE(queue.empty());
}

TEST_F(ThreadsafeQueueTest, WaitAndPop) {
    std::thread producer([this]() {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        queue.push(42);
    });

    int value;
    queue.wait_and_pop(value);
    EXPECT_EQ(value, 42);

    producer.join();
}

TEST_F(ThreadsafeQueueTest, ConcurrentPushAndPop) {
    const int num_threads = 4;
    const int num_operations = 1000;

    std::vector<std::thread> producers;
    std::vector<std::thread> consumers;

    for (int i = 0; i < num_threads; ++i) {
        producers.emplace_back([this, num_operations]() {
            for (int j = 0; j < num_operations; ++j) {
                queue.push(j);
            }
        });

        consumers.emplace_back([this, num_operations]() {
            for (int j = 0; j < num_operations; ++j) {
                int value;
                queue.wait_and_pop(value);
                EXPECT_GE(value, 0);
                EXPECT_LT(value, num_operations);
            }
        });
    }

    for (auto& t : producers) t.join();
    for (auto& t : consumers) t.join();

    EXPECT_TRUE(queue.empty());
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}