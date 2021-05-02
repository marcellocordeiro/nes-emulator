project(glad)

add_library(glad STATIC "src/gl.c")

target_include_directories(glad SYSTEM PUBLIC "include")

if(UNIX)
  target_link_libraries(glad PRIVATE dl)
endif()
