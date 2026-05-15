#pragma once

#include <entt.hpp>

namespace NEXUS_CORE::ECS
{
  class Registry
  {
  public:
    Registry();
    ~Registry() = default;

    inline entt::registry& GetRegistry() { return *_Registry; }
    inline entt::entity CreateEntity() { return _Registry->create(); }

    template <typename TContext>
    TContext AddToContext(TContext context);

    template <typename TContext>
    TContext &GetContext();

  private:
    std::unique_ptr<entt::registry> _Registry;
  };

  template <typename TContext>
  inline TContext Registry::AddToContext(TContext context)
  {
    return _Registry->ctx().emplace<TContext>(context);
  }

  template <typename TContext>
  inline TContext &Registry::GetContext()
  {
    return _Registry->ctx().get<TContext>();
  }
}
