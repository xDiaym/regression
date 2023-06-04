#pragma once
#include <utility>
#include <cstddef>

class Vec2 {
public:
  Vec2(double x1, double x2)
      : x1_(x1), x2_(x2) {}

  [[nodiscard]] auto x1() const noexcept { return x1_; }
  [[nodiscard]] auto x2() const noexcept { return x2_; }

  template<size_t N>
  [[nodiscard]]
  double get() const {
    if constexpr (N == 0) return x1_;
    return x2_;
  }

private:
  double x1_, x2_;
};

namespace std {
  template <>
  struct tuple_size<Vec2> : std::integral_constant<size_t, 2> {};

  template <size_t N>
  struct tuple_element<N, Vec2> {
    using type = decltype(declval<Vec2>().get<N>());
  };
}

class Mat2x2 {
public:
  Mat2x2(double a, double b, double c, double d)
    : a_(a), b_(b), c_(c), d_(d) {}

  double det() const noexcept;
  Mat2x2 inverse() const noexcept;
  Vec2 operator*(const Vec2& other) const;

  [[nodiscard]] auto a() const noexcept { return a_; }
  [[nodiscard]] auto b() const noexcept { return b_; }
  [[nodiscard]] auto c() const noexcept { return c_; }
  [[nodiscard]] auto d() const noexcept { return d_; }

private:
  double a_, b_, c_, d_;
};
