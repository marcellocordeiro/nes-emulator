function(enable_cache)
  find_program(CCACHE_PROGRAM ccache)

  if(CCACHE_PROGRAM)
    message("Using ccache")

    set(CMAKE_CXX_COMPILER_LAUNCHER
      ${CCACHE_PROGRAM}
      CACHE FILEPATH "CXX compiler cache"
    )

    set(CMAKE_C_COMPILER_LAUNCHER
      ${CCACHE_PROGRAM}
      CACHE FILEPATH "C compiler cache"
    )
  else()
    message("ccache not found")
  endif()
endfunction()
