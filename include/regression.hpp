#pragma once
#include <vector>
#include <linalg.hpp>

class Regression {
public:
  using dataset = std::vector<Vec2>;

  explicit Regression(dataset&& ds)
      : dataset_(std::move(ds)) {}

  Vec2 fit() const noexcept;

private:
  Mat2x2 coefficients() const noexcept;
  Vec2 freeTerms() const noexcept;

  dataset dataset_;
};