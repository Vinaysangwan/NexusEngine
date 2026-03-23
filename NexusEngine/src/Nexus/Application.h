#pragma once

#include "Core.h"

namespace Nexus
{

	class NX_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();
	};

	// Defined By the Client
	Application* CreateApplication();

}

