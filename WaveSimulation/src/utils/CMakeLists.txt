# CMakeLists.txt for WaveSimulation/src/utils

# Add source files for the utils library
file(GLOB UTILS_SOURCES
    "*.cpp"
    "*.h"
)

add_library(Utils STATIC ${UTILS_SOURCES})

# Set C++ standard for this target
target_compile_features(Utils PUBLIC cxx_std_20)

target_link_libraries(Utils PUBLIC
    glm::glm-header-only
)

# Include directories for headers
target_include_directories(Utils PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/WaveSimulation/src
)
