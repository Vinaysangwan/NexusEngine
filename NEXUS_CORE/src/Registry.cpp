#include "Core/ECS/Registry.h"

namespace NEXUS_CORE::ECS
{
  Registry::Registry()
    : _Registry(nullptr)
  {
    _Registry = std::make_unique<entt::registry>();
  }
}