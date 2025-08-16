#pragma once

#include "result.hpp"
#include "types.hpp"
#include "utils.hpp"
#include <SFML/Graphics.hpp>
#include <glm/vec2.hpp>
#include <vector>

namespace core {
using Events = sf::Event;

// A wrapper over SFML so is easier to switch from it in the future
class Event {
public:
  Event(sf::Event ev)
      : ev(ev){};

  template <typename T>
  inline bool is() const {
    return (static_cast<sf::Event>(this->ev)).is<T>();
  }
  template <typename T>
  inline const T *getIf() const {
    return (static_cast<sf::Event>(this->ev)).getIf<T>();
  }

private:
  sf::Event ev;
};

// forward reference
class Renderer;

/**
 * @brief The Class for make a Window
 * @param width is the width of the window
 * @param height is the height of the window
 * @param title is the title of the window
 */
class Window {
public:
  Window(ui width, ui height, const std::string &title);

  using Load = std::function<Result<void>()>;
  using Update = std::function<Result<void>(float)>;
  using Draw = std::function<Result<void>(float)>;
  Result<void> Run(Load load, Update update, Draw draw);

  using EventHandle = std::function<void(const Event &)>;
  inline Result<void> EventHandler(EventHandle handle) {
    this->handle = handle;
    return Ok();
  }

  Result<void> SetBg(const std::string &hex);

  using vec2u = glm::vec<2, unsigned, glm::packed_highp>;
  inline void Resize(vec2u &size) {
    this->size = sf::Vector2u(size.x, size.y);
  }

  inline void Close() {
    this->_window.close();
  }

  inline float Time() const {
    return globalClock.getElapsedTime().asSeconds();
  }

  inline void SetFPSLimit(ui limit) {
    this->_window.setFramerateLimit(limit);
  }

private:
  friend class Renderer;

  sf::RenderWindow _window;
  sf::Color _bg_color;
  sf::RenderTexture renderTexture;
  sf::Vector2u size;
  sf::Clock globalClock;
  float delta;
  std::string title;
  EventHandle handle;
};
} // namespace core
