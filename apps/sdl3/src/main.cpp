#include <exception>
#include <string_view>
#include <vector>

#include "spdlog/spdlog.h"

#include "app.hpp"
#include "lib/version.hpp"

auto main(const int argc, char* argv[]) -> int {
  auto args = std::vector<std::string_view>(argv, argv + argc);

  spdlog::info("{} {}", version::PROJECT_NAME, version::PROJECT_VERSION);
  spdlog::info("Git SHA: {}", version::GIT_SHA);

  try {
    auto app = App(args);
    app.run();
  } catch (const std::exception& error) {
    spdlog::error("Error: {}", error.what());
  } catch (...) {
    spdlog::error("Unknown error.");
  }

  return 0;
}
