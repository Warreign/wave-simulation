# CMakeLists.txt for WaveSimulation/src/simulation/shaders

# Add source files for the simulation shaders library
file(GLOB SIMULATION_SHADERS_SOURCES
    "*.cpp"
    "*.h"
)

add_library(SimulationShaders STATIC ${SIMULATION_SHADERS_SOURCES})

# Set C++ standard for this target
target_compile_features(SimulationShaders PRIVATE cxx_std_20)

target_link_libraries(SimulationShaders PRIVATE
    glm::glm-header-only
)

# Include directories for headers
target_include_directories(SimulationShaders PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/WaveSimulation/src
)
