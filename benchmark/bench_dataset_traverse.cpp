#include <cstring>
#include <iostream>
#include <immintrin.h>
#include <dataloader.hpp>
#include <benchmark/benchmark.h>

static std::vector<Vec2> dataset() {
  CSVLoader loader("../dataset.csv");
  std::vector<Vec2> v;
  (void)loader.load(v);
  return v;
}

using vofd = std::vector<double>;
static std::pair<vofd, vofd> dataset_2() {
  auto v = dataset();
  vofd x(v.size()), y(v.size());
  for (size_t i = 0; i < v.size(); ++i)
    x[i] = v[i].x1(), y[i] = v[i].x2();
  return {x, y};
}

using vofv = std::vector<__m256d>;
static std::pair<vofv, vofv> dataset_3() {
  auto v = dataset();
  const size_t nvec = (v.size() + 3) & ~0b11;

  vofv x(nvec / 4), y(nvec / 4);
  for (size_t i = 0; i < v.size() / 4; ++i) {
    x[i] = _mm256_set_pd(v[4*i].x1(), v[4*i+1].x1(), v[4*i+2].x1(), v[4*i+3].x1());
    y[i] = _mm256_set_pd(v[4*i].x2(), v[4*i+1].x2(), v[4*i+2].x2(), v[4*i+3].x2());
  }

  // process tail
  if (nvec != v.size()) {
    alignas(32) double xs[4] = {0}, ys[4] = {0};
    for (size_t i = nvec - 4; i < v.size(); ++i) {
      const size_t j = i + 4 - nvec;
      xs[j] = v[i].x1(), ys[j] = v[i].x2();
    }
    x[nvec/4-1] = _mm256_load_pd(xs);
    y[nvec/4-1] = _mm256_load_pd(ys);
  }
  return {x, y};
}

static void linear_traverse(benchmark::State& state) {
  auto dataset_ = dataset();
  for (auto _ : state) {
    double xSquared = 0, x = 0, y = 0, xy = 0;
    for (auto&[xi, yi] : dataset_) {
      y += yi;
      x += xi;
      xy += xi * yi;
      xSquared += xi * xi;
    }
    benchmark::DoNotOptimize(xSquared);
    benchmark::DoNotOptimize(x);
    benchmark::DoNotOptimize(y);
    benchmark::DoNotOptimize(xy);
  }
}

static void double_linear_traverse(benchmark::State& state) {
  auto [ax, ay] = dataset_2();
  for (auto _ : state) {
    double xSquared = 0, x = 0, y = 0, xy = 0;
    for (size_t i = 0; i < ax.size(); ++i) {
      y += ay[i];
      x += ax[i];
      xy += ax[i] * ay[i];
      xSquared += ax[i] * ax[i];
    }
    benchmark::DoNotOptimize(xSquared);
    benchmark::DoNotOptimize(x);
    benchmark::DoNotOptimize(y);
    benchmark::DoNotOptimize(xy);
  }
}

static void avx_traverse(benchmark::State& state) {
  auto&&[ax, ay] = dataset_2();
  for (auto _ : state) {
    __m256d  x = _mm256_set1_pd(0),
            y = _mm256_set1_pd(0),
           xy = _mm256_set1_pd(0),
           xS = _mm256_set1_pd(0);
    size_t i;
    for (i = 0; i < ax.size(); i += 4) {
      const __m256d xi = _mm256_loadu_pd(&ax[i]);
      x = _mm256_add_pd(x, xi);

      const __m256d yi = _mm256_loadu_pd(&ay[i]);
      y = _mm256_add_pd(y, yi);

      xy = _mm256_fmadd_pd(yi, xi, xy);
      xS = _mm256_fmadd_pd(xi, xi, xS);
    }
    alignas(32) double r[4];
    __m256d sum = _mm256_hadd_pd(x, y);
    __m128d sum_high = _mm256_extractf128_pd(sum, 1);
    __m128d res = _mm_add_pd(sum_high, _mm256_castpd256_pd128(sum));
    _mm_store_pd(r, res);

    sum = _mm256_hadd_pd(xS, xy);
    sum_high = _mm256_extractf128_pd(sum, 1);
    res = _mm_add_pd(sum_high, _mm256_castpd256_pd128(sum));
    _mm_store_pd(r + 2, res);

    for (; i < ax.size(); ++i) {
      r[0] += ax[i];
      r[1] += ay[i];
      r[2] += ax[i] * ax[i];
      r[3] += ay[i] * ax[i];
    }

    benchmark::DoNotOptimize(r);
  }
}

static void aligned_avx_traverse(benchmark::State& state) {
  auto&&[ax, ay] = dataset_3();
  for (auto _ : state) {
    __m256d x = _mm256_set1_pd(0),
           y = _mm256_set1_pd(0),
           xy = _mm256_set1_pd(0),
           xS = _mm256_set1_pd(0);

    for (size_t i = 0; i < ax.size(); ++i) {
      const __m256d xi = ax[i];
      x = _mm256_add_pd(x, xi);

      const __m256d yi = ay[i];
      y = _mm256_add_pd(y, yi);

      xy = _mm256_fmadd_pd(yi, xi, xy);
      xS = _mm256_fmadd_pd(xi, xi, xS);
    }

    alignas(32) double r[4];
    __m256d sum = _mm256_hadd_pd(x, y);
    __m128d sum_high = _mm256_extractf128_pd(sum, 1);
    __m128d res = _mm_add_pd(sum_high, _mm256_castpd256_pd128(sum));
    _mm_store_pd(r, res);

    sum = _mm256_hadd_pd(xS, xy);
    sum_high = _mm256_extractf128_pd(sum, 1);
    res = _mm_add_pd(sum_high, _mm256_castpd256_pd128(sum));
    _mm_store_pd(r + 2, res);

    benchmark::DoNotOptimize(r);
  }
}


#define BENCH(func)                                             \
  BENCHMARK_PRIVATE_DECLARE(func) =                             \
      (::benchmark::internal::RegisterBenchmarkInternal(        \
          new ::benchmark::internal::FunctionBenchmark(         \
              #func,                                            \
              [](::benchmark::State& st) {                      \
                  FlushCacheNonpausing();                       \
                  func(st);                                     \
              }                                                 \
          )                                                     \
       ))                                                       \
    ->Iterations(10)

inline void FlushCacheNonpausing(int level = 3) {
  const auto& cpu = benchmark::CPUInfo::Get();

  std::vector<unsigned> resetter;
  for (const auto& cache : cpu.caches) {
    if (cache.level != level) {
      continue;
    }

    resetter.resize(cache.size / sizeof(unsigned),
                    static_cast<unsigned>(level));
  }

  if (resetter.empty()) {
    throw std::logic_error("Failed to reset cache");
  }

  unsigned trash = static_cast<unsigned>(level * level);
  for (unsigned& v : resetter) {
    trash += v;
    v += trash;
  }

  benchmark::DoNotOptimize(trash);
  benchmark::DoNotOptimize(resetter);
}

BENCHMARK(linear_traverse);
BENCHMARK(double_linear_traverse);
BENCHMARK(avx_traverse);
BENCHMARK(aligned_avx_traverse);

BENCHMARK_MAIN();
