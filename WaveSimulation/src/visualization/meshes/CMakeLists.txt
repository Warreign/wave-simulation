# CMakeLists.txt for WaveSimulation/src/visualization/meshes

# Add source files for the meshes library
file(GLOB MESHES_SOURCES
    "*.cpp"
    "*.h"
)

add_library(Meshes STATIC ${MESHES_SOURCES})

# Set C++ standard for this target
target_compile_features(Meshes PRIVATE cxx_std_20)

target_link_libraries(Meshes PRIVATE
    glm::glm-header-only
)

# Include directories for headers
target_include_directories(Meshes PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/WaveSimulation/src
)
