#pragma once

#ifdef NX_PLATFORM_WINDOWS

extern Nexus::Application* Nexus::CreateApplication();

int main(int argc, char** argv)
{
	Nexus::Log::Init();
	NX_CORE_WARN("Initailized Log!");
	NX_INFO("Hello From APP!");

	auto app = Nexus::CreateApplication();
	app->Run();
	delete app;
}

#endif

