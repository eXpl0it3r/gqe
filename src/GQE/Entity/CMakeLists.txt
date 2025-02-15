# Entity library public header include file
set(Entity_HEADER ${INCROOT}/Entity.hpp)

# Entity library additional include files
set(INC
    ${INCROOT}/Entity/systems/ActionSystem.hpp
    ${INCROOT}/Entity/systems/AnimationSystem.hpp
    ${INCROOT}/Entity/systems/TransformSystem.hpp
    ${INCROOT}/Entity/systems/RenderSystem.hpp
	${INCROOT}/Entity/systems/TimerSystem.hpp
    ${INCROOT}/Entity/classes/ActionGroup.hpp
    ${INCROOT}/Entity/classes/PrototypeManager.hpp
    ${INCROOT}/Entity/classes/Instance.hpp
    ${INCROOT}/Entity/classes/Prototype.hpp
  	${INCROOT}/Entity/classes/EntityEvents.hpp
  	${INCROOT}/Entity/classes/KeyBinder.hpp
  	${INCROOT}/Entity/classes/MouseBinder.hpp
  	${INCROOT}/Entity/classes/JoystickBinder.hpp
    ${INCROOT}/Entity/interfaces/IAction.hpp
    ${INCROOT}/Entity/interfaces/IEntity.hpp
    ${INCROOT}/Entity/interfaces/ISystem.hpp
    ${INCROOT}/Entity/interfaces/ICollisionSystem.hpp
    ${INCROOT}/Entity/Entity_types.hpp

)

# Entity library source files
set(SRC
    ${SRCROOT}/Entity/systems/ActionSystem.cpp
    ${SRCROOT}/Entity/systems/AnimationSystem.cpp
    ${SRCROOT}/Entity/systems/TransformSystem.cpp
    ${SRCROOT}/Entity/systems/RenderSystem.cpp
	${SRCROOT}/Entity/systems/TimerSystem.cpp
    ${SRCROOT}/Entity/classes/ActionGroup.cpp
    ${SRCROOT}/Entity/classes/PrototypeManager.cpp
    ${SRCROOT}/Entity/classes/Instance.cpp
    ${SRCROOT}/Entity/classes/Prototype.cpp
    ${SRCROOT}/Entity/classes/EntityEvents.cpp
    ${SRCROOT}/Entity/classes/KeyBinder.cpp
  	${SRCROOT}/Entity/classes/MouseBinder.cpp
  	${SRCROOT}/Entity/classes/JoystickBinder.cpp
    ${SRCROOT}/Entity/interfaces/IAction.cpp
    ${SRCROOT}/Entity/interfaces/IEntity.cpp
    ${SRCROOT}/Entity/interfaces/ISystem.cpp
    ${SRCROOT}/Entity/interfaces/ICollisionSystem.cpp
    ${SRCROOT}/Entity/Entity_types.cpp
)

# add platform specific sources

# find external SFML libraries
find_package(SFML REQUIRED graphics window system)

if(BUILD_SHARED_LIBS)
  # define the gqe-Entity target
  gqe_add_library(gqe-entity
                  HEADER        ${Entity_HEADER}
                  HEADER_DIR    ${INCROOT}/Entity
                  INCLUDES      ${INC}
                  SOURCES       ${SRC}
                  DEPENDS       gqe-core
                  EXTERNAL_LIBS sfml-graphics)
else()
  # define the gqe-Entity target (for a static build, we use depends to remove LNK4006 and LNK4221 errors with Visual Studio)
  gqe_add_library(gqe-entity
                  HEADER        ${Entity_HEADER}
                  HEADER_DIR    ${INCROOT}/Entity
                  INCLUDES      ${INC}
                  SOURCES       ${SRC}
                  DEPENDS       gqe-core
                  DEPENDS       sfml-graphics)
endif()
