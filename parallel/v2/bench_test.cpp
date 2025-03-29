#include <benchmark/benchmark.h>
#include <vector>
#include <iostream>
using namespace std;

void demo() {
    string str = "hello world";
    str.size();
}

static void BM_demo(benchmark::State& state) {
    for (auto _ : state)
        demo();
}

BENCHMARK(BM_demo);
BENCHMARK_MAIN();