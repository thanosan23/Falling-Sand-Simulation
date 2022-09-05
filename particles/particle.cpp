#include "particle.hpp"

Particle::Particle(const vec2<u32> &pos, const ParticleType type) {
  this->pos = std::move(pos);
  this->changeType(type);
}

void Particle::changeType(const ParticleType &type) {
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


ParticleManager::ParticleManager(const vec2<u32> &windowSize, const vec2<u32> &gridSize) {
  this->windowSize = std::move(windowSize);
  this->gridSize = std::move(gridSize);
  assert(this->windowSize.x % this->gridSize.x == 0);
  assert(this->windowSize.y % this->gridSize.y == 0);
  // calculate size of each particle
  this->particleSize = vec2<u32>(this->windowSize.x / this->gridSize.x,
      this->windowSize.y / this->gridSize.y);

  // generate particles
  for(u32 y = 0; y < this->gridSize.y; ++y) {
    for(u32 x = 0; x < this->gridSize.x; ++x) {
      particles.emplace_back(std::move(Particle(vec2<u32>(x, y), EMPTY)));
    }
  }
}

Particle *ParticleManager::getParticle(const vec2<u32> &pos) {
  // get address of a particle at a given position
  return &particles[pos.y * this->gridSize.x + pos.x];
}

void ParticleManager::drawParticle(const vec2<u32> &pos) {
  // Draws a particle
  Particle *p = this->getParticle(pos);
  DrawRectangle(p->pos.x * this->particleSize.x,
      p->pos.y * this->particleSize.y, this->particleSize.x,
      this->particleSize.y, p->color);
}

void ParticleManager::updateParticles() {
  // Draw and then update particles
  BeginDrawing();
  ClearBackground(RAYWHITE);

  for(u32 y = 0; y < this->gridSize.y; ++y) {
    for(u32 x = 0; x < this->gridSize.x; ++x) {

      this->drawParticle(vec2<u32>(x, y));

      Particle *p = this->getParticle(vec2<u32>(x, y));
      switch(p->type) {
        case EMPTY:
        case WALL:
          break;
        case SAND:
          ParticleMover<SAND>::move(this, p);
          break;
      }
    }
  }

  EndDrawing();
}

void ParticleManager::changeParticle(const vec2<f32> &pos, const ParticleType &type) {
  // change particle from empty to a specific type
  u32 gridX = static_cast<u32>(pos.x / particleSize.x);
  u32 gridY = static_cast<u32>(pos.y / particleSize.y);
  // clamp positions so that the positions is bounded by the window
  gridX = std::max(std::min(gridX, gridSize.x-1), static_cast<u32>(0));
  gridY = std::max(std::min(gridY, gridSize.y-1), static_cast<u32>(0));
  // get particle and change type
  Particle *p = this->getParticle(vec2<u32>(gridX, gridY));
  if(p->type == EMPTY) {
    p->changeType(type);
  }
}

void ParticleManager::move(const vec2<u32> &pos1, const vec2<u32> &pos2) {
  // move particle from pos1 to pos2

  // make sure that particle is within the window
  if(pos2.x >= 0 && pos2.x < this->gridSize.x
      && pos2.y >= 0 && pos2.y < this->gridSize.y) {
    // get particles
    Particle *p = this->getParticle(pos1);
    Particle *dest = this->getParticle(pos2);
    // swap types
    ParticleType temp = dest->type;
    dest->changeType(p->type);
    p->changeType(temp);
  }
}


