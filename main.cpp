#include "raylib.h"
#include "toml++/toml.h"

#include "util/std.hpp"
#include "particles/particle.hpp"

int main(UNUSED int argc, UNUSED char *argv[]) {
  // Parse Configs
  auto settings = toml::parse_file( "defaults.toml" );

  // Parse application configs
  auto applicationSettings = settings["application"];
  std::string name =
    *applicationSettings["name"].value<std::string>();
  u16 width = *applicationSettings["width"].value<u16>();
  u16 height = *applicationSettings["height"].value<u16>();
  u8 fps = *applicationSettings["fps"].value<u8>();

  const vec2<u32> windowSize = vec2<u32>(width, height);
  const vec2<u32> gridSize = vec2<u32>(160, 160);

  // Initialize window
  InitWindow(width, height, name.c_str());
  SetTargetFPS(fps);

  ParticleManager particles(windowSize, gridSize);

  vec2<> mousePos;

  while(!WindowShouldClose()) {

    if(IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
      mousePos = vec2<>(GetMouseX(), GetMouseY());
      particles.changeParticle(mousePos, SAND);
    }

    particles.updateParticles();

    // rendering
    BeginDrawing();
    ClearBackground(RAYWHITE);

    particles.drawParticles();

    EndDrawing();
  }

  CloseWindow();

  return 0;
}
