function(add_spdlog)
  FetchContent_Declare(
    spdlog
    GIT_REPOSITORY "https://github.com/gabime/spdlog.git"
    GIT_TAG "v1.15.2"
    SYSTEM OVERRIDE_FIND_PACKAGE
  )

  FetchContent_MakeAvailable(spdlog)
endfunction()
