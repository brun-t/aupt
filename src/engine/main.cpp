#include "core/drawables.hpp"
#include "core/renderer.hpp"
#include "core/result.hpp"
#include "core/window.hpp"

#include <iostream>

int main() {
  core::Window window(800, 600, "Mesh Example");
  core::Renderer renderer(window);

  window.EventHandler([&window](const core::Event &ev) {
    if (ev.is<core::Events::Closed>())
      window.Close();
  });

  window.SetBg("#333333");

  auto triangle = std::make_shared<core::Mesh>();

  triangle->AddVertex({0.f, -50.f}, sf::Color::Red);
  triangle->AddVertex({50.f, 50.f}, sf::Color::Green);
  triangle->AddVertex({-50.f, 50.f}, sf::Color::Blue);
  triangle->SetPosition({400.f, 300.f}); // center screen

  renderer.AddDrawable(triangle);

  float rotation = 0.f;

  auto result = window.Run(
      [&renderer]() -> core::Result<void> {
        return core::Ok(); // Check for any failures here
      },
      [&triangle, &rotation](float dt) -> core::Result<void> {
        //std::cout << "Delta: " << dt << std::endl;
        rotation += 100 * dt; // Make sure rotation is updated over time
        //std::cout << "Rotation:" << sf::degrees(rotation).asDegrees() << std::endl;
        triangle->SetRotation(rotation);
        return core::Ok();
      },
      [&renderer](float dt) -> core::Result<void> {
        return renderer.Render(); // Render updates
      });

  return result.match(
      []() -> int { return 0; },
      [](std::string err) -> int {
        std::cerr << err;
        return 1;
      });
}
