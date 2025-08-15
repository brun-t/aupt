#include "core/renderer.hpp"
#include "core/result.hpp"
#include "core/window.hpp"

class Circle : public core::Drawable {
public:
  Circle(float radius, sf::Vector2f position, sf::Color color)
      : shape(radius) {
    shape.setPosition(position);
    shape.setFillColor(color);
  }

  core::Result<const sf::Drawable *> Draw(float, sf::RenderTexture &) override {
    return core::OkWithValue(static_cast<const sf::Drawable *>(&shape));
  }

private:
  sf::CircleShape shape;
};

int main() {
  core::Window window(600u, 600u, "Circle Test");
  core::Renderer renderer(window);

  window.EventHandler([&window](const core::Event &ev) {
    if (ev.is<core::Events::Closed>())
      window.Close();
  });

  window.SetBg("#222244");

  renderer.AddDrawable(std::make_unique<Circle>(
      50.0f, sf::Vector2f(275.f, 275.f), sf::Color::Green));

  return window.Run(
      []() -> core::Result<void> {
        // Load resources
        return core::Ok();
      },
      [](float dt) -> core::Result<void> {
        // Update logic (none for now)
        return core::Ok();
      },
      [&renderer](float dt) -> core::Result<void> {
        return renderer.Render();
      });
}
