load("@rules_cc//cc:defs.bzl", "cc_binary")

cc_binary(
    name = "hello-world",
    srcs = ["src/hello-world.cc"],
    deps = [
        "//common/math/wykobi:wykobi",
    ],
)

cc_binary (
    name = "test_threadsafe_queue",
    srcs = [
        "src/threadsafe_queue_test.cpp",
        "src/threadsafe_queue.h"
    ],
    deps = [
        "@gtest//:gtest",
        "@gtest//:gtest_main",
    ],
)

cc_binary (
    name = "test_threadsafe_lookup_table",
    srcs = [
        "src/threadsafe_lookup_table_test.cpp",
        "src/threadsafe_lookup_table.h"
    ],
    deps = [
        "@gtest//:gtest",
        "@gtest//:gtest_main",
    ],
)

cc_binary (
    name = "test_threadsafe_list",
    srcs = [
        "src/threadsafe_list_test.cpp",
        "src/threadsafe_list.h"
    ],
    deps = [
        "@gtest//:gtest",
        "@gtest//:gtest_main",
    ],
)

cc_binary (
    name = "test_spin_lock",
    srcs = [
        "src/spin_lock_test.cpp",
        "src/spin_lock.h"
    ],
    deps = [
        "@gtest//:gtest",
        "@gtest//:gtest_main",
    ],
)