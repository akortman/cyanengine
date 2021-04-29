cmake_minimum_required(VERSION 3.18)
project(cyan)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(./cyan/generated/)

set(CYAN_ENGINE_SRC ${CYAN_GENERATED_SRC} cyan/src/engine/ecs/ecs_common.hpp cyan/src/engine/ecs/ecs.hpp cyan/src/engine/ecs/single_component_registry.hpp cyan/src/engine/ecs/entity.hpp cyan/src/logging/assert.hpp cyan/src/engine/ecs/object_registry.hpp cyan/src/engine/ecs/component_map.hpp cyan/src/engine/ecs/component_map.cpp cyan/src/engine/ecs/ecs.cpp cyan/src/engine/ecs/ecs_common.cpp cyan/src/engine/ecs/ecs_global.hpp cyan/src/engine/ecs/ecs_global.cpp cyan/src/engine.hpp cyan/src/engine.cpp cyan/src/engine/script/chai_engine.hpp cyan/src/engine/script/chai_engine.cpp cyan/src/logging/logger.hpp cyan/src/logging/logger.cpp cyan/src/engine/script/ecs_script.hpp cyan/src/engine/script/core_stdlib.hpp cyan/src/engine/script/ecs_script.cpp cyan/src/logging/error.hpp cyan/src/engine/resource/resource_array.hpp cyan/src/engine/resource/loaders/resource_loader.hpp cyan/src/engine/garbage_collect_interface.hpp cyan/src/engine/resource/resource_manager.hpp cyan/src/engine/resource/loaders/script_loader.hpp cyan/src/engine/resource/resource.hpp cyan/src/engine/resource/loaders/all_resource_loaders.hpp cyan/src/util/string.hpp cyan/src/util/string.cpp cyan/src/engine/resource/module.hpp cyan/src/engine/resource/resource.cpp cyan/src/engine/script/generated_script.hpp cyan/src/engine/script/generated_script.cpp cyan/src/engine/resource/module.cpp cyan/src/engine/script/resource_script.hpp cyan/src/engine/script/resource_script.cpp cyan/src/io/renderer_interface.hpp cyan/src/io/backends/opengl/renderer.hpp cyan/src/io/backends/opengl/renderer.cpp cyan/src/io/backends/opengl/shader.hpp cyan/src/io/backends/opengl/shader.cpp)
set(CYAN_TEST_SRC cyan/test/test_main.cpp cyan/test/engine/ecs/test_ecs_common.cpp cyan/test/engine/ecs/test_single_component_registry.cpp cyan/test/engine/ecs/test_object_registry.cpp cyan/test/engine/ecs/test_component_map.cpp cyan/test/engine/ecs/test_ecs.cpp cyan/test/engine/script/test_chai_engine.cpp cyan/test/engine/script/test_ecs_script.cpp cyan/test/engine/resource/test_resource.cpp cyan/test/engine/resource/test_module.cpp cyan/test/engine/resource/test_resource_array.cpp cyan/test/engine/resource/test_resource_manager.cpp cyan/test/util/test_string.cpp cyan/test/engine/script/test_resource_script.cpp)

# Set up includes.
# All project files should be able to include something relative from the project directory itself.
# Additionally, dependencies are located in ./library, and this should also be accessible as an include location, as
# well as specific locations within ./library.
include_directories(${PROJECT_SOURCE_DIR})
set(LIBDIR "${PROJECT_SOURCE_DIR}/library/")
set(CMAKE_PREFIX_PATH "${LIBDIR};${CMAKE_PREFIX_PATH}")
include_directories(${LIBDIR})

# chaiscript
include_directories(${LIBDIR}/chaiscript/include)

# glfw
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(library/glfw)
include_directories(library/glfw/include)

# glew
add_subdirectory(library/glew/build/cmake)
include_directories(library/glew/include)

# Find required packages.
find_package(fmt REQUIRED)
find_package(OpenGL REQUIRED)

# Libraries shared between different targets stored in LIBS.
set(CYAN_LIBS
        fmt::fmt-header-only
        OpenGL::GL
        glfw
        glew_s)

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
