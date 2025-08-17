#include "result.hpp"
#include "window.hpp"

namespace core {
Window::Window(ui width, ui height, const std::string &title) {
  this->size = sf::Vector2u(width, height);
  this->title = title;
  this->delta = 0.f;
  sf::ContextSettings settings;
  settings.antiAliasingLevel = 16;
  this->_window = sf::RenderWindow(
      sf::VideoMode(this->size),
      sf::String(this->title),
      sf::Style::Default,
      sf::State::Windowed,
      settings);
  this->renderTexture = sf::RenderTexture(this->size, settings);
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
      if (event->is<sf::Event::Resized>()) {
        const sf::Event::Resized *ev = event->getIf<sf::Event::Resized>();
        this->size = ev->size;
      }
    }

    _window.setView(_window.getDefaultView());

    float delta = clock.restart().asSeconds();
    this->delta = delta;

    if (auto res = update(delta); !res)
      return res;

    this->_window.setSize(this->size);
    [[maybe_unused]] bool _ = this->renderTexture.resize(this->size);
    this->_window.clear(this->_bg_color);
    if (auto res = draw(delta); !res)
      return res;

    sf::Texture texture = this->renderTexture.getTexture();
    texture.setSmooth(false);
    this->_window.draw(sf::Sprite(texture));

    this->_window.display();
  }

  return core::Ok();
}
} // namespace core
