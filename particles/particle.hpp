#pragma once

#include "raylib.h"
#include "../util/std.hpp"


enum ParticleType {
  EMPTY,
  SAND,
  WALL
};

struct Particle {
  Particle(const vec2<u32> &pos, const ParticleType type) {
    this->pos = std::move(pos);
    this->changeType(type);
  }

  void changeType(const ParticleType &type) {
    if(this->type != type) {
      this->type = type;
      switch(type) {
        case(EMPTY):
          this->color = BLANK;
          break;
        case(SAND):
          this->color = BROWN;
          break;
        case(WALL):
          this->color = BLACK;
          break;
      }
    }
  }
  Color color;
  vec2<u32> pos;
  ParticleType type;
};

struct ParticleManager {
  ParticleManager(const vec2<u32> &windowSize, const vec2<u32> &gridSize) {
    this->windowSize = std::move(windowSize);
    this->gridSize = std::move(gridSize);
    assert(this->windowSize.x % this->gridSize.x == 0);
    assert(this->windowSize.y % this->gridSize.y == 0);
    this->particleSize = vec2<u32>(this->windowSize.x / this->gridSize.x,
        this->windowSize.y / this->gridSize.y);

    // generate particles
    for(u32 y = 0; y < this->gridSize.y; ++y) {
      for(u32 x = 0; x < this->gridSize.x; ++x) {
        particles.emplace_back(std::move(Particle(vec2<u32>(x, y), EMPTY)));
      }
    }
  }

  Particle *getParticle(const vec2<u32> &pos) {
    return &particles[pos.y * this->gridSize.x + pos.x];
  }

  void drawParticle(const vec2<u32> &pos) {
      Particle *p = this->getParticle(pos);
      DrawRectangle(p->pos.x * this->particleSize.x,
          p->pos.y * this->particleSize.y, this->particleSize.x,
          this->particleSize.y, p->color);
  }

  void updateParticles() {

    BeginDrawing();
    ClearBackground(RAYWHITE);
    for(u32 y = 0; y < this->gridSize.y; ++y) {
      for(u32 x = 0; x < this->gridSize.x; ++x) {
        this->drawParticle(vec2<u32>(x, y));
        Particle *p = this->getParticle(vec2<u32>(x, y));
        switch(p->type) {
          case(EMPTY):
            break;
          case(SAND):
            if(getParticle(p->pos + vec2<u32>(0, 1))->type == EMPTY) {
              this->move(p->pos, p->pos + vec2<u32>(0, 1));
            } else if(getParticle(p->pos + vec2<u32>(1, 1))->type == EMPTY) {
              this->move(p->pos, p->pos + vec2<u32>(1, 1));
            } else if(getParticle(p->pos + vec2<u32>(0, 1) - vec2<u32>(1, 0))->type == EMPTY) {
              this->move(p->pos, p->pos + vec2<u32>(0, 1) - vec2<u32>(1, 0));
            }
            break;
          case(WALL):
            break;
          }
      }
    }
    EndDrawing();
  }

  void changeParticle(const vec2<f32> &pos, const ParticleType &type) {
    u32 gridX = static_cast<u32>(pos.x / particleSize.x);
    u32 gridY = static_cast<u32>(pos.y / particleSize.y);
    gridX = std::max(std::min(gridX, gridSize.x-1), static_cast<u32>(0));
    gridY = std::max(std::min(gridY, gridSize.y-1), static_cast<u32>(0));
    Particle *p = this->getParticle(vec2<u32>(gridX, gridY));
    if(p->type == EMPTY) {
      p->changeType(type);
    }
  }

  void move(const vec2<u32> &pos1, const vec2<u32> &pos2) {
    // move particle as pos1 to pos2
    if(pos2.x >= 0 && pos2.x < this->gridSize.x
        && pos2.y >= 0 && pos2.y < this->gridSize.y) {
      Particle *p = this->getParticle(pos1);
      Particle *dest = this->getParticle(pos2);
      if(dest->type == EMPTY) {
        dest->changeType(p->type);
        p->changeType(EMPTY);
      }
    }
  }
private:
  std::vector<Particle> particles;
  vec2<u32> windowSize, gridSize, particleSize;
};
