#include "linalg.hpp"


double Mat2x2::det() const noexcept {
  return a_ * d_ - b_ * c_;
}

Mat2x2 Mat2x2::inverse() const noexcept {
  auto d = det();
  return {d_ / d, -b_ / d, -c_ / d, a_ / d};
}

Vec2 Mat2x2::operator*(const Vec2 &other) const {
  return {a_ * other.x1() + b_ * other.x2(),
          c_ * other.x1() + d_ * other.x2()};
}