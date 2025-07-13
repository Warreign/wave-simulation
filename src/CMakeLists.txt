# This is the main CMakeLists.txt for the WaveSimulation executable

# Add source files for the main executable
file(GLOB_RECURSE APP_SOURCES
    "*.cpp"
    "*.h"
)

list(APPEND CMAKE_PREFIX_PATH ${CMAKE_SOURCE_DIR}/vcpkg_installed/x64-windows/)

# Find external dependencies
find_package(DevIL REQUIRED) # Assuming DevIL is installed and findable
find_library(DEVIL_LIBRARY NAMES devil)

find_package(glm CONFIG REQUIRED) # Assuming GLM is installed and findable
find_package(glfw3 CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED) # Or find_package(GLAD REQUIRED) if using GLAD
find_package(imgui CONFIG REQUIRED) # Assuming ImGui is installed and findable

add_executable(WaveSimulation ${APP_SOURCES})

# Set C++ standard for this target
target_compile_features(WaveSimulation PRIVATE cxx_std_20)

# Add subdirectories for components, simulation, utils, and visualization
add_subdirectory(application)
add_subdirectory(simulation)
add_subdirectory(utils)
add_subdirectory(visualization)

# Link necessary libraries
target_link_libraries(WaveSimulation PUBLIC
    Application
    Components
    Simulation
    SimulationShaders
    Utils
    Visualization
    Meshes
    Objects
    VisualizationShaders
    glfw
    glad::glad
    glm::glm-header-only
    imgui::imgui # Link ImGui to the main executable
)

# Include directories for headers
target_include_directories(WaveSimulation PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/application
    ${CMAKE_CURRENT_SOURCE_DIR}/simulation
    ${CMAKE_CURRENT_SOURCE_DIR}/utils
    ${CMAKE_CURRENT_SOURCE_DIR}/visualization
)

# Handle assets
# This might involve copying assets to the build directory or setting up resource paths
# For now, we'll just note it.
