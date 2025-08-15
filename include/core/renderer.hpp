#pragma once

#include "window.hpp"
#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>

namespace core {
class Drawable {
public:
  virtual ~Drawable() = default;
  virtual Result<const sf::Drawable *> Draw(float, sf::RenderTexture &) = 0;
};
class Renderer {
public:
  using Drawable_ptr = std::unique_ptr<Drawable>;
  Renderer(Window &window);
  void AddDrawable(Drawable_ptr &&drawable);
  Result<void> Render();

private:
  std::vector<Drawable_ptr> drawables;
  Window &_window;
};
} // namespace core