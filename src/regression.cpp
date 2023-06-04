#include "regression.hpp"

Vec2 Regression::fit() const noexcept {
  auto A = coefficients();
  auto b = freeTerms();
  return A.inverse() * b;
}

Mat2x2 Regression::coefficients() const noexcept {
  double xSquared = 0, x = 0;
  for (auto&[xi, _] : dataset_)
    x += xi, xSquared += xi * xi;
  auto size = static_cast<double>(dataset_.size());
  return {x, size, xSquared, x};
}

Vec2 Regression::freeTerms() const noexcept {
  double y = 0, xy = 0;
  for (auto&[xi, yi] : dataset_)
    y += yi, xy += yi * xi;
  return {y, xy};
}
