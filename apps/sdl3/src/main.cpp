#include <exception>
#include <string_view>
#include <vector>

#include <spdlog/spdlog.h>

#include "app.hpp"
#include "lib/common.hpp"

auto main(i32 argc, char* argv[]) -> i32 {
  auto args = std::vector<std::string_view>(argv, argv + argc);

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
