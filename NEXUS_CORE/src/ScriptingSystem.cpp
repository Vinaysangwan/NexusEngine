#include "Core/Systems/ScriptingSystem.h"

#include "Core/ECS/Components/ScriptComponent.h"
#include "Core/ECS/Entity.h"

#include <Logger/Logger.h>

namespace NEXUS_CORE::Systems
{
  ScriptingSystem::ScriptingSystem(NEXUS_CORE::ECS::Registry& registry)
    : _Registry{registry}, _MainLoaded{false}
  {
  }
  
  bool ScriptingSystem::LoadMainScript(sol::state &lua)
  {
    try
    {
      auto result = lua.safe_script_file("C:/coding/C++/projects/NexusEngine/NEXUS_EDITOR/assets/scripts/main.lua");
    }
    catch (const sol::error& err)
    {
      NEXUS_ERROR("Error Loading the main.lua script: {}", err.what());
      return false;
    }

    sol::table main_lua = lua["main"];
    sol::optional<sol::table> updateExists = main_lua[1];
    if (updateExists == sol::nullopt)
    {
      NEXUS_ERROR("There is no update funciton in main.lua");
      return false;
    }

    sol::table update_script = main_lua[1];
    sol::function update = update_script["update"];

    sol::optional<sol::table> renderExists = main_lua[2];
    if (renderExists == sol::nullopt)
    {
      NEXUS_ERROR("There is no render function in main.lua");
      return false;
    }

    sol::table render_script = main_lua[2];
    sol::function render = render_script["render"];

    NEXUS_CORE::ECS::Entity mainLuaScript(_Registry, "main_script", "");
    mainLuaScript.AddComponent<NEXUS_CORE::ECS::ScriptComponent>(NEXUS_CORE::ECS::ScriptComponent{
      .update = update,
      .render = render
    });
    
    _MainLoaded = true;
    return true;
  }

  void ScriptingSystem::Update()
  {
    if (!_MainLoaded)
    {
      NEXUS_ERROR("Main lua script has not been loaded!");
      return;
    }

    auto view = _Registry.GetRegistry().view<NEXUS_CORE::ECS::ScriptComponent>();

    for (const auto& entity : view)
    {
      NEXUS_CORE::ECS::Entity ent{_Registry, entity};
      if (ent.GetName() != "main_script")
      {
        continue;
      }

      auto& script = ent.GetComponent<NEXUS_CORE::ECS::ScriptComponent>();
      auto error = script.update(entity);
      if (!error.valid())
      {
        sol::error err = error;
        NEXUS_ERROR("Error Running the update script: {}", err.what());
      }
    }
  }

  void ScriptingSystem::Render()
  {
    if (!_MainLoaded)
    {
      NEXUS_ERROR("Main lua script has not been loaded!");
      return;
    }

    auto view = _Registry.GetRegistry().view<NEXUS_CORE::ECS::ScriptComponent>();

    for (const auto& entity : view)
    {
      NEXUS_CORE::ECS::Entity ent{_Registry, entity};
      if (ent.GetName() != "main_script")
      {
        continue;
      }

      auto& script = ent.GetComponent<NEXUS_CORE::ECS::ScriptComponent>();
      auto error = script.render(entity);
      if (!error.valid())
      {
        sol::error err = error;
        NEXUS_ERROR("Error Running the render script: {}", err.what());
      }
    }
  }
}
