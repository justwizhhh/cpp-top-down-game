cmake_minimum_required(VERSION 3.16)
project(PlatformerSFML)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

#---- Fetch SFML, only works if you have git installed on your machine ----#

include(FetchContent)   # Allows fetching content remotely at configure time
FetchContent_Declare(   # Fetches SFML from github and puts it in the build directory
    SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 69ea0cd863aed1d4092b970b676924a716ff718b    # Makes sure to fetch SFML version 2.6.1
)

FetchContent_MakeAvailable(SFML)    # Makes sure SFML and its dependencies are avaiable for the project

#---- File management & .exe creation ----#

set(SOURCE_DIR src/)
include_directories(src/)

set(SOURCE_FILES src/main.cpp src/Game.cpp src/Game.h "src/Base/GameState.h" "src/Base/GameState.cpp" "src/GameStates/StartState.h" "src/GameStates/StartState.cpp" "src/GameStates/GameplayState.h" "src/GameStates/GameplayState.cpp" "src/Level/Tilemap.h" "src/Level/Tilemap.cpp" "src/Level/Objects/Player.h" "src/Base/GameObject.h" "src/Base/GameObject.cpp" "src/Level/Objects/Player.cpp" "src/Base/Vector2.h" "src/Base/Vector2.cpp" "src/Base/Animation.h" "src/Base/Animation.cpp" "src/Level/Objects/PlayerCue.h" "src/Level/Objects/PlayerCue.cpp" "src/UI/UIButton.h" "src/UI/UIButton.cpp" "src/Level/Objects/Heart.h" "src/Level/Objects/Heart.cpp" "src/Level/Objects/EndPotion.h" "src/Level/Objects/EndPotion.cpp" "src/GameStates/WinState.h" "src/GameStates/WinState.cpp" "src/GameStates/LoseState.h" "src/GameStates/LoseState.cpp" "src/Level/LevelRoom.h" "src/Level/LevelRoom.cpp" "src/Level/Objects/Gem.h" "src/Level/Objects/Gem.cpp" "src/Level/Objects/Bush.h" "src/Level/Objects/Bush.cpp" "src/Base/Enemy.h" "src/Base/Enemy.cpp" "src/Level/Objects/Enemies/EnemySlime.h" "src/Level/Objects/Enemies/EnemySlime.cpp" "src/GameStates/InstructionsState.h" "src/GameStates/InstructionsState.cpp" "src/Level/Objects/Enemies/EnemyMetal.h" "src/Level/Objects/Enemies/EnemyMetal.cpp" "src/Level/Pathfinder.h" "src/Level/Pathfinder.cpp" "src/Level/PathfindNode.h" "src/Level/PathfindNode.cpp" "src/UI/GameplayUI.h" "src/UI/GameplayUI.cpp" "src/Level/Objects/Enemies/EnemyHeavy.h" "src/Level/Objects/Enemies/EnemyHeavy.cpp" "src/Level/TilemapData.h")  # Sets variable SOURCE_FILES to hold names & locations of files that need to be compiled into the application
add_executable(PlatformerSFML ${SOURCE_FILES})  # Names the application and tells compiler which source files to use

target_link_libraries(PlatformerSFML PRIVATE sfml-graphics sfml-window sfml-system sfml-network sfml-audio) # Links the SFML libraries we need to our application
target_compile_features(PlatformerSFML PRIVATE cxx_std_20)  # Defines which standard of C++ the compiler should use

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/Data/ DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/Bin/Data/)  # Copies data from source directory to build directory (so the executable can access it)

if(WIN32)
    add_custom_command(
        TARGET PlatformerSFML
        COMMENT "Copy OpenAL DLL to build directory so app can have sound"
        PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${SFML_SOURCE_DIR}/extlibs/bin/$<IF:$<EQUAL:${CMAKE_SIZEOF_VOID_P},8>,x64,x86>/openal32.dll $<TARGET_FILE_DIR:PlatformerSFML>
        VERBATIM)
endif()