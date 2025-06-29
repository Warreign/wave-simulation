# CMakeLists.txt for WaveSimulation/src/application/components

# Add source files for the components library
file(GLOB COMPONENTS_SOURCES
    "*.cpp"
    "*.h"
)

add_library(Components STATIC ${COMPONENTS_SOURCES})

# Set C++ standard for this target
target_compile_features(Components PRIVATE cxx_std_20)

# Include directories for headers
target_include_directories(Components PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${ImGui_INCLUDE_DIRS} # Add ImGui include directories
    ${CMAKE_SOURCE_DIR}/WaveSimulation/src
)

# Link ImGui to the Components library
target_link_libraries(Components PRIVATE imgui::imgui)
