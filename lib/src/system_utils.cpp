#include "system_utils.h"

#include <string>

#ifdef _WIN32
#include <Windows.h>
#elif defined(__linux__)
#include <unistd.h>
#endif

namespace lib {
std::filesystem::path get_base_path()
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
