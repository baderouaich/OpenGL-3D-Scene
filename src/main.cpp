#include <pch.hpp>
#include "Application/Application.hpp"

int main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[]) try
{
  const std::unique_ptr<Application> app(new Application());
  app->Run();
  return EXIT_SUCCESS;
}
catch (const std::exception& e)
{
  std::cerr << "[EXCEPTION THROWN]: " << e.what() << std::endl;
  return EXIT_FAILURE;
}
catch (...)
{
  std::cerr << "[UNKNOWN EXCEPTION THROWN]" << std::endl;
  return EXIT_FAILURE;
}


/**
*	Windows Entry Point
*/
#if defined(_WIN32) && defined(RELEASE)
int WINAPI WinMain(
	[[maybe_unused]] _In_ HINSTANCE instance,
	[[maybe_unused]] _In_opt_ HINSTANCE prev_instance,
	[[maybe_unused]] _In_ LPSTR cmd_line,
	[[maybe_unused]] _In_ int cmd_show)
{
	return ::main(__argc, __argv);
}
#endif