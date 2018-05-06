cmake_minimum_required(VERSION 3.5.1)
project(basin)

include_directories(src)

# Put source files in this glob
file(GLOB SOURCES
    src/main.cpp

    src/battery.hpp
    src/battery.cpp
    src/boulder.hpp
    src/boulder.cpp
    src/blockade.hpp
    src/blockade.cpp
    src/game.hpp
    src/game.cpp
    src/config.hpp
    src/info.hpp
    src/info.cpp
    src/player.hpp
    src/player.cpp
    src/enemy.hpp
    src/enemy.cpp
    src/collectable.hpp
    src/collectable.cpp
    src/sliding_door.hpp
    src/sliding_door.cpp
    src/switchboard.hpp
    src/switchboard.cpp
    src/jumper.hpp
    src/jumper.cpp
    src/torch.hpp
    src/torch.cpp

    src/e/entity.hpp
    src/e/entity.cpp
    src/e/group.hpp
    src/e/group.cpp
    src/e/renderer.hpp
    src/e/renderer.cpp
    src/e/scene.hpp
    src/e/scene.cpp

    src/e/camera.hpp
    src/e/camera.cpp
    src/e/core.hpp
    src/e/core.cpp
    src/e/resources.hpp
    src/e/resources.cpp
    src/e/sprite.hpp
    src/e/sprite.cpp
    src/e/point.hpp
    src/e/point.cpp
    src/e/csv.hpp
    src/e/csv.cpp
    src/e/input.hpp
    src/e/input.cpp
    src/e/tilemap.hpp
    src/e/tilemap.cpp
    src/e/tilelayer.hpp
    src/e/tilelayer.cpp
    src/e/tileset.hpp
    src/e/tileset.cpp
    src/e/collision.hpp
    src/e/collision.cpp

    src/e/text.hpp
    src/e/text.cpp)

add_executable(game ${SOURCES})

# Copy over assets
add_custom_command(TARGET game PRE_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory 
    "${CMAKE_SOURCE_DIR}/assets" "${CMAKE_BINARY_DIR}/assets"
    COMMENT "copying over assets"
	VERBATIM)

# Use C++11. We always use C++11.
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --std=c++11")

# Runs if building for a native platform (currently only Linux)
if (UNIX AND NOT APPLE AND NOT EMSCRIPTEN)
	set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

	find_package(SDL2 REQUIRED)
	find_package(SDL2_image REQUIRED)
	find_package(SDL2_ttf REQUIRED)

	include_directories(${SDL2_INCLUDE_DIRS}
						${SDL2_IMAGE_INCLUDE_DIR}
						${SDL2_TTF_INCLUDE_DIR})

	target_link_libraries(game ${SDL2_LIBRARY}
						 ${SDL2_IMAGE_LIBRARIES}
						 ${SDL2_TTF_LIBRARIES})
endif()

# Runs if building for Emscripten (web)
# Modified excerpt from https://discourse.libsdl.org/t/sdl2-desktop-bridge-emscripten-questions/22632/2
if(EMSCRIPTEN)
    # Make emscripten output a html page instead of just the javascript (for easier testing).
    set(CMAKE_EXECUTABLE_SUFFIX ".html")

    # Using SDL2 and SDL2_image
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -s USE_SDL=2 -s USE_SDL_IMAGE=2 -s USE_SDL_TTF=2 --std=c++11")

    # Get SDL2_image to use PNGs
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -s SDL2_IMAGE_FORMATS='[\"png\"]'")

    # Use O2 optimisation level (makes a .mem file)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -O2")

    # Use assets folder
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --preload-file assets")

    # Another option you can try if having trouble with loading images:
    # https://groups.google.com/forum/#!topic/emscripten-discuss/0qxtF7zyG0s
    #set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -s STB_IMAGE=1")

    # Might need to play around with this if emscripten gives errors saying your program is too big.
    #set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -s TOTAL_MEMORY=67108864")
endif()
