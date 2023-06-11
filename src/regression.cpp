#include "regression.hpp"

Vec2 Regression::fit() const noexcept {
  auto[A, b] = calculateParams();
  return A.inverse() * b;
}

Regression::params Regression::calculateParams() const noexcept {
  double xSquared = 0, x = 0, y = 0, xy = 0;
  for (auto&[xi, yi] : dataset_) {
    y += yi;
    x += xi;
    xy += xi * yi;
    xSquared += xi * xi;
  }
  auto size = static_cast<double>(dataset_.size());
  return {Mat2x2(x, size, xSquared, x), Vec2(y, xy)};
}
