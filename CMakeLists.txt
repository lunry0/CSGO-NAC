cmake_minimum_required(VERSION 3.16)

set(PROJECT_NAME csgo_nac_dll)
project(${PROJECT_NAME} VERSION 1.0.0)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Set build type to Release by default
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# Compiler-specific options
if(MSVC)
    add_compile_options(/W4 /WX- /permissive-)
    add_compile_definitions(_CRT_SECURE_NO_WARNINGS)
    add_compile_definitions(NOMINMAX)
    add_compile_definitions(WIN32_LEAN_AND_MEAN)
    
    # Enable multi-processor compilation
    add_compile_options(/MP)
    
    # Optimization flags for Release
    if(CMAKE_BUILD_TYPE STREQUAL "Release")
        add_compile_options(/O2 /Ob2 /Ot /GL)
        add_link_options(/LTCG /OPT:REF /OPT:ICF)
    endif()
endif()

# Include directories
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)

# Find required packages
find_package(PkgConfig QUIET)

# Try to find DirectX SDK
if(WIN32)
    # Look for DirectX SDK
    find_path(DIRECTX_INCLUDE_DIR
        NAMES d3d11.h
        PATHS
            "$ENV{DXSDK_DIR}/Include"
            "$ENV{ProgramFiles}/Microsoft DirectX SDK*/Include"
            "$ENV{ProgramFiles(x86)}/Microsoft DirectX SDK*/Include"
            "C:/Program Files/Microsoft DirectX SDK*/Include"
            "C:/Program Files (x86)/Microsoft DirectX SDK*/Include"
    )
    
    find_library(DIRECTX_D3D11_LIBRARY
        NAMES d3d11
        PATHS
            "$ENV{DXSDK_DIR}/Lib/x64"
            "$ENV{ProgramFiles}/Microsoft DirectX SDK*/Lib/x64"
            "$ENV{ProgramFiles(x86)}/Microsoft DirectX SDK*/Lib/x64"
            "C:/Program Files/Microsoft DirectX SDK*/Lib/x64"
            "C:/Program Files (x86)/Microsoft DirectX SDK*/Lib/x64"
    )
    
    find_library(DIRECTX_DXGI_LIBRARY
        NAMES dxgi
        PATHS
            "$ENV{DXSDK_DIR}/Lib/x64"
            "$ENV{ProgramFiles}/Microsoft DirectX SDK*/Lib/x64"
            "$ENV{ProgramFiles(x86)}/Microsoft DirectX SDK*/Lib/x64"
            "C:/Program Files/Microsoft DirectX SDK*/Lib/x64"
            "C:/Program Files (x86)/Microsoft DirectX SDK*/Lib/x64"
    )
    
    find_library(DIRECTX_D3DCOMPILER_LIBRARY
        NAMES d3dcompiler
        PATHS
            "$ENV{DXSDK_DIR}/Lib/x64"
            "$ENV{ProgramFiles}/Microsoft DirectX SDK*/Lib/x64"
            "$ENV{ProgramFiles(x86)}/Microsoft DirectX SDK*/Lib/x64"
            "C:/Program Files/Microsoft DirectX SDK*/Lib/x64"
            "C:/Program Files (x86)/Microsoft DirectX SDK*/Lib/x64"
    )
endif()

# Source files
set(SOURCES
    # Core files
    src/dllmain.cpp
    src/config.cpp
    
    # GUI files
    src/gui/gui.cpp
    src/gui/animations.cpp
    
    # Feature files
    src/features/fps_boost.cpp
    src/features/low_details.cpp
    
    # Utility files
    src/utils/logger.cpp
)

# Header files
set(HEADERS
    include/csgo_sdk.h
    include/config.h
    src/gui/gui.h
    src/gui/animations.h
    src/features/fps_boost.h
    src/features/low_details.h
    src/utils/logger.h
    src/types/index.h
)

# Create the DLL
add_library(${PROJECT_NAME} SHARED ${SOURCES} ${HEADERS})

# Set DLL properties
set_target_properties(${PROJECT_NAME} PROPERTIES
    OUTPUT_NAME "csgo_nac"
    SUFFIX ".dll"
    PREFIX ""
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
)

# Preprocessor definitions
target_compile_definitions(${PROJECT_NAME} PRIVATE
    UNICODE
    _UNICODE
    IMGUI_IMPL_OPENGL_LOADER_GL3W
    IMGUI_DISABLE_OBSOLETE_FUNCTIONS
)

# System libraries
set(SYSTEM_LIBS
    kernel32
    user32
    gdi32
    winspool
    comdlg32
    advapi32
    shell32
    ole32
    oleaut32
    uuid
    odbc32
    odbccp32
    d3d11
    dxgi
    d3dcompiler
)

# Link libraries
target_link_libraries(${PROJECT_NAME} PRIVATE ${SYSTEM_LIBS})

# Add DirectX libraries if found
if(DIRECTX_D3D11_LIBRARY)
    target_link_libraries(${PROJECT_NAME} PRIVATE ${DIRECTX_D3D11_LIBRARY})
endif()

if(DIRECTX_DXGI_LIBRARY)
    target_link_libraries(${PROJECT_NAME} PRIVATE ${DIRECTX_DXGI_LIBRARY})
endif()

if(DIRECTX_D3DCOMPILER_LIBRARY)
    target_link_libraries(${PROJECT_NAME} PRIVATE ${DIRECTX_D3DCOMPILER_LIBRARY})
endif()

# Include DirectX headers if found
if(DIRECTX_INCLUDE_DIR)
    target_include_directories(${PROJECT_NAME} PRIVATE ${DIRECTX_INCLUDE_DIR})
endif()

# Try to find and link ImGui (if available)
find_path(IMGUI_INCLUDE_DIR
    NAMES imgui.h
    PATHS
        ${CMAKE_CURRENT_SOURCE_DIR}/third_party/imgui
        ${CMAKE_CURRENT_SOURCE_DIR}/external/imgui
        ${CMAKE_CURRENT_SOURCE_DIR}/libs/imgui
)

if(IMGUI_INCLUDE_DIR)
    message(STATUS "Found ImGui at: ${IMGUI_INCLUDE_DIR}")
    target_include_directories(${PROJECT_NAME} PRIVATE ${IMGUI_INCLUDE_DIR})
    
    # Add ImGui source files
    set(IMGUI_SOURCES
        ${IMGUI_INCLUDE_DIR}/imgui.cpp
        ${IMGUI_INCLUDE_DIR}/imgui_demo.cpp
        ${IMGUI_INCLUDE_DIR}/imgui_draw.cpp
        ${IMGUI_INCLUDE_DIR}/imgui_tables.cpp
        ${IMGUI_INCLUDE_DIR}/imgui_widgets.cpp
        ${IMGUI_INCLUDE_DIR}/backends/imgui_impl_dx11.cpp
        ${IMGUI_INCLUDE_DIR}/backends/imgui_impl_win32.cpp
    )
    
    target_sources(${PROJECT_NAME} PRIVATE ${IMGUI_SOURCES})
    target_include_directories(${PROJECT_NAME} PRIVATE ${IMGUI_INCLUDE_DIR}/backends)
else()
    message(WARNING "ImGui not found. GUI functionality may be limited.")
endif()

# Try to find and link JSON library (jsoncpp)
find_path(JSON_INCLUDE_DIR
    NAMES json/json.h
    PATHS
        ${CMAKE_CURRENT_SOURCE_DIR}/third_party/jsoncpp/include
        ${CMAKE_CURRENT_SOURCE_DIR}/external/jsoncpp/include
        ${CMAKE_CURRENT_SOURCE_DIR}/libs/jsoncpp/include
        /usr/include
        /usr/local/include
)

if(JSON_INCLUDE_DIR)
    message(STATUS "Found JSON library at: ${JSON_INCLUDE_DIR}")
    target_include_directories(${PROJECT_NAME} PRIVATE ${JSON_INCLUDE_DIR})
    
    # Try to find the library file
    find_library(JSON_LIBRARY
        NAMES jsoncpp json_cpp
        PATHS
            ${CMAKE_CURRENT_SOURCE_DIR}/third_party/jsoncpp/lib
            ${CMAKE_CURRENT_SOURCE_DIR}/external/jsoncpp/lib
            ${CMAKE_CURRENT_SOURCE_DIR}/libs/jsoncpp/lib
            /usr/lib
            /usr/local/lib
    )
    
    if(JSON_LIBRARY)
        target_link_libraries(${PROJECT_NAME} PRIVATE ${JSON_LIBRARY})
    endif()
else()
    message(WARNING "JSON library not found. Configuration system may be limited.")
endif()

# Custom target for copying DLL to output directory
add_custom_target(copy_dll ALL
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:${PROJECT_NAME}>
        ${CMAKE_CURRENT_SOURCE_DIR}/output/
    DEPENDS ${PROJECT_NAME}
    COMMENT "Copying DLL to output directory"
)

# Create output directory
file(MAKE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/output)

# Installation
install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)

# Install configuration files
install(FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/README.md
    DESTINATION .
)

# Print configuration summary
message(STATUS "=== CS:GO NAC Build Configuration ===")
message(STATUS "Project Name: ${PROJECT_NAME}")
message(STATUS "Version: ${PROJECT_VERSION}")
message(STATUS "Build Type: ${CMAKE_BUILD_TYPE}")
message(STATUS "C++ Standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "Compiler: ${CMAKE_CXX_COMPILER_ID}")
message(STATUS "DirectX Include: ${DIRECTX_INCLUDE_DIR}")
message(STATUS "ImGui Include: ${IMGUI_INCLUDE_DIR}")
message(STATUS "JSON Include: ${JSON_INCLUDE_DIR}")
message(STATUS "Output Directory: ${CMAKE_BINARY_DIR}/bin")
message(STATUS "=====================================")

# Add compile-time definitions for version info
target_compile_definitions(${PROJECT_NAME} PRIVATE
    NAC_VERSION_MAJOR=${PROJECT_VERSION_MAJOR}
    NAC_VERSION_MINOR=${PROJECT_VERSION_MINOR}
    NAC_VERSION_PATCH=${PROJECT_VERSION_PATCH}
    NAC_VERSION_STRING="${PROJECT_VERSION}"
)