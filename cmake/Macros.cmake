
# Name: set_option
# Description: Set the CMAKE option to default if not found or to the value
# specified.
# Usage: set_option(var default type docstring)
# Example: set_option(TARGET_HOST_TYPE "x86" String "x64")
macro(set_option var default type docstring)
  if(NOT DEFINED ${var})
    set(${var} ${default})
  endif()
  set(${var} ${${var}} CACHE ${type} ${docstring} FORCE)
endmacro(set_option)

# check if a value is contained in a list
# sets ${var} to TRUE if the value is found
macro(gqe_list_contains var value)
  set(${var})
  foreach(value2 ${ARGN})
    if(${value} STREQUAL ${value2})
      set(${var} TRUE)
    endif()
  endforeach()
endmacro()

# parse a list of arguments and options
# ex: gqe_parse_arguments(THIS "SOURCES;DEPENDS" "FLAG" FLAG SOURCES s1 s2 s3 DEPENDS d1 d2)
# will define the following variables:
# - THIS_SOURCES (s1 s2 s3)
# - THIS_DEPENDS (d1 d2)
# - THIS_FLAG TRUE
macro(gqe_parse_arguments prefix arg_names option_names)
  foreach(arg_name ${arg_names})
    set(${prefix}_${arg_name})
  endforeach()
  foreach(option_name ${option_names})
    set(${prefix}_${option_name} FALSE)
  endforeach()
  set(current_arg_name)
  set(current_arg_list)
  foreach(arg ${ARGN})
    gqe_list_contains(is_arg_name ${arg} ${arg_names})
    if(is_arg_name)
      set(${prefix}_${current_arg_name} ${current_arg_list})
      set(current_arg_name ${arg})
      set(current_arg_list)
    else()
      gqe_list_contains(is_option ${arg} ${option_names})
      if(is_option)
        set(${prefix}_${arg} TRUE)
      else()
        set(current_arg_list ${current_arg_list} ${arg})
      endif()
    endif()
  endforeach()
  set(${prefix}_${current_arg_name} ${current_arg_list})
endmacro()

# add a new target which is a GQE library
# ex: gqe_add_library(gqe-core
#                     HEADER        include/GQE/Core.hpp
#                     HEADER_DIR    include/GQE/Core
#                     SOURCES       include/GQE/Core/classes/App.hpp src/GQE/Core/classes/App.cpp ...
#                     DEPENDS       ...
#                     EXTERNAL_LIBS sfml-audio sfml-graphics sfml-window sfml-system ...)
macro(gqe_add_library target)
  # parse the arguments
  gqe_parse_arguments(THIS "HEADER;HEADER_DIR;INCLUDES;SOURCES;DEPENDS;EXTERNAL_LIBS" "" ${ARGN})

  # create the target
  add_library(${target} ${THIS_HEADER} ${THIS_INCLUDES} ${THIS_SOURCES})

  # adjust the output file prefix/suffix to match our conventions
  if(BUILD_SHARED_LIBS)
    if(WINDOWS)
      # include the major version number in Windows shared library names (but not import library names)
      set_target_properties(${target} PROPERTIES DEBUG_POSTFIX -d)
      set_target_properties(${target} PROPERTIES SUFFIX "-${GQE_VERSION_MAJOR}_${GQE_VERSION_MINOR}${CMAKE_SHARED_LIBRARY_SUFFIX}")
    else()
      set_target_properties(${target} PROPERTIES DEBUG_POSTFIX -d)
    endif()
    if (WINDOWS AND COMPILER_GCC)
      # on Windows/gcc get rid of "lib" prefix for shared libraries,
      # and transform the ".dll.a" suffix into ".a" for import libraries
      set_target_properties(${target} PROPERTIES PREFIX "")
      set_target_properties(${target} PROPERTIES IMPORT_SUFFIX ".a")
    endif()
  else()
    set_target_properties(${target} PROPERTIES DEBUG_POSTFIX -s-d)
    set_target_properties(${target} PROPERTIES RELEASE_POSTFIX -s)
  endif()

  # set target public header file for this library
  if(THIS_HEADER)
    set_target_properties(${target} PROPERTIES PUBLIC_HEADER "${THIS_HEADER}")
  endif(THIS_HEADER)

  # set the version and soversion of the target (for compatible systems -- mostly Linuxes)
  set_target_properties(${target} PROPERTIES SOVERSION ${GQE_VERSION_MAJOR}.${GQE_VERSION_MINOR})
  set_target_properties(${target} PROPERTIES VERSION ${GQE_VERSION_MAJOR}.${GQE_VERSION_MINOR}.${GQE_VERSION_PATCH})

  # for gcc 4.x on Windows, apply the BUILD_STATIC_STD_LIBS option if it is enabled
  if(WINDOWS AND BUILD_STATIC_STD_LIBS)
    if(COMPILER_GCC AND "${GCC_VERSION}" MATCHES "4\\..*")
      set_target_properties(${target} PROPERTIES LINK_FLAGS "-static-libgcc -static-libstdc++")
    elseif(COMPILER_MSVC)
      set_target_properties(${target} PROPERTIES MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
    endif()
  endif()

  # link the target to its GQE dependencies
  if(THIS_DEPENDS)
    target_link_libraries(${target} ${THIS_DEPENDS})
  endif()

  # link the target to its external dependencies
  if(THIS_EXTERNAL_LIBS)
    target_link_libraries(${target} ${THIS_EXTERNAL_LIBS})
  endif(THIS_EXTERNAL_LIBS)

  # add the install rule
  install(TARGETS ${target}
          # IMPORTANT: Add the target library to the "export-set"
          EXPORT GQE_LibraryDepends
          PUBLIC_HEADER DESTINATION include/GQE COMPONENT devel
          RUNTIME DESTINATION bin COMPONENT bin
          LIBRARY DESTINATION lib COMPONENT shlib
          ARCHIVE DESTINATION lib COMPONENT devel)

  # install Core library include files
  if(THIS_HEADER_DIR)
    install(DIRECTORY ${THIS_HEADER_DIR}
            DESTINATION include/GQE
            COMPONENT devel
            PATTERN ".hg" EXCLUDE)
  endif(THIS_HEADER_DIR)
endmacro()

# add a new target which is a GQE example
# ex: gqe_add_example(demo
#                     SOURCES main.cpp ...
#                     DEPENDS gqe-core gqe-standalone)
macro(gqe_add_example target)

    # parse the arguments
    gqe_parse_arguments(THIS "SOURCES;DEPENDS" "GUI_APP" ${ARGN})

    # create the target
    if(THIS_GUI_APP AND WINDOWS)
        add_executable(${target} WIN32 ${THIS_SOURCES})
        target_link_libraries(${target} ${SFML_MAIN_LIBRARY})
    else()
        add_executable(${target} ${THIS_SOURCES})
    endif()

    # set the debug suffix
    set_target_properties(${target} PROPERTIES DEBUG_POSTFIX -d)

    # for gcc 4.x on Windows, apply the BUILD_STATIC_STD_LIBS option if it is enabled
    if(WINDOWS AND BUILD_STATIC_STD_LIBS)
        if(COMPILER_GCC AND "${GCC_VERSION}" MATCHES "4\\..*")
            set_target_properties(${target} PROPERTIES LINK_FLAGS "-static-libgcc -static-libstdc++")
        elseif(COMPILER_MSVC)
          set_target_properties(${target} PROPERTIES MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
        endif()
    endif()

    # link the target to its GQE dependencies
    if(THIS_DEPENDS)
        target_link_libraries(${target} ${THIS_DEPENDS})
    endif()

    # always copy dll files over to build directory after build
    if(WINDOWS)
      if(ARCH_32BITS)
        if(EXISTS ${SFML_INCLUDE_DIR}/../bin/x86/openal32.dll)
          add_custom_command(TARGET ${target}
                              POST_BUILD
                              COMMAND ${CMAKE_COMMAND} -E copy
                              ${SFML_INCLUDE_DIR}/../bin/x86/openal32.dll
                              ${PROJECT_BINARY_DIR}/${CMAKE_CFG_INTDIR})
        endif()
      else()
        if(EXISTS ${SFML_INCLUDE_DIR}/../bin/x64/openal32.dll)
          add_custom_command(TARGET ${target}
                              POST_BUILD
                              COMMAND ${CMAKE_COMMAND} -E copy
                              ${SFML_INCLUDE_DIR}/../bin/x64/openal32.dll
                              ${PROJECT_BINARY_DIR}/${CMAKE_CFG_INTDIR})
        endif()
      endif()
    endif()

    # always copy resources over to build directory after build
    add_custom_command(TARGET ${target}
                       POST_BUILD
                       COMMAND ${CMAKE_COMMAND} -E copy_directory
                         ${PROJECT_SOURCE_DIR}/resources
                         ${PROJECT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/resources)

    # add the install rule
    install(TARGETS ${target}
            RUNTIME DESTINATION examples/${target} COMPONENT examples)

    # install the example's resources as well
    set(EXAMPLE_RESOURCES "${PROJECT_SOURCE_DIR}/resources")
    if(EXISTS ${EXAMPLE_RESOURCES})
        install(DIRECTORY ${EXAMPLE_RESOURCES}
                DESTINATION examples/${target}
                COMPONENT examples
                PATTERN ".hg" EXCLUDE)
    endif()
endmacro()
