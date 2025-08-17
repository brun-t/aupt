#pragma once

#include "drawables.hpp"
#include "result.hpp"
#include <concepts>
#include <entt/entt.hpp>
#include <type_traits>

namespace core {

struct DrawableComponent {
  std::shared_ptr<Drawable> drawable;
};

class Stage {
public:
  Stage();

  inline entt::entity Create() {
    return registry.create();
  }

  template <class Type, class... Args>
  void Emplace(entt::entity entity, Args &&...args) {
    registry.emplace<Type>(entity, std::forward<Args>(args)...);
  }

  template <typename Func>
  Result<void> Foreach(Func &&cb) {
    auto view = registry.view<Transform, DrawableComponent>();
    static_assert(std::invocable<Func, decltype(view), entt::entity>, "Callback must be invocable with (view, entity)");
    static_assert(std::same_as<std::invoke_result_t<Func, decltype(view), entt::entity>, Result<void>>, "Callback must return Result<void>");

    for (auto entity : view) {
      const auto result = cb(view, entity);
      if (!result) {
        return Err(result.error_string());
      }
    }
    return Ok();
  }

  template <typename T>
  T &Get(entt::entity entity) {
    return registry.get<T>(entity);
  }

private:
  entt::registry registry;
};

} // namespace core