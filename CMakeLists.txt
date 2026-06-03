# Kindly generated for me by the big Claude because theres no fucking way im doing ts.

cmake_minimum_required(VERSION 3.16)
project(OptiTone VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

add_executable(${PROJECT_NAME}
    main.cpp
)

find_package(PkgConfig QUIET)

if(PkgConfig_FOUND)
    pkg_check_modules(PORTAUDIO REQUIRED portaudio-2.0)

    target_include_directories(${PROJECT_NAME} PRIVATE ${PORTAUDIO_INCLUDE_DIRS})
    target_link_libraries(${PROJECT_NAME} PRIVATE ${PORTAUDIO_LIBRARIES})
    target_link_directories(${PROJECT_NAME} PRIVATE ${PORTAUDIO_LIBRARY_DIRS})
    target_compile_options(${PROJECT_NAME} PRIVATE ${PORTAUDIO_CFLAGS_OTHER})

    find_package(SDL2 REQUIRED)

    target_include_directories(${PROJECT_NAME} PRIVATE ${SDL2_INCLUDE_DIRS})
    target_link_libraries(${PROJECT_NAME} PRIVATE ${SDL2_LIBRARIES})
    target_link_directories(${PROJECT_NAME} PRIVATE ${SDL2_LIBRARY_DIRS})


else()
    # Fallback: search common Homebrew/macOS paths manually
    find_path(PORTAUDIO_INCLUDE_DIR
        NAMES portaudio.h
        PATHS
            /opt/homebrew/include       # Apple Silicon
            /usr/local/include          # Intel Mac
            /usr/include
    )

    find_library(PORTAUDIO_LIBRARY
        NAMES portaudio
        PATHS
            /opt/homebrew/lib
            /usr/local/lib
            /usr/lib
    )

    if(NOT PORTAUDIO_INCLUDE_DIR OR NOT PORTAUDIO_LIBRARY)
        message(FATAL_ERROR "PortAudio not found. Install it with: brew install portaudio")
    endif()

    target_include_directories(${PROJECT_NAME} PRIVATE ${PORTAUDIO_INCLUDE_DIR})
    target_link_libraries(${PROJECT_NAME} PRIVATE ${PORTAUDIO_LIBRARY})
endif()

# ── macOS frameworks (required by PortAudio on macOS) ─────────────────────────
if(APPLE)
    target_link_libraries(${PROJECT_NAME} PRIVATE
        "-framework CoreAudio"
        "-framework AudioToolbox"
        "-framework AudioUnit"
        "-framework CoreFoundation"
        "-framework CoreServices"
    )
endif()

# ── Optional: print paths for debugging ───────────────────────────────────────
message(STATUS "PortAudio include: ${PORTAUDIO_INCLUDE_DIRS}")
message(STATUS "PortAudio libs:    ${PORTAUDIO_LIBRARIES}")