#include "lib/system_utils.h"

#include <string>

#ifdef _WIN32
#include <Windows.h>
#elif defined(__linux__)
#include <unistd.h>
#elif defined(__APPLE__)
#include <array>

#include <mach-o/dyld.h>
#include <sys/syslimits.h>
#endif

namespace lib {
auto get_base_path() -> std::filesystem::path {
  std::string path;
  path.resize(300);

#ifdef WIN32
  constexpr auto path_separator = "\\";

  GetModuleFileName(nullptr, path.data(), static_cast<DWORD>(path.size()));
#elif defined(__linux__)
  constexpr auto path_separator = "/";

  readlink("/proc/self/exe", path.data(), path.size());
#elif defined(__APPLE__)
  constexpr auto path_separator = "/";

  std::array<char, PATH_MAX> buf = {};
  uint32_t bufsize = PATH_MAX;
  _NSGetExecutablePath(buf.data(), &bufsize);

  path = std::string(buf.data(), bufsize);
#endif

  path = path.substr(0, path.find_last_of(path_separator));

  return std::filesystem::canonical(path);
}
} // namespace lib
