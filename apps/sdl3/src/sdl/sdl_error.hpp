#pragma once

#include <exception>
#include <format>
#include <string>
#include <utility>

#include "sdl_include.hpp"

namespace SDL {
class Error final : public std::exception {
public:
  [[nodiscard]]
  auto what() const noexcept -> const char* override {
    return message.c_str();
  }

  [[nodiscard]]
  auto static from_context_with_source(std::string source) -> Error {
    const auto* rawError = SDL_GetError();
    auto wrappedError = std::string(rawError);

    clear_context_error();

    return {std::move(source), wrappedError};
  }

  [[nodiscard]]
  auto get_source() const -> std::string {
    return source;
  }

  [[nodiscard]]
  auto get_error() const -> std::string {
    return error;
  }

  [[nodiscard]]
  auto get_message() const -> std::string {
    return message;
  }

private:
  [[nodiscard]]
  Error(std::string source, std::string error) :
    source(source),
    error(error),
    message(std::format("Error in {}: {}", source, error)) {}

  static void clear_context_error() {
    SDL_ClearError();
  }

  std::string source;
  std::string error;
  std::string message;
};
} // namespace SDL
