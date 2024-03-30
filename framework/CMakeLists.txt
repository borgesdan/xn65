# CMakeList.txt : CMake project for xna, include source and define
# project specific logic here.
#

# Add source to this project's executable.
add_executable (xna WIN32 "xna.cpp" "xna.h" "platform/window-dx.cpp" "platform/device-dx.cpp"   "platform/adapter-dx.cpp" "platform/swapchain-dx.cpp" "platform/rendertarget-dx.cpp" "platform/texture-dx.cpp" "platform/blendstate-dx.cpp" "platform/game-dx.cpp" "platform/clock-dx.cpp" "csharp/stream.cpp"  "platform/gdevicemanager-dx.cpp")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET xna PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
target_link_libraries(${PROJECT_NAME} D3d11.lib dxgi.lib)
