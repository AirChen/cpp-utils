#include <gtest/gtest.h>
#include "src/threadsafe_lookup_table.h"
#include <thread>
#include <vector>
#include <string>

class ThreadsafeLookupTableTest : public ::testing::Test {
protected:
    threadsafe_lookup_table<std::string, int> table;
};

TEST_F(ThreadsafeLookupTableTest, AddAndRetrieve) {
    table.add_or_update_mapping("key1", 100);
    table.add_or_update_mapping("key2", 200);

    EXPECT_EQ(table.value_for("key1"), 100);
    EXPECT_EQ(table.value_for("key2"), 200);
    EXPECT_EQ(table.value_for("key3", -1), -1);
}

TEST_F(ThreadsafeLookupTableTest, UpdateExisting) {
    table.add_or_update_mapping("key", 100);
    EXPECT_EQ(table.value_for("key"), 100);

    table.add_or_update_mapping("key", 200);
    EXPECT_EQ(table.value_for("key"), 200);
}

TEST_F(ThreadsafeLookupTableTest, RemoveMapping) {
    table.add_or_update_mapping("key", 100);
    EXPECT_EQ(table.value_for("key"), 100);

    table.remove_mapping("key");
    EXPECT_EQ(table.value_for("key", -1), -1);
}

TEST_F(ThreadsafeLookupTableTest, GetMap) {
    table.add_or_update_mapping("key1", 100);
    table.add_or_update_mapping("key2", 200);

    auto map = table.get_map();
    EXPECT_EQ(map.size(), 2);
    EXPECT_EQ(map["key1"], 100);
    EXPECT_EQ(map["key2"], 200);
}

TEST_F(ThreadsafeLookupTableTest, ConcurrentAccess) {
    const int num_threads = 4;
    const int num_operations = 1000;

    std::vector<std::thread> threads;

    for (int i = 0; i < num_threads; ++i) {
        threads.emplace_back([this, i, num_operations]() {
            for (int j = 0; j < num_operations; ++j) {
                std::string key = "key" + std::to_string(i) + "-" + std::to_string(j);
                table.add_or_update_mapping(key, j);
                int value = table.value_for(key);
                EXPECT_EQ(value, j);
                table.remove_mapping(key);
            }
        });
    }

    for (auto& t : threads) {
        t.join();
    }

    EXPECT_EQ(table.get_map().size(), 0);
}

TEST_F(ThreadsafeLookupTableTest, ConcurrentAddOrUpdate) {
    const int num_threads = 8;
    const int num_operations = 10000;
    const std::string test_key = "concurrent_key";

    std::vector<std::thread> threads;
    std::atomic<int> max_value{0};

    for (int i = 0; i < num_threads; ++i) {
        threads.emplace_back([this, &max_value, num_operations, &test_key]() {
            for (int j = 0; j < num_operations; ++j) {
                int new_value = j + 1;
                table.add_or_update_mapping(test_key, new_value);
                
                int current_max = max_value.load(std::memory_order_relaxed);
                while (new_value > current_max &&
                       !max_value.compare_exchange_weak(current_max, new_value,
                                                        std::memory_order_relaxed,
                                                        std::memory_order_relaxed)) {
                    // 自旋等待，直到成功更新 max_value
                }
            }
        });
    }

    for (auto& t : threads) {
        t.join();
    }

    int final_value = table.value_for(test_key);
    EXPECT_EQ(final_value, max_value.load());
    EXPECT_EQ(final_value, num_operations);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}