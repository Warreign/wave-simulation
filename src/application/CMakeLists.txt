# CMakeLists.txt for WaveSimulation/src/application

# Add source files for the application library
file(GLOB APPLICATION_SOURCES
    "*.cpp"
    "*.h"
)

add_library(Application STATIC ${APPLICATION_SOURCES})

# Set C++ standard for this target
target_compile_features(Application PRIVATE cxx_std_20)

# Include directories for headers
target_include_directories(Application PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/src
)

# Add subdirectory for components
add_subdirectory(components)

target_link_libraries(Application 
    PUBLIC
        Components
        glad::glad
)
