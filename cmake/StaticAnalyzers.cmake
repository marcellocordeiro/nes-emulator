function(enable_clang_tidy)
  find_program(CLANG_TIDY_PROGRAM clang-tidy)

  if(NOT CLANG_TIDY_PROGRAM)
    message(WARNING "clang-tidy not found")
    return()
  endif()

  set(CLANG_TIDY_OPTIONS
    ${CLANG_TIDY_PROGRAM}
  )

  message(STATUS "clang-tidy enabled")
  set(CMAKE_CXX_CLANG_TIDY ${CLANG_TIDY_OPTIONS} PARENT_SCOPE)
endfunction()

function(enable_cppcheck)
  find_program(CPPCHECK_PROGRAM cppcheck)

  if(NOT CPPCHECK_PROGRAM)
    message(WARNING "cppcheck not found")
    return()
  endif()

  set(CPPCHECK_OPTIONS
    ${CPPCHECK_PROGRAM}
    --check-level=exhaustive
  )

  message(STATUS "cppcheck enabled")
  set(CMAKE_CXX_CPPCHECK ${CPPCHECK_OPTIONS} PARENT_SCOPE)
endfunction()

function(enable_iwyu)
  find_program(IWYU_PROGRAM include-what-you-use)

  if(NOT IWYU_PROGRAM)
    message(WARNING "include-what-you-use not found")
    return()
  endif()

  set(IWYU_OPTIONS
    ${IWYU_PROGRAM}
  )

  message(STATUS "include-what-you-use enabled")
  set(CMAKE_CXX_INCLUDE_WHAT_YOU_USE ${IWYU_OPTIONS} PARENT_SCOPE)
endfunction()
