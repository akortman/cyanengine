cmake_minimum_required(VERSION 3.18)
project(cyan)

set(CMAKE_CXX_STANDARD 17)

set(CYAN_ENGINE_SRC cyan/src/engine/ecs/ecs_common.hpp cyan/src/engine/ecs/ecs.hpp cyan/src/engine/ecs/single_component_registry.hpp cyan/src/engine/ecs/entity.hpp cyan/src/logging/assert.hpp cyan/src/engine/ecs/object_registry.hpp cyan/src/engine/ecs/component_map.hpp cyan/src/engine/ecs/component_map.cpp cyan/src/engine/ecs/ecs.cpp cyan/src/engine/ecs/ecs_common.cpp cyan/src/engine/ecs/ecs_global.hpp cyan/src/engine/ecs/ecs_global.cpp cyan/src/engine/engine.hpp cyan/src/engine/engine.cpp cyan/src/engine/script/chai_engine.hpp cyan/src/engine/script/chai_engine.cpp cyan/src/logging/logger.hpp cyan/src/logging/logger.cpp)
set(CYAN_TEST_SRC cyan/test/test_main.cpp cyan/test/engine/ecs/test_ecs_common.cpp cyan/test/engine/ecs/test_single_component_registry.cpp cyan/test/engine/ecs/test_object_registry.cpp cyan/test/engine/ecs/test_component_map.cpp cyan/test/engine/ecs/test_ecs.cpp)

# Set up includes.
# All project files should be able to include something relative from the project directory itself.
# Additionally, dependencies are located in ./library, and this should also be accessible as an include location, as
# well as specific locations within ./library.
include_directories(${PROJECT_SOURCE_DIR})
set(LIBDIR "${PROJECT_SOURCE_DIR}/library/")
set(CMAKE_PREFIX_PATH "${LIBDIR};${CMAKE_PREFIX_PATH}")
include_directories(${LIBDIR})
include_directories(${LIBDIR}/chaiscript/include)

# Find required packages.
find_package(fmt REQUIRED)

# Libraries shared between different targets stored in LIBS.
set(CYAN_LIBS
        fmt::fmt-header-only)

# The engine core is compiled as library that can be linked into game (or utility) applications.
add_library(cyan ${CYAN_ENGINE_SRC})
target_link_libraries(cyan ${CYAN_LIBS})

# Tests are run in an executable that is linked to the cyan engine library
add_executable(cyan_test ${CYAN_TEST_SRC})
target_link_libraries(cyan_test cyan)

## APPS
# The engine comes packaged with applications that use the cyan engine library.

# The cyan_loader app is a simple main function that creates and runs an Engine object.
# For games written entirely in the scripting system, this should be sufficient to run the engine.
set(CYAN_APP_LOADER_SRC apps/cyan_loader/src/main.cpp)
add_executable(cyan_app_loader ${CYAN_APP_LOADER_SRC})
target_link_libraries(cyan_app_loader cyan)

# In the future, a development application for script writing and organization may be written.
# TODO
