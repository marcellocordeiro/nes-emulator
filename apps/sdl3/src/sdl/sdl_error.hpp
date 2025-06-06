#pragma once

#include <exception>
#include <format>
#include <string>
#include <utility>

#include "sdl_include.hpp"

namespace sdl {
class Error final: public std::exception {
public:
  [[nodiscard]]
  auto what() const noexcept -> const char* override {
    return message_.c_str();
  }

  [[nodiscard]]
  auto static from_context_with_source(std::string source) -> Error {
    const auto* raw_error = SDL_GetError();
    auto wrapped_error = std::string(raw_error);

    clear_context_error();

    return {std::move(source), wrapped_error};
  }

  [[nodiscard]]
  auto get_source() const -> std::string {
    return source_;
  }

  [[nodiscard]]
  auto get_error() const -> std::string {
    return error_;
  }

  [[nodiscard]]
  auto get_message() const -> std::string {
    return message_;
  }

private:
  [[nodiscard]]
  Error(std::string source, std::string error) :
    source_(source),
    error_(error),
    message_(std::format("{}: {}", source, error)) {}

  static void clear_context_error() {
    SDL_ClearError();
  }

  std::string source_;
  std::string error_;
  std::string message_;
};
} // namespace sdl
