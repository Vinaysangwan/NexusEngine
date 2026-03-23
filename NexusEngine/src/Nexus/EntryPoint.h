#pragma once

#ifdef NX_PLATFORM_WINDOWS

extern Nexus::Application* Nexus::CreateApplication();

int main(int argc, char** argv)
{
	auto app = Nexus::CreateApplication();
	app->Run();
	delete app;
}

#endif
