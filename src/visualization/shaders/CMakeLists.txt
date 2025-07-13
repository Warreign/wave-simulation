# CMakeLists.txt for WaveSimulation/src/visualization/shaders

# Add source files for the visualization shaders library
file(GLOB VISUALIZATION_SHADERS_SOURCES
    "*.cpp"
    "*.h"
)

add_library(VisualizationShaders STATIC ${VISUALIZATION_SHADERS_SOURCES})

# Set C++ standard for this target
target_compile_features(VisualizationShaders PUBLIC cxx_std_20)

target_link_libraries(VisualizationShaders 
    PRIVATE
        glm::glm-header-only
        Utils
)

# Include directories for headers
target_include_directories(VisualizationShaders 
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
    PRIVATE
        ${CMAKE_SOURCE_DIR}/src
)
