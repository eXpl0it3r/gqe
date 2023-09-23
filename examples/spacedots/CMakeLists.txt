project(SpaceDots)

set(SRCROOT ${PROJECT_SOURCE_DIR}/src)

# all source files
set(SRC
 ${SRCROOT}/main.cpp
 ${SRCROOT}/SpaceDotsApp.hpp
 ${SRCROOT}/SpaceDotsApp.cpp
 ${SRCROOT}/GameState.hpp
 ${SRCROOT}/GameState.cpp)

# find external SFML libraries
if(WINDOWS)
  find_package(SFML REQUIRED main)
  set(SFML_WINDOWS_DEPENDENCY sfml-main)
endif(WINDOWS)

# define the demo target
gqe_add_example(spacedots GUI_APP
                SOURCES ${SRC}
                DEPENDS gqe-core ${SFML_WINDOWS_DEPENDENCY})
