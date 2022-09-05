#pragma once

#include "raylib.h"
#include "../util/std.hpp"

enum ParticleType {
  EMPTY = 0x00,
  SAND,
  WALL
};

struct Particle {
  Particle(const vec2<u32> &pos, const ParticleType type);
  void changeType(const ParticleType &type);
  Color color;
  vec2<u32> pos;
  ParticleType type;
};

struct ParticleManager {
  ParticleManager(const vec2<u32> &windowSize, const vec2<u32> &gridSize);
  Particle *getParticle(const vec2<u32> &pos);
  void drawParticle(const vec2<u32> &pos);
  void updateParticles();
  void changeParticle(const vec2<f32> &pos, const ParticleType &type);
  void move(const vec2<u32> &pos1, const vec2<u32> &pos2);
private:
  std::vector<Particle> particles;
  vec2<u32> windowSize, gridSize, particleSize;
};


template <ParticleType t>
struct ParticleMover {
  static void move(ParticleManager *manager, Particle *p) {
  };
};

template <>
struct ParticleMover<SAND> {
  static void move(ParticleManager *manager, Particle *p) {
    // sand moving logic
    if(manager->getParticle(p->pos + vec2<u32>(0, 1))->type == EMPTY) {
      manager->move(p->pos, p->pos + vec2<u32>(0, 1));
    } else if(manager->getParticle(p->pos + vec2<u32>(1, 1))->type == EMPTY) {
      manager->move(p->pos, p->pos + vec2<u32>(1, 1));
    } else if(
        manager->getParticle(p->pos + vec2<u32>(0, 1) - vec2<u32>(1, 0))->type == EMPTY) {
      manager->move(p->pos, p->pos + vec2<u32>(0, 1) - vec2<u32>(1, 0));
    }
  }
};
