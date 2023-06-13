#include <dataloader.hpp>
#include <iostream>
#include <benchmark/benchmark.h>
#include <immintrin.h>

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

using vofv = std::vector<__m256>;
static std::pair<vofv, vofv> dataset_3() {
  auto v = dataset();
  vofv x(v.size() / 4), y(v.size() / 4);
  for (size_t i = 0; i < x.size(); ++i) {
    x[i] = _mm256_set_pd(v[4*i].x1(), v[4*i+1].x1(), v[4*i+2].x1(), v[4*i+3].x1());
    x[i] = _mm256_set_pd(v[4*i].x2(), v[4*i+1].x2(), v[4*i+2].x2(), v[4*i+3].x2());
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
BENCHMARK(linear_traverse);

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
BENCHMARK(double_linear_traverse);

static void avx_traverse(benchmark::State& state) {
  auto&&[ax, ay] = dataset_2();
  for (auto _ : state) {
    __m256  x = _mm256_set1_pd(0),
            y = _mm256_set1_pd(0),
           xy = _mm256_set1_pd(0),
           xS = _mm256_set1_pd(0);
    for (size_t i = 0; i < ax.size(); i += 4) {
      __m256 xi = _mm256_loadu_pd(&ax[i]);
      x = _mm256_add_pd(x, xi);

      __m256 yi = _mm256_loadu_pd(&ay[i]);
      y = _mm256_add_pd(y, yi);

      yi = _mm256_mul_pd(yi, xi);
      xy = _mm256_add_pd(yi, xy);

      xi = _mm256_mul_pd(xi, xi);
      xS = _mm256_add_pd(xS, xi);
    }
    alignas(32) double bx[4], by[4], bxS[4], bxy[4];
    _mm256_store_pd(bx, x);
    _mm256_store_pd(by, y);
    _mm256_store_pd(bxS, xS);
    _mm256_store_pd(bxy, xy);

    benchmark::DoNotOptimize(bx);
    benchmark::DoNotOptimize(by);
    benchmark::DoNotOptimize(bxS);
    benchmark::DoNotOptimize(bxy);
  }
}
BENCHMARK(avx_traverse);

static void aligned_avx_traverse(benchmark::State& state) {
  auto [ax, ay] = dataset_3();
  for (auto _ : state) {
    __m256  x = _mm256_set1_pd(0),
           y = _mm256_set1_pd(0),
           xy = _mm256_set1_pd(0),
           xS = _mm256_set1_pd(0);
    for (size_t i = 0; i < ax.size(); i += 4) {
      __m256 xi = ax[i];
      x = _mm256_add_pd(x, xi);

      __m256 yi = ay[i];
      y = _mm256_add_pd(y, yi);

      yi = _mm256_mul_pd(yi, xi);
      xy = _mm256_add_pd(yi, xy);

      x = _mm256_mul_pd(x, x);
      xS = _mm256_add_pd(xS, x);
    }
    alignas(32) double bx[4], by[4], bxS[4], bxy[4];
    _mm256_store_pd(bx, x);
    _mm256_store_pd(by, y);
    _mm256_store_pd(bxS, xS);
    _mm256_store_pd(bxy, xy);

    benchmark::DoNotOptimize(bx);
    benchmark::DoNotOptimize(by);
    benchmark::DoNotOptimize(bxS);
    benchmark::DoNotOptimize(bxy);
  }
}
BENCHMARK(aligned_avx_traverse);

BENCHMARK_MAIN();
