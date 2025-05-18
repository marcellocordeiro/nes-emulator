option(ENABLE_IPO "Enable IPO/LTO" OFF)
option(WARNINGS_AS_ERRORS "Treat warnings as errors" ON)
option(ENABLE_CLANG_TIDY "Enable clang-tidy" OFF)
option(ENABLE_IWYU "Enable include-what-you-use" OFF)
option(ENABLE_CPPCHECK "Enable cppcheck" OFF)
option(ENABLE_CACHE "Enable cache (ccache)" OFF)

if(ENABLE_IPO)
  include(cmake/InterproceduralOptimization.cmake)
  enable_ipo()
endif()

if(ENABLE_CACHE)
  include(cmake/Cache.cmake)
  enable_cache()
endif()

include(cmake/StaticAnalyzers.cmake)
