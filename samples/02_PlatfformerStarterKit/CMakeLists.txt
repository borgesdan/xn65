﻿# CMakeList.txt : CMake project for CmakeSharedExeTest, include source and define
# project specific logic here.
#

# Add source to this project's executable.
add_executable (PlatformApp WIN32 "game.cpp" "animation.cpp" "enemy.cpp" "level.cpp" "player.cpp" "gem.cpp")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET PlatformApp PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
target_link_libraries(
	PlatformApp Xn65DX
)