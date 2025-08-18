#pragma once

#include "drawables.hpp"
#include "result.hpp"
#include <concepts>
#include <entt/entt.hpp>
#include <functional>
#include <type_traits>
#include <vector>

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
  inline void Emplace(entt::entity entity, Args &&...args) {
    registry.emplace<Type>(entity, std::forward<Args>(args)...);
  }

  template <typename... Components, typename Func>
  inline Result<void> Foreach(Func &&cb) {
    auto view = registry.view<Components...>();

    static_assert(std::invocable<Func, entt::entity, Components &...>, "Callback must accept (entity, components...)");
    static_assert(std::same_as<std::invoke_result_t<Func, entt::entity, Components &...>, Result<void>>, "Callback must return Result<void>.");

    for (auto entity : view) {
      auto componentsTuple = view.get<Components...>(entity);
      auto result = std::apply(
          [&](Components &...comps) -> Result<void> {
            return cb(entity, comps...);
          },
          componentsTuple);
      Assert(result);
    }

    return Ok();
  }

  template <typename T>
  inline T &Get(entt::entity entity) {
    return registry.get<T>(entity);
  }

  template <typename T>
  inline [[nodiscard]] bool Has(entt::entity entity) {
    return registry.all_of<T>(entity);
  }

  using System = std::function<Result<void>(Stage &, float)>;
  inline void AddSystem(System system) {
    systems.push_back(system);
  }

  inline void RunSystems(float dt) {
    for (auto system : systems) {
      Assert(system(*this, dt));
    }
  }

  template <typename... Components>
  inline auto View() {
    return registry.view<Components...>();
  }

  template <typename... Components>
  inline auto View() const {
    return registry.view<Components...>();
  }

  template <typename T>
  inline void Remove(entt::entity entity) {
    registry.remove<T>(entity);
  }

  inline entt::registry &Registry() {
    return registry;
  }

private:
  entt::registry registry;
  std::vector<System> systems;
};

} // namespace core