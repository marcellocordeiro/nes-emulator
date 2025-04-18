function(add_fmt)
    if (LINUX)
        return()
    endif()

    FetchContent_Declare(
        fmt
        GIT_REPOSITORY "https://github.com/fmtlib/fmt.git"
        GIT_TAG        "11.1.4"
        SYSTEM
        OVERRIDE_FIND_PACKAGE
    )

    FetchContent_MakeAvailable(fmt)
endfunction()
