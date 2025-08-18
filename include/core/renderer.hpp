#pragma once

#include "window.hpp"
#include <SFML/Graphics.hpp>
#include <entt/entt.hpp>
#include <glm/vec2.hpp>
#include <memory>
#include <utility>
#include <vector>

namespace core {
struct Transform {
  glm::vec2 position, scale;
  float rotation;
};

class Drawable {
public:
  virtual ~Drawable() = default;
  virtual Result<std::pair<const sf::Drawable *, sf::Transform>> Draw(float, sf::RenderTexture &) = 0;

  virtual void SetPosition(const glm::vec2 &) = 0;
  virtual void SetRotation(float) = 0;
  virtual void SetScale(const glm::vec2 &) = 0;

  virtual glm::vec2 GetPosition() = 0;
  virtual float GetRotation() = 0;
  virtual glm::vec2 GetScale() = 0;

  virtual Transform GetTransform() = 0;
};

class Stage;

class Renderer {
public:
  // Change Drawable_ptr to use shared_ptr
  using Drawable_ptr = std::shared_ptr<Drawable>;
  using Stage_ptr = std::shared_ptr<Stage>;
  Renderer(Window &window);
  // Change AddDrawable to take a shared_ptr by value
  inline void AddDrawable(Drawable_ptr drawable) {
    this->drawables.push_back(drawable);
  }

  inline void SetCamera(const glm::vec2 &position, const glm::vec2 &size) {
    _view = sf::View(sf::FloatRect(sf::Vector2f{position.x, position.y}, sf::Vector2f{size.x, size.y}));
    _window._window.setView(_view); // Make sure to apply the view to the window
  }

  inline void CameraFollow(const glm::vec2 &position) {
    _view.setCenter(sf::Vector2f{position.x, position.y});
    _window._window.setView(_view); // Update view after changing center
  }

  Result<void> AddFromStage(Stage &stage);
  Result<void> SyncFromStage(Stage &stage);
  void AddStage(Stage_ptr stage);

  Result<void> Render();

private:
  std::vector<Drawable_ptr> drawables;
  std::vector<Stage_ptr> stages;
  Window &_window;
  sf::View _view;
};
} // namespace core