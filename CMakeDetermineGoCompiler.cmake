if(NOT CMAKE_Go_COMPILER)
  set(Go_BIN_PATH
    $ENV{GOPATH}
    $ENV{GOROOT}
    $ENV{GOROOT}/../bin
    /usr/bin
    /usr/local/bin
    )
    find_program(CMAKE_Go_COMPILER
      NAMES go
      PATHS ${Go_BIN_PATH}
    )
endif()

mark_as_advanced(CMAKE_Go_COMPILER)

configure_file(${CMAKE_CURRENT_LIST_DIR}/CMakeGoCompiler.cmake.in ${CMAKE_PLATFORM_INFO_DIR}/CMakeGoCompiler.cmake @ONLY)

set(CMAKE_Go_COMPILER_ENV_VAR "GOCOMPILER")
