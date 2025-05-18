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
  #src/todo/audio.cpp
  #src/todo/audio.hpp
  #src/todo/debugger.cpp
  #src/todo/debugger.hpp
  src/types/cpu_types.cpp
  src/types/cpu_types.hpp
  src/types/ppu/loopy_addr.cpp
  src/types/ppu/loopy_addr.hpp
  src/types/ppu/ppuctrl.cpp
  src/types/ppu/ppuctrl.hpp
  src/types/ppu/ppumask.cpp
  src/types/ppu/ppumask.hpp
  src/types/ppu/ppustatus.cpp
  src/types/ppu/ppustatus.hpp
  src/types/ppu_types.cpp
  src/types/ppu_types.hpp
  src/utility/file_manager.cpp
  src/utility/file_manager.hpp
  src/utility/ips_patch.cpp
  src/utility/ips_patch.hpp
  src/utility/snapshotable.hpp
)

set(HEADERS
  include/nes/constants.hpp
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

set_project_options(nes-core)
set_compiler_warnings(nes-core)

target_link_libraries(nes-core
  PRIVATE
    lib::common
    fmt::fmt
    spdlog::spdlog
)

install(
  TARGETS nes-core
  FILE_SET HEADERS
)
