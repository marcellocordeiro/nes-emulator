function(set_project_options target)
  set_target_properties(${target}
    PROPERTIES
      CXX_STANDARD 23
      CXX_STANDARD_REQUIRED YES
      CXX_EXTENSIONS NO
  )
endfunction()
