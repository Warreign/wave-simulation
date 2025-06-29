# CMakeLists.txt for WaveSimulation/src/simulation

# Add source files for the simulation library
file(GLOB SIMULATION_SOURCES
    "*.cpp"
    "*.h"
)

add_library(Simulation STATIC ${SIMULATION_SOURCES})

# Set C++ standard for this target
target_compile_features(Simulation PRIVATE cxx_std_20)

# Include directories for headers
target_include_directories(Simulation PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/WaveSimulation/src
)

# Add subdirectory for shaders
add_subdirectory(shaders)

# Link the shaders library to the simulation library
target_link_libraries(Simulation PRIVATE SimulationShaders glm::glm-header-only)
