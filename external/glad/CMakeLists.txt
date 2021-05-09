project(glad LANGUAGES C)

add_library(glad STATIC "src/gl.c")

target_include_directories(glad SYSTEM PUBLIC "include")

set_target_properties(glad
  PROPERTIES
    C_STANDARD 11
    C_STANDARD_REQUIRED YES
    C_EXTENSIONS NO
)
