#include "Entity.h"

namespace NEXUS_CORE::ECS
{
  template <typename TComponent, typename... Args>
  inline TComponent &Entity::AddComponent(Args &&...args)
  {
    auto &registry = _Registry.GetRegistry();
    return registry.emplace<TComponent>(_Entity, std::forward<Args>(args) ...);
  }

  template <typename TComponent, typename... Args>
  inline void Entity::ReplaceComponent(Args &...args)
  {
    auto &registry = _Registry.GetRegistry();
    if (registry.all_of<TComponent>(_Entity))
    {
      return registry.replace<TComponent>(_Entity, std::forward<Args>(args) ...);
    }
    else
    {
      return registry.emplace<TComponent>(_Entity, std::forward<Args>(args) ...);
    }
  }

  template <typename TComponent>
  inline TComponent &Entity::GetComponent()
  {
    auto &registry = _Registry.GetRegistry();
    return registry.get<TComponent>(_Entity);
  }

  template <typename TComponent>
  inline bool Entity::HasComponent()
  {
    auto &registry = _Registry.GetRegistry();
    return registry.all_of<TComponent>(_Entity);
  }

  template <typename TComponent>
  inline void Entity::RemoveComponent()
  {
    auto &registry = _Registry.GetRegistry();
    return registry.remove<TComponent>(_Entity);
  }
}
