#pragma once

#include <format>
#include <stdexcept>
#include <string>
#include <string_view>

#include <SDL3/SDL.h>

namespace SDLHelpers {
class Error final : public std::exception {
public:
  [[nodiscard]]
  auto what() const noexcept -> const char* override {
    return message.c_str();
  }

  [[nodiscard]]
  auto static fromContextWithSource(std::string source) -> Error {
    auto rawError = SDL_GetError();
    auto wrappedError = std::string(rawError);

    clearContextError();

    return Error(source, wrappedError);
  }

  [[nodiscard]]
  auto getSource() const -> std::string_view {
    return source;
  }

  [[nodiscard]]
  auto getError() const -> std::string_view {
    return error;
  }

  [[nodiscard]]
  auto getMessage() const -> std::string_view {
    return message;
  }

private:
  [[nodiscard]]
  Error(std::string source, std::string error) :
    source(source),
    error(error),
    message(std::format("Error in {}: {}", source, error)) {}

  static void clearContextError() {
    SDL_ClearError();
  }

  std::string source;
  std::string error;
  std::string message;
};
} // namespace SDLHelpers
