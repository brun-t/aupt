#include "renderer.hpp"

namespace core {
Renderer::Renderer(Window &window)
    : _window(window){};

void Renderer::AddDrawable(Renderer::Drawable_ptr &&drawable) {
  this->drawables.push_back(std::move(drawable));
}

Result<void> Renderer::Render() {
  this->_window.renderTexture.clear(this->_window._bg_color);
  Result<void> result;
  for (auto &drawable : this->drawables) {
    result = (*drawable)
                 .Draw(this->_window.delta,
                       this->_window.renderTexture)
                 .match(
                     [this](const sf::Drawable *draw) -> Result<void> {
                       this->_window.renderTexture.draw(*draw);
                       return Ok();
                     },
                     [this](auto err) -> Result<void> {
                       return Err(err);
                     });
  }
  this->_window.renderTexture.display();

  return result;
}

} // namespace core
