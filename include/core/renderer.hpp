#pragma once

#include "window.hpp"
#include <SFML/Graphics.hpp>
#include <glm/vec2.hpp>
#include <memory>
#include <utility>
#include <vector>

namespace core {
class Drawable {
public:
  virtual ~Drawable() = default;
  virtual Result<std::pair<const sf::Drawable *, sf::Transform>> Draw(float, sf::RenderTexture &) = 0;
  virtual void SetPosition(const glm::vec2 &) = 0;
  virtual void SetRotation(float) = 0;
  virtual void SetScale(const glm::vec2 &) = 0;
};
class Renderer {
public:
  // Change Drawable_ptr to use shared_ptr
  using Drawable_ptr = std::shared_ptr<Drawable>;
  Renderer(Window &window);
  // Change AddDrawable to take a shared_ptr by value
  inline void AddDrawable(Drawable_ptr drawable) {
    this->drawables.push_back(drawable);
  }
  Result<void> Render();

private:
  std::vector<Drawable_ptr> drawables;
  Window &_window;
};
} // namespace core