#include <vector>
#include <linalg.hpp>
#include <regression.hpp>
#include <dataloader.hpp>
#include <benchmark/benchmark.h>

static void BM_dataset_loading(benchmark::State& state) {
  for (auto _ : state) {
    state.PauseTiming();
    CSVLoader loader("../dataset.csv");
    std::vector<Vec2> dataset;
    state.ResumeTiming();

    (void)loader.load(dataset);
    benchmark::DoNotOptimize(dataset);
    benchmark::DoNotOptimize(loader);
  }
}
BENCHMARK(BM_dataset_loading);

static void BM_model_fit(benchmark::State& state) {
  CSVLoader loader("../dataset.csv");
  std::vector<Vec2> dataset;
  (void)loader.load(dataset);
  for (auto _ : state) {
    state.PauseTiming();
    std::vector<Vec2> dataset_copy = dataset;
    state.ResumeTiming();

    Regression r(std::move(dataset_copy));
    (void)r.fit();

    benchmark::DoNotOptimize(dataset_copy);
    benchmark::DoNotOptimize(r);
  }
}
BENCHMARK(BM_model_fit);

BENCHMARK_MAIN();
