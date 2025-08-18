#include "triangle.hpp"
#include <cmath>

TriangleStage::TriangleStage(core::Renderer &renderer, core::Window &window)
    : renderer(renderer), window(window), stage(std::make_shared<core::Stage>()) {

  // === Create Triangle ===
  triangleEntity = stage->Create();

  auto triangle = std::make_shared<core::Mesh>();
  core::Assert(triangle->AddVertex({0.f, -50.f}, "#f00"));
  core::Assert(triangle->AddVertex({50.f, 50.f}, "#0f0"));
  core::Assert(triangle->AddVertex({-50.f, 50.f}, "#00f"));

  stage->Emplace<core::DrawableComponent>(triangleEntity, triangle);
  stage->Emplace<core::Transform>(triangleEntity, triangle->GetTransform());

  // === Create Text ===
  auto text = std::make_shared<core::Text>("Hola Aupt!");
  core::Assert(text->SetColor("#fff"));

  auto textEntity = stage->Create();
  stage->Emplace<core::DrawableComponent>(textEntity, text);
  stage->Emplace<core::Transform>(textEntity, text->GetTransform());
}

void TriangleStage::Update(float dt) {
  if (!stage->Has<core::Transform>(triangleEntity))
    return;

  auto &transform = stage->Get<core::Transform>(triangleEntity);
  rotation += 100.f * dt;

  transform.rotation = rotation;

  glm::vec2 size = window.GetSize();
  transform.position = {std::floor(size.x * 0.5f), std::floor(size.y * 0.5f)};

  renderer.SetCamera({0.f, 0.f}, size);
  renderer.CameraFollow(transform.position);

  stage->RunSystems(dt);
}
