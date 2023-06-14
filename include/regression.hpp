#pragma once
#include <utility>
#include <vector>
#include <linalg.hpp>

class Regression {
public:
  using dataset = std::vector<Vec2>;

  explicit Regression(dataset&& ds)
      : dataset_(std::move(ds)) {}

  Vec2 fit() const noexcept;

private:
  struct params {
    Mat2x2 coefficients;
    Vec2 freeTerms;
  };
  params calculateParams() const noexcept;
  
  dataset dataset_;
};
