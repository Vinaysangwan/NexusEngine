#pragma once

#include "Core/ECS/Registry.h"
#include <sol/sol.hpp>

namespace NEXUS_CORE::Systems
{
  class ScriptingSystem
  {
  public:
    ScriptingSystem(NEXUS_CORE::ECS::Registry& _registry);
    ~ScriptingSystem() = default;

    bool LoadMainScript(sol::state& lua);
    void Update();
    void Render();

  private:
    NEXUS_CORE::ECS::Registry& _Registry;
    bool _MainLoaded;
  };
}
