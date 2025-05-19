function(enable_ipo)
  include(CheckIPOSupported)
  check_ipo_supported(RESULT result OUTPUT output)

  if(NOT result)
    message(SEND_ERROR "IPO is not supported: ${output}")
  endif()

  set(CMAKE_INTERPROCEDURAL_OPTIMIZATION ON PARENT_SCOPE)
endfunction()
