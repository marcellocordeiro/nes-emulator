project(common)

add_library(common STATIC 
  "src/file_utils.cpp"
  "src/system_utils.cpp"
)

target_include_directories(common PUBLIC "include")

set_target_properties(common
  PROPERTIES
    CXX_STANDARD 23
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
)

target_link_libraries(common PRIVATE project-warnings)
