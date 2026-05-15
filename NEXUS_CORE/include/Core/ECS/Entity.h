#pragma once

#include "Registry.h"

namespace NEXUS_CORE::ECS
{
  class Entity
  {
  public:
    Entity(Registry &registry);
    Entity(Registry &registry, const std::string &name, const std::string &group);
    ~Entity() = default;

    inline std::uint32_t Kill() {return _Registry.GetRegistry().destroy(_Entity);}

    inline entt::entity &GetEntity() {return _Entity;}
    inline entt::registry &GetRegistry() {return _Registry.GetRegistry();}

    template <typename TComponent, typename ...Args>
    TComponent& AddComponent(Args&& ...args);

    template <typename TComponent, typename ...Args>
    void ReplaceComponent(Args& ...args);

    template <typename TComponent>
    TComponent& GetComponent();

    template <typename TComponent>
    bool HasComponent();

    template <typename TComponent>
    void RemoveComponent();

  private:
    Registry &_Registry;
    entt::entity _Entity;
    std::string _Name, _Group;
  };
}

#include "Entity.inl"
