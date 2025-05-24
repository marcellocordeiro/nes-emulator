set(CMAKE_COLOR_DIAGNOSTICS ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Support for C++20 modules is still poor.
# This causes issues with clang-tidy and MSVC.
message(NOTICE "C++20 modules support is disabled")
set(CMAKE_CXX_SCAN_FOR_MODULES OFF)

# WIP
function(get_compiler_friendly_name result)
  message("Compiler ID: ${CMAKE_CXX_COMPILER_ID}")
  message("Compiler frontend variant: ${CMAKE_CXX_COMPILER_FRONTEND_VARIANT}")

  if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set(${result} "GCC" PARENT_SCOPE)
    return()
  endif()

  if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    set(${result} "MSVC" PARENT_SCOPE)
    return()
  endif()

  # Clang variants
  if(CMAKE_CXX_COMPILER_ID MATCHES ".*Clang")
    if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
      set(${result} "Clang" PARENT_SCOPE)
      return()
    endif()

    if(CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang")
      set(${result} "AppleClang" PARENT_SCOPE)
      return()
    endif()

    if(CMAKE_CXX_COMPILER_FRONTEND_VARIANT STREQUAL "MSVC")
      set(${result} "Clang-CL" PARENT_SCOPE)
      return()
    endif()
  endif()

  message(FATAL_ERROR "Unsupported compiler")
endfunction()

get_compiler_friendly_name(COMPILER_NAME)

message("Resolved compiler name: ${COMPILER_NAME}")
