#include "core/result.hpp"
#include "core/window.hpp"

int main() {
  core::Window window(600u, 600u, "Hello Aupt");

  window.EventHandler([&window](const core::Event &ev) {
    if (ev.is<core::Events::Closed>())
      window.Close();
  });

  window.SetBg("#babaee");

  return window.Run(
      []() -> core::Result<void> {
        // Load resources
        return core::Ok();
      },
      [](float dt) -> core::Result<void> {
        // Update logic
        return core::Ok();
      },
      [](float dt) -> core::Result<void> {
        // Draw
        return core::Ok();
      });
}