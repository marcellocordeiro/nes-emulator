function(add_sdl3)
  FetchContent_Declare(
    SDL3
    GIT_REPOSITORY "https://github.com/libsdl-org/SDL.git"
    GIT_TAG "release-3.2.10"
    SYSTEM OVERRIDE_FIND_PACKAGE
  )

  FetchContent_MakeAvailable(SDL3)
endfunction()
