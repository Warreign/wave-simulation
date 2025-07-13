# CMakeLists.txt for WaveSimulation/src/visualization/objects

# Add source files for the objects library
file(GLOB OBJECTS_SOURCES
    "*.cpp"
    "*.h"
)

add_library(Objects STATIC ${OBJECTS_SOURCES})

# Set C++ standard for this target
target_compile_features(Objects PRIVATE cxx_std_20)

# Include directories for headers
target_include_directories(Objects
    PRIVATE
        ${CMAKE_SOURCE_DIR}/src
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

# target_link_directories(Objects
#     PRIVATE
#         $<$<CONFIG:Debug>:${CMAKE_BINARY_DIR}/vcpkg_installed/x64-windows/debug/lib>
#         $<$<CONFIG:Release>:${CMAKE_BINARY_DIR}/vcpkg_installed/x64-windows/lib>
# )

target_link_libraries(Objects 
    PUBLIC
        Meshes
        VisualizationShaders
        glad::glad
        ${DEVIL_LIBRARY}
)

get_target_property(LINK_DIRS Objects INTERFACE_LINK_DIRECTORIES)

message(STATUS "Link directories for Objects: ${LINK_DIRS}")
