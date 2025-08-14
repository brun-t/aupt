#pragma once

#include "result.hpp"
#include "types.hpp"
#include "utils.hpp"
#include <SFML/Graphics.hpp>
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

  void Close() {
    this->_window.close();
  }

private:
  sf::RenderWindow _window;
  sf::Color _bg_color;
  ui width, height;
  std::string title;
  EventHandle handle;
};
} // namespace core
