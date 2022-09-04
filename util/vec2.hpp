#pragma once

#include "std.hpp"
#include "types.hpp"

template<typename T>
concept is_number = std::floating_point<T> || std::integral<T>;

template <typename T = f32>
  requires is_number<T>
struct vec2 {
  vec2() = default;
  vec2(const T &x, const T &y) {
    this->x = x;
    this->y = y;
  }

  vec2(const vec2 &oth) : x(oth.x), y(oth.y) {}
  vec2(const vec2 &&oth) : x(std::move(oth.x)), y(std::move(oth.y)) {}

  vec2 &operator=(const vec2 &oth) {
    this->x = x;
    this->y = y;
    return *this;
  }

  vec2 &operator=(const vec2 &&oth) {
    this->x = std::move(oth.x);
    this->y = std::move(oth.y);
    return *this;
  }

  // operator overloading
  // TODO: allow operations with scalars

  vec2 &operator+=(const vec2 &oth) {
    this->x += oth.x;
    this->y += oth.y;
    return *this;
  }

  vec2 &operator-=(const vec2 &oth) {
    this->x -= oth.x;
    this->y -= oth.y;
    return *this;
  }

  vec2 &operator*=(const vec2 &oth) {
    this->x *= oth.x;
    this->y *= oth.y;
    return *this;
  }

  vec2 &operator/=(const vec2 &oth) {
    assert(oth.x != 0 && oth.y != 0);
    this->x *= 1/oth.x;
    this->y *= 1/oth.y;
    return *this;
  }

  friend vec2 abs(const vec2 &a) {
    return vec2<>(std::abs(a.x), std::abs(a.y));
  }

  friend vec2 min(const vec2 &a, const vec2 &b) {
    return vec2<>(std::min(a.x, b.x), std::min(a.y, b.y));
  }

  friend vec2 operator+(vec2 a, const vec2 &b) {
    return vec2(a.x + b.x, a.y + b.y);
  }

  friend vec2 operator-(vec2 a, const vec2 &b) {
    return vec2(a.x - b.x, a.y - b.y);
  }

  friend vec2 operator*(vec2 a, const vec2 &b) {
    return vec2(a.x * b.x, a.y * b.y);
  }

  friend vec2 operator/(vec2 a, const vec2 &b) {
    assert(b.x != 0 && b.y != 0);
    return vec2(a.x * (1/b.x), a.y * (1/b.y));
  }

  T x, y;
};

template <typename T>
struct is_vec2 {
  static const bool value = false;
};

template <typename T>
  requires is_number<T>
struct is_vec2<vec2<T>> {
  static const bool value = true;
};
