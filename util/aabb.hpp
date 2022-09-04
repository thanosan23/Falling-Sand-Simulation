#pragma once

#include "raylib.h"

#include "types.hpp"
#include "vec2.hpp"

struct AABB {
  AABB() = default;
  AABB(const vec2<> &pos, const u32 &width, const u32 &height) {
    this->pos = std::move(pos);
    this->width = width;
    this->height = height;
  }

  bool side(const AABB &other) {
    return this->pos.x + this->width >= other.pos.x &&
      this->pos.x <= other.pos.x + other.width &&
      this->pos.y > other.pos.y && this->pos.y < other.pos.y + other.height;
  }

  bool top(const AABB &other) {
    return this->pos.y + this->height >= other.pos.y &&
      this->pos.y + this->height <= other.pos.y + other.height &&
      this->pos.x > other.pos.x && this->pos.x < other.pos.x + other.width;
  }

  bool collidesX(const AABB &other) {
    return this->pos.x < other.pos.x + other.width &&
      this->pos.x + this->width > other.pos.x;
  }

  bool collidesY(const AABB &other) {
    return this->pos.y < other.pos.y + other.height &&
      this->pos.y + this->height > other.pos.y;
  }

  bool collides(const AABB &other) {
    return collidesX(other) && collidesY(other);
  }

  void update(const vec2<> &pos) {
    this->pos = std::move(pos);
  }

  vec2<> pos;
  u32 width, height;
};

