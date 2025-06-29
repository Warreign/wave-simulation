# CMakeLists.txt for WaveSimulation/src/visualization/shaders

# Add source files for the visualization shaders library
file(GLOB VISUALIZATION_SHADERS_SOURCES
    "*.cpp"
    "*.h"
)

add_library(VisualizationShaders STATIC ${VISUALIZATION_SHADERS_SOURCES})

# Set C++ standard for this target
target_compile_features(VisualizationShaders PRIVATE cxx_std_20)

target_link_libraries(VisualizationShaders PRIVATE
    glm::glm-header-only
)

# Include directories for headers
target_include_directories(VisualizationShaders PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/WaveSimulation/src
)
