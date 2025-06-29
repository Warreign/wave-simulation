# CMakeLists.txt for WaveSimulation/src/visualization/objects

# Add source files for the objects library
file(GLOB OBJECTS_SOURCES
    "*.cpp"
    "*.h"
)

add_library(Objects STATIC ${OBJECTS_SOURCES})

# Set C++ standard for this target
target_compile_features(Objects PRIVATE cxx_std_20)

target_link_libraries(Objects PRIVATE
    glad::glad
)

# Include directories for headers
target_include_directories(Objects PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/src
)
