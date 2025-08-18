#pragma once

#include "core/drawables.hpp"
#include "core/renderer.hpp"
#include "core/result.hpp"
#include "core/stage.hpp"
#include "core/window.hpp"
#include <memory>


class TriangleStage {
public:
  TriangleStage(core::Renderer &renderer, core::Window &window);

  std::shared_ptr<core::Stage> GetStage() const {
    return stage;
  }

  void Update(float dt);

private:
  std::shared_ptr<core::Stage> stage;
  entt::entity triangleEntity;
  float rotation = 0.0f;

  core::Window &window;
  core::Renderer &renderer;
};
