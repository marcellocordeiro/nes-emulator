set(SOURCES
  src/base_mapper.cpp
  src/base_mapper.hpp
  src/cartridge.cpp
  src/cartridge.hpp
  src/controller.cpp
  src/controller.hpp
  src/cpu.cpp
  src/cpu.hpp
  src/cpu_instructions.cpp
  src/mappers/mapper_0.cpp
  src/mappers/mapper_0.hpp
  src/mappers/mapper_1.cpp
  src/mappers/mapper_1.hpp
  src/mappers/mapper_2.cpp
  src/mappers/mapper_2.hpp
  src/mappers/mapper_4.cpp
  src/mappers/mapper_4.hpp
  src/mappers/mapper_7.cpp
  src/mappers/mapper_7.hpp
  src/nes.cpp
  src/ppu.cpp
  src/ppu.hpp
  src/todo/audio.cpp
  src/todo/audio.hpp
  src/todo/debugger.cpp
  src/todo/debugger.hpp
  src/types/cpu_types.cpp
  src/types/cpu_types.hpp
  src/types/ppu_types.cpp
  src/types/ppu_types.hpp
  src/utility/file_manager.cpp
  src/utility/file_manager.hpp
  src/utility/ips_patch.cpp
  src/utility/ips_patch.hpp
  src/utility/snapshotable.hpp
)

set(HEADERS
  include/nes/nes.hpp
)

add_library(nes-core STATIC ${SOURCES})
add_library(nes::core ALIAS nes-core)

target_sources(nes-core
  PUBLIC
    FILE_SET HEADERS
    BASE_DIRS include
    FILES ${HEADERS}
)

set_target_properties(nes-core
  PROPERTIES
    CXX_STANDARD 23
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
)

target_link_libraries(nes-core
  PRIVATE
    global::warnings
    lib::common
    fmt::fmt
    spdlog::spdlog
)

install(
  TARGETS nes-core
  FILE_SET HEADERS
)
