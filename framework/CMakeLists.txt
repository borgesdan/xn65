# CMakeList.txt : CMake project for xna, include source and define
# project specific logic here.
#

# Add source to this project's executable.
add_executable (xna WIN32 
"xna.cpp"
"platform/window-dx.cpp"
"platform/device-dx.cpp"
"platform/adapter-dx.cpp"
"platform/swapchain-dx.cpp"
"platform/rendertarget-dx.cpp"
"platform/texture-dx.cpp"
"platform/blendstate-dx.cpp"
"platform/game-dx.cpp"

"csharp/stream.cpp"
"platform/gdevicemanager-dx.cpp"

"platform/shader-dx.cpp"
"platform/rasterizerstate-dx.cpp"




"platform/samplerstate-dx.cpp"
"platform/sprite-dx.cpp"

"platform/depthstencilstate-dx.cpp"
"platform/keyboard-dx.cpp"
"platform/mouse-dx.cpp"
"platform/gamepad-dx.cpp"
"platform/soundeffect-dx.cpp"
"platform/displaymode-dx.cpp"

"game/component.cpp"
"content/manager.cpp"
"content/reader.cpp"
"csharp/binary.cpp"
"content/lzx/decoder.cpp"
"content/lzx/decoderstream.cpp"
"content/typereadermanager.cpp"
"csharp/object.cpp"
"csharp/type.cpp"
"platform/init-dx.cpp"
"game/servicecontainer.cpp"
"common/color.cpp"
"common/collision.cpp" 
"common/gjk.cpp"
"common/numerics.cpp" "common/packedvalue.cpp" "platform/buffer.cpp")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET xna PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.

find_package(directxtk CONFIG REQUIRED)

target_link_libraries(${PROJECT_NAME} D3d11.lib dxgi.lib dxguid.lib d3dcompiler.lib Microsoft::DirectXTK)
