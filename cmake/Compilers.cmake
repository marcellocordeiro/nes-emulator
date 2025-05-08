set(CMAKE_COLOR_DIAGNOSTICS ON)

# WIP
function(get_compiler_friendly_name result)
  message("Compiler ID: ${CMAKE_CXX_COMPILER_ID}")
  message("Compiler frontend variant: ${CMAKE_CXX_COMPILER_FRONTEND_VARIANT}")

  # Clang variants
  if(CMAKE_CXX_COMPILER_ID MATCHES ".*Clang")

    # Clang
    if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
      # clang
      set(${result} "clang" PARENT_SCOPE)
      return()
    endif()

    # AppleClang
    if(CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang")
      set(${result} "AppleClang" PARENT_SCOPE)
      return()
    endif()

    # Clang-cl
    if(CMAKE_CXX_COMPILER_FRONTEND_VARIANT STREQUAL "MSVC")
      set(${result} "clang-cl" PARENT_SCOPE)
      return()
    endif()
  endif()

  # GCC
  if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    # gcc
    set(${result} "gcc" PARENT_SCOPE)
    return()
  endif()

  # MSVC
  if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    set(${result} "msvc" PARENT_SCOPE)
    return()
  endif()

  message(FATAL_ERROR "Unsupported compiler")
endfunction()

get_compiler_friendly_name(COMPILER_NAME)

message("Resolved compiler name: ${COMPILER_NAME}")
