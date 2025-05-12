include("cmake/get_cpm.cmake")

CPMAddPackage(
  URI "gh:fmtlib/fmt#11.2.0"
)

CPMAddPackage(
  URI "gh:gabime/spdlog#v1.15.3"
  OPTIONS
    "SPDLOG_FMT_EXTERNAL ON"
    "SPDLOG_BUILD_SHARED OFF"
)

# Not needed
find_package(fmt CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)

# Rely on the system package instead
find_package(SDL3 CONFIG REQUIRED COMPONENTS SDL3-shared)

# Testing stuff
CPMAddPackage(
  URI "gh:catchorg/Catch2#v3.8.1"
)

list(APPEND CMAKE_MODULE_PATH ${Catch2_SOURCE_DIR}/extras)

find_package(Catch2 CONFIG REQUIRED)
include(CTest)
include(Catch)
