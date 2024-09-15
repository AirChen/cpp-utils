#include <gtest/gtest.h>
#include <thread>
#include <vector>
#include <algorithm>
#include "threadsafe_list.h"

class ThreadsafeListTest : public ::testing::Test {
protected:
    threadsafe_list<int> list;
};

TEST_F(ThreadsafeListTest, PushFront) {
    list.push_front(1);
    list.push_front(2);
    list.push_front(3);

    std::vector<int> result;
    list.for_each([&result](int value) { result.push_back(value); });

    ASSERT_EQ(result, std::vector<int>({3, 2, 1}));
}

TEST_F(ThreadsafeListTest, ForEach) {
    list.push_front(1);
    list.push_front(2);
    list.push_front(3);

    int sum = 0;
    list.for_each([&sum](int value) { sum += value; });

    ASSERT_EQ(sum, 6);
}

TEST_F(ThreadsafeListTest, FindFirstIf) {
    list.push_front(1);
    list.push_front(2);
    list.push_front(3);

    auto result = list.find_first_if([](int value) { return value % 2 == 0; });
    ASSERT_TRUE(result);
    ASSERT_EQ(*result, 2);

    result = list.find_first_if([](int value) { return value > 5; });
    ASSERT_FALSE(result);
}

TEST_F(ThreadsafeListTest, RemoveIf) {
    list.push_front(1);
    list.push_front(2);
    list.push_front(3);
    list.push_front(4);

    list.remove_if([](int value) { return value % 2 == 0; });

    std::vector<int> result;
    list.for_each([&result](int value) { result.push_back(value); });

    ASSERT_EQ(result, std::vector<int>({3, 1}));
}

TEST_F(ThreadsafeListTest, ConcurrentPushFront) {
    const int num_threads = 10;
    const int num_iterations = 1000;

    std::vector<std::thread> threads;
    for (int i = 0; i < num_threads; ++i) {
        threads.emplace_back([this, i, num_iterations]() {
            for (int j = 0; j < num_iterations; ++j) {
                list.push_front(i * num_iterations + j);
            }
        });
    }

    for (auto& thread : threads) {
        thread.join();
    }

    int count = 0;
    list.for_each([&count](int) { ++count; });

    ASSERT_EQ(count, num_threads * num_iterations);
}

TEST_F(ThreadsafeListTest, ConcurrentForEach) {
    const int num_elements = 10000;
    for (int i = 0; i < num_elements; ++i) {
        list.push_front(i);
    }

    std::atomic<int> sum(0);
    std::vector<std::thread> threads;
    const int num_threads = 4;

    for (int i = 0; i < num_threads; ++i) {
        threads.emplace_back([this, &sum]() {
            list.for_each([&sum](int value) { sum += value; });
        });
    }

    for (auto& thread : threads) {
        thread.join();
    }

    int expected_sum = (num_elements - 1) * num_elements / 2;
    ASSERT_EQ(sum, expected_sum * num_threads);
}

TEST_F(ThreadsafeListTest, ConcurrentFindFirstIf) {
    const int num_elements = 10000;
    for (int i = 0; i < num_elements; ++i) {
        list.push_front(i);
    }

    std::vector<std::thread> threads;
    const int num_threads = 4;
    std::vector<std::shared_ptr<int>> results(num_threads);

    for (int i = 0; i < num_threads; ++i) {
        threads.emplace_back([this, i, &results]() {
            results[i] = list.find_first_if([](int value) { return value % 1000 == 0; });
        });
    }

    for (auto& thread : threads) {
        thread.join();
    }

    for (const auto& result : results) {
        ASSERT_TRUE(result);
        ASSERT_EQ(*result % 1000, 0);
    }
}

TEST_F(ThreadsafeListTest, ConcurrentRemoveIf) {
    const int num_elements = 10000;
    for (int i = 0; i < num_elements; ++i) {
        list.push_front(i);
    }

    std::vector<std::thread> threads;
    const int num_threads = 4;

    for (int i = 0; i < num_threads; ++i) {
        threads.emplace_back([this, i]() {
            list.remove_if([i](int value) { return value % num_threads == i; });
        });
    }

    for (auto& thread : threads) {
        thread.join();
    }

    int count = 0;
    list.for_each([&count](int) { ++count; });

    ASSERT_EQ(count, 0);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}