# By default CTest adds lot of targets like ContinuousBuild, NightlyBuild, etc...
# Let's disable them for now.
# TODO: unclear what these are for and how they can be helpful
set_property(GLOBAL PROPERTY CTEST_TARGETS_ADDED ON)
include(CTest)

if(BUILD_TESTING)
  find_package(Catch2 3 CONFIG REQUIRED)
  include(Catch)
endif()
