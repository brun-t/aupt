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
  template <typename T>
  inline void AddDrawable(T drawable) requires(std::derived_from<T, Drawable> &&std::is_move_constructible_v<T>) {
    this->drawables.push_back(std::make_unique<T>(std::move(drawable)));
  }
  Result<void> Render();

private:
  std::vector<Drawable_ptr> drawables;
  Window &_window;
};
} // namespace core