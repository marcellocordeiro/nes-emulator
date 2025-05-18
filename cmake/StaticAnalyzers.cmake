if(ENABLE_CLANG_TIDY)
  find_program(CLANG_TIDY_PROGRAM clang-tidy)

  if(NOT CLANG_TIDY_PROGRAM)
    message(WARNING "clang-tidy not found")
    return()
  endif()

  set(CLANG_TIDY_OPTIONS
    ${CLANG_TIDY_PROGRAM}
  )

  message(STATUS "clang-tidy enabled")
  set(CMAKE_CXX_CLANG_TIDY ${CLANG_TIDY_OPTIONS})
endif()
