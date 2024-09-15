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
