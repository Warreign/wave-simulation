# CMakeLists.txt for WaveSimulation/src/visualization/meshes

# Add source files for the meshes library
file(GLOB MESHES_SOURCES
    "*.cpp"
    "*.h"
)

add_library(Meshes STATIC ${MESHES_SOURCES})

# Set C++ standard for this target
target_compile_features(Meshes PUBLIC cxx_std_20)

target_link_libraries(Meshes 
    PRIVATE
        glm::glm-header-only
        VisualizationShaders
)

# Include directories for headers
target_include_directories(Meshes 
    PRIVATE
        ${CMAKE_SOURCE_DIR}/src
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)
