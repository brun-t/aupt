#include "core/drawables.hpp"
#include "core/renderer.hpp"
#include "core/result.hpp"
#include "core/stage.hpp"
#include "core/window.hpp"

#include <iostream>

int main() {
  core::Window window(800, 600, "Mesh Example with ECS");
  core::Renderer renderer(window);
  core::Stage stage;

  // Handle close event
  core::Assert(window.EventHandler([&window](const core::Event &ev) {
    if (ev.is<core::Events::Closed>())
      window.Close();
  }));

  // Set background color
  core::Assert(window.SetBg("#333"));

  // === Create Triangle Entity ===
  entt::entity triangleEntity = stage.Create();
  auto triangle = std::make_shared<core::Mesh>();

  core::Assert(triangle->AddVertex({0.f, -50.f}, "#f00"));
  core::Assert(triangle->AddVertex({50.f, 50.f}, "#0f0"));
  core::Assert(triangle->AddVertex({-50.f, 50.f}, "#00f"));

  stage.Emplace<core::DrawableComponent>(triangleEntity, triangle);
  stage.Emplace<core::Transform>(triangleEntity, triangle->GetTransform());

  // === Create Text Entity ===
  entt::entity textEntity = stage.Create();
  auto text = std::make_shared<core::Text>("Hola Aupt!");
  core::Assert(text->SetColor("#fff"));

  stage.Emplace<core::DrawableComponent>(textEntity, text);
  stage.Emplace<core::Transform>(textEntity, text->GetTransform());

  // Add drawables from stage to renderer
  core::Assert(renderer.AddFromStage(stage));

  float rotation = 0.f;

  // Main loop using ECS
  core::Assert(window.Run(
      []() -> core::Result<void> {
        return core::Ok();
      },
      [&](float dt) -> core::Result<void> {
        // Animate triangle rotation
        auto &transform = stage.Get<core::Transform>(triangleEntity);
        rotation += 100.f * dt;
        transform.rotation = rotation;

        // Get current window size
        glm::vec2 size = window.GetSize();

        // Compute new centered position
        glm::vec2 newPos = {std::floor(size.x * 0.5f), std::floor(size.y * 0.5f)};

        transform.position = newPos;

        renderer.SetCamera({0.f, 0.f}, size);

        renderer.CameraFollow(transform.position);

        return core::Ok();
      },
      [&](float) -> core::Result<void> {
        core::Assert(renderer.SyncFromStage(stage));
        return renderer.Render();
      }));

  core::DisplayErrors();
  return 0;
}
