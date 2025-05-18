function(enable_cache)
  find_program(CCACHE_PROGRAM ccache)

  if(NOT CCACHE_PROGRAM)
      message(WARNING "ccache not found")
      return()
  endif()

  message(STATUS "Using ccache")

  set(CMAKE_CXX_COMPILER_LAUNCHER
    ${CCACHE_PROGRAM}
    CACHE FILEPATH "CXX compiler cache"
  )

  set(CMAKE_C_COMPILER_LAUNCHER
    ${CCACHE_PROGRAM}
    CACHE FILEPATH "C compiler cache"
  )

  if(MSVC)
    message(STATUS "Setting MSVC debug information format to 'Embedded' to support caching")
    set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT
      "$<$<CONFIG:Debug,RelWithDebInfo>:Embedded>"
      PARENT_SCOPE
    )
  endif()
endfunction()
