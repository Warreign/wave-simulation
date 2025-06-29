# CMakeLists.txt for WaveSimulation/src/visualization

# Add source files for the visualization library
file(GLOB VISUALIZATION_SOURCES
    "*.cpp"
    "*.h"
)

add_library(Visualization STATIC ${VISUALIZATION_SOURCES})

# Set C++ standard for this target
target_compile_features(Visualization PRIVATE cxx_std_20)

# Include directories for headers
target_include_directories(Visualization PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/src
)

# Add subdirectories
add_subdirectory(meshes)
add_subdirectory(objects)
add_subdirectory(shaders)

# Link the sub-libraries to the visualization library
target_link_libraries(Visualization PRIVATE Meshes Objects VisualizationShaders glm::glm-header-only)
