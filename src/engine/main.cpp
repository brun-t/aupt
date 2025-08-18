#include "core/renderer.hpp"
#include "core/result.hpp"
#include "core/window.hpp"
#include "triangle.hpp"

// Hello Ninja I have obviously changed

int main() {
  core::Window window(800, 600, "Triangle scene");
  core::Renderer renderer(window);

  core::Assert(window.EventHandler([&window](const core::Event &ev) {
    if (ev.is<core::Events::Closed>())
      window.Close();
  }));

  core::Assert(window.SetBg("#333"));

  // Use composition — TriangleStage holds its own Stage
  TriangleStage triangleStage(renderer, window);
  renderer.AddStage(triangleStage);

  core::Assert(window.Run(
      []() -> core::Result<void> {
        return core::Ok(); // Load
      },
      [&](float dt) -> core::Result<void> {
        triangleStage.Update(dt);
        return core::Ok(); // Update
      },
      [&](float) -> core::Result<void> {
        return renderer.Render(); // Draw
      }));

  core::DisplayErrors();
  return 0;
}
