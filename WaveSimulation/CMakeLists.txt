# This is the main CMakeLists.txt for the WaveSimulation executable

# Add source files for the main executable
file(GLOB_RECURSE APP_SOURCES
    "src/*.cpp"
    "src/*.h"
)

list(APPEND CMAKE_PREFIX_PATH ${CMAKE_SOURCE_DIR}/vcpkg_installed/x64-windows/debug/lib)

# Find external dependencies
find_package(DevIL REQUIRED) # Assuming DevIL is installed and findable
find_package(glm CONFIG REQUIRED) # Assuming GLM is installed and findable
find_package(glfw3 CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED) # Or find_package(GLAD REQUIRED) if using GLAD
find_package(imgui CONFIG REQUIRED) # Assuming ImGui is installed and findable

add_executable(WaveSimulation ${APP_SOURCES})

# Set C++ standard for this target
target_compile_features(WaveSimulation PRIVATE cxx_std_20)

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

# Add subdirectories for components, simulation, utils, and visualization
add_subdirectory(src/application)
add_subdirectory(src/simulation)
add_subdirectory(src/utils)
add_subdirectory(src/visualization)

# Include directories for headers
target_include_directories(WaveSimulation PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/src/application
    ${CMAKE_CURRENT_SOURCE_DIR}/src/simulation
    ${CMAKE_CURRENT_SOURCE_DIR}/src/utils
    ${CMAKE_CURRENT_SOURCE_DIR}/src/visualization
)

# Handle assets
# This might involve copying assets to the build directory or setting up resource paths
# For now, we'll just note it.
