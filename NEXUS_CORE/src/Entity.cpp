#include "Core/ECS/Entity.h"
#include "Core/ECS/Components/Identification.h"

namespace NEXUS_CORE::ECS
{
  Entity::Entity(Registry &registry, const entt::entity &entity)
    : _Registry{registry}, _Entity(entity), _Name{""}, _Group{""}
  {
    if (HasComponent<Identification>())
    {
      auto id = GetComponent<Identification>();
      _Name = id.name;
      _Group = id.group;
    }
  }

  Entity::Entity(Registry &registry)
    : Entity(registry, "GameObject", "")
  {
  }

  Entity::Entity(Registry &registry, const std::string &name, const std::string &group)
    : _Registry{registry},
      _Entity{registry.CreateEntity()},
      _Name{name}, _Group{group}
  {
    AddComponent<Identification>(Identification{
      .name = name,
      .group = group,
      .entity_id = static_cast<int32_t>(_Entity)
    });
  }
}
