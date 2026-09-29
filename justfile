# Documentation: https://github.com/casey/just
# Cheat sheet: https://cheatography.com/linux-china/cheat-sheets/justfile/

# List all available scripts
[private]
default:
  @just --list --unsorted

# Init submodules. Warning: may discard changes
[group("configuration")]
init-submodules:
  git submodule update --init --recursive

# Format C/C++ source files and headers
[unix]
[group("maintenance")]
format-cpp:
  #!/usr/bin/env zsh
  clang-format -i ./apps/**/**.{cpp,hpp}
  clang-format -i ./core/**/**.h

# Update all project dependencies (vcpkg)
[group("maintenance")]
update:
  vcpkg x-update-baseline
