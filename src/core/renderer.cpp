#include "renderer.hpp"
#include "stage.hpp"

namespace core {
Renderer::Renderer(Window &window)
    : _window(window){};

Result<void> Renderer::AddFromStage(Stage &stage) {
  return stage.Foreach([this](auto view, entt::entity entity) -> Result<void> {
    auto &drawableComp = view.template get<core::DrawableComponent>(entity);
    this->AddDrawable(drawableComp.drawable);
    return core::Ok();
  });
}

Result<void> Renderer::SyncFromStage(Stage &stage) {
  return stage.Foreach([](auto view, entt::entity entity) -> Result<void> {
    auto &transform = view.template get<Transform>(entity);
    auto &drawableComp = view.template get<DrawableComponent>(entity);

    if (drawableComp.drawable) {
      drawableComp.drawable->SetPosition(transform.position);
      drawableComp.drawable->SetScale(transform.scale);
      drawableComp.drawable->SetRotation(transform.rotation);
    }

    return core::Ok();
  });
}

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
