project(project-lib)

add_library(project-lib STATIC 
  "src/system_utils.cpp"
)

target_include_directories(project-lib SYSTEM PUBLIC "include")

set_target_properties(project-lib
  PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
)

target_link_libraries(project-lib PRIVATE project-warnings)

add_subdirectory("external/glad")
