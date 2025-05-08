include("cmake/get_cpm.cmake")

CPMAddPackage(
  URI "gh:fmtlib/fmt#11.2.0"
)

CPMAddPackage(
  URI "gh:gabime/spdlog#v1.15.2"
  OPTIONS
    "SPDLOG_FMT_EXTERNAL ON"
    "SPDLOG_BUILD_SHARED OFF"
)

find_package(fmt CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)

# Rely on the system package instead
find_package(SDL3 CONFIG REQUIRED COMPONENTS SDL3-shared)
