#include "renderer.hpp"
#include "stage.hpp"

namespace core {
Renderer::Renderer(Window &window)
    : _window(window){};

void Renderer::AddStage(Stage_ptr stage) {
  stages.push_back(stage);
  core::Assert(AddFromStage(*stage));
}

Result<void> Renderer::AddFromStage(Stage &stage) {
  return stage.Foreach<Transform, DrawableComponent>([this](entt::entity entity, Transform &_, DrawableComponent &drawableComp) -> Result<void> {
    this->AddDrawable(drawableComp.drawable);
    return core::Ok();
  });
}

Result<void> Renderer::SyncFromStage(Stage &stage) {
  return stage.Foreach<Transform, DrawableComponent>([](entt::entity entity, Transform &transform, DrawableComponent &drawableComp) -> Result<void> {
    if (drawableComp.drawable) {
      drawableComp.drawable->SetPosition(transform.position);
      drawableComp.drawable->SetScale(transform.scale);
      drawableComp.drawable->SetRotation(transform.rotation);
    }

    return core::Ok();
  });
}

Result<void> Renderer::Render() {
  for (auto &stage : stages) {
    core::Assert(SyncFromStage(*stage));
  }
  _window.renderTexture.clear(_window._bg_color);

  for (auto &drawable : drawables) {
    auto result = drawable->Draw(_window.delta, _window.renderTexture)
                      .match(
                          [this](std::pair<const sf::Drawable *, sf::Transform> pair) -> Result<void> {
                            _window.renderTexture.draw(*pair.first, pair.second);
                            return Ok();
                          },
                          [](auto err) -> Result<void> {
                            return Err(err);
                          });

    if (!result)
      return result;
  }

  _window.renderTexture.display();
  return Ok();
}

} // namespace core
