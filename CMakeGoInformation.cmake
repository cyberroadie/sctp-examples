if(CMAKE_USER_MAKE_RULES_OVERRIDE)
  # Save the full path of the file so try_compile can use it.
  include(${CMAKE_USER_MAKE_RULES_OVERRIDE} RESULT_VARIABLE _override)
  set(CMAKE_USER_MAKE_RULES_OVERRIDE "${_override}")
endif()

if(CMAKE_USER_MAKE_RULES_OVERRIDE_Go)
  # Save the full path of the file so try_compile can use it.
   include(${CMAKE_USER_MAKE_RULES_OVERRIDE_Go} RESULT_VARIABLE _override)
   set(CMAKE_USER_MAKE_RULES_OVERRIDE_Go "${_override}")
endif()

if(NOT CMAKE_Go_COMPILE_OBJECT)
  set(CMAKE_Go_COMPILE_OBJECT "go tool compile -l -N -o <OBJECT> <SOURCE> ")
endif()
if(NOT CMAKE_Go_LINK_EXECUTABLE)
  set(CMAKE_Go_LINK_EXECUTABLE "go tool link -o <TARGET> <OBJECTS>  ")
endif()
