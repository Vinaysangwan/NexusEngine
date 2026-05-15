#include "Application.h"

int main()
{
  auto &app = NEXUS_EDITOR::Application::GetInstance();
  app.Run();
}
