#include "system_utils.h"

#include <string>

#ifdef _WIN32
#include <Windows.h>
#elif defined(__linux__)
#include <unistd.h>
#endif

namespace lib {
void message_box(const char* message)
{
#ifdef _WIN32
  MessageBox(NULL, message, "Error!", MB_OK);
#elif defined(__linux__)
  std::string command = "gdialog --title \"Error!\" --msgbox \"";
  command += std::string(message);
  command += "\"";
  system(command.c_str());
#endif
}

void hide_console()
{
#ifdef _WIN32
  ShowWindow(GetConsoleWindow(), SW_HIDE);
#endif
}

void show_console()
{
#ifdef _WIN32
  ShowWindow(GetConsoleWindow(), SW_SHOW);
#endif
}

std::filesystem::path get_executable_path()
{
  std::string path;
  path.resize(300);

#ifdef WIN32
  GetModuleFileName(nullptr, path.data(), path.size());
  constexpr auto path_separator = "\\";
#elif defined(__linux__)
  readlink("/proc/self/exe", path.data(), path.size());
  constexpr auto path_separator = "/";
#endif

  path = path.substr(0, path.find_last_of(path_separator));

  return std::filesystem::canonical(path);
}
}  // namespace lib
