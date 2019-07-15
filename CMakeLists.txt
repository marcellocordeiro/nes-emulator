cmake_minimum_required(VERSION 3.14)

project(nes-emulator VERSION 1.0.0)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_HOME_DIRECTORY}/bin)

######################
#    Dependencies    #
######################

find_package(SDL2 2.0 REQUIRED)
# find_package(fmt 5.3 REQUIRED)

add_subdirectory("lib")

######################
#    Source files    #
######################

include_directories("include")

file(GLOB SOURCE_FILES
    "src/*.cpp"
    "src/mappers/*.cpp"
    "src/types/*.cpp"
)

######################
#     Executable     #
######################

add_executable(nes-emulator ${SOURCE_FILES})

######################
#   Compiler flags   #
######################

set(GNU_FLAGS
    -Werror
    -Wall
    -Wextra
    -pedantic
    # -Wuseless-cast # Doesn't work on Clang?
    -Wshadow
    -Wformat=2
    -Wdisabled-optimization
)

set(MSVC_FLAGS
    /WX
    /W4
    /permissive-
    /diagnostics:caret
)

set_target_properties(
    nes-emulator
    PROPERTIES CXX_STANDARD 17
               CXX_STANDARD_REQUIRED YES
               CXX_EXTENSIONS NO
)

if(UNIX)
    target_compile_options(nes-emulator PRIVATE ${GNU_FLAGS})
elseif(MSVC)
    target_compile_options(nes-emulator PRIVATE ${MSVC_FLAGS})
    target_compile_definitions(nes-emulator PRIVATE SDL_MAIN_HANDLED)

    # Remove for CMake 3.15 (CMP0092)
    string(REGEX REPLACE "/W3" "/W4" CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS})
endif()

##################
# Link libraries #
##################

target_link_libraries(nes-emulator PRIVATE nes-emulator-lib)
target_link_libraries(nes-emulator PRIVATE SDL2::SDL2)

configure_file("res/nes_mesen.pal" ${CMAKE_RUNTIME_OUTPUT_DIRECTORY} COPYONLY)
