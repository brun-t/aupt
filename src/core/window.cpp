#include "result.hpp"
#include "window.hpp"

namespace core {
Window::Window(ui width, ui height, const std::string &title) {
  this->height = height;
  this->width = width;
  this->title = title;
  this->delta = 0.f;
  this->_window = sf::RenderWindow(sf::VideoMode({this->width, this->height}), this->title);
  this->renderTexture = sf::RenderTexture({this->width, this->height});
}

Result<void> Window::SetBg(const std::string &bg) {
  return core::parseHexColor(bg).match(
      [this](auto color) -> Result<void> {
        this->_bg_color = color;
        return core::Ok();
      },
      [](auto err) -> Result<void> {
        return core::Err<void>(err);
      });
}

Result<void> Window::Run(Load load, Update update, Draw draw) {
  sf::Clock clock;

  Result<void> result = load();
  if (!result)
    return result;

  while (this->_window.isOpen()) {
    while (const std::optional event = this->_window.pollEvent()) {
      this->handle(Event(*event));
    }

    float delta = clock.restart().asSeconds();
    this->delta = delta;

    if (auto res = update(delta); !res)
      return res;
    this->_window.clear(this->_bg_color);
    if (auto res = draw(delta); !res)
      return res;

    this->_window.draw(sf::Sprite(this->renderTexture.getTexture()));

    this->_window.display();
  }

  return core::Ok();
}
} // namespace core
