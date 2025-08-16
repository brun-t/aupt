#include "renderer.hpp"

namespace core {
Renderer::Renderer(Window &window)
    : _window(window){};

Result<void> Renderer::Render() {
  this->_window.renderTexture.clear(this->_window._bg_color);
  Result<void> result;
  for (auto &drawable : this->drawables) {
    result = (*drawable)
                 .Draw(this->_window.delta,
                       this->_window.renderTexture)
                 .match(
                     [this](std::pair<const sf::Drawable *, sf::Transform> pair) -> Result<void> {
                       this->_window.renderTexture.draw(*pair.first, pair.second);
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
