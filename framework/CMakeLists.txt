# CMakeList.txt : CMake project for xna, include source and define
# project specific logic here.
#

# Add source to this project's executable.
add_executable (xna WIN32 
"xna.cpp"
"csharp/stream.cpp"
"game/component.cpp"
"content/manager.cpp"
"content/reader.cpp"
"csharp/binary.cpp"
"content/lzx/decoder.cpp"
"content/lzx/decoderstream.cpp"
"content/typereadermanager.cpp"
"csharp/object.cpp"
"csharp/type.cpp"
"game/servicecontainer.cpp"
"common/color.cpp"
"common/collision.cpp" 
"common/gjk.cpp"
"common/numerics.cpp"
"common/packedvalue.cpp" 
"platform-dx/window.cpp"
"platform-dx/device.cpp"
"platform-dx/adapter.cpp"
"platform-dx/swapchain.cpp"
"platform-dx/rendertarget.cpp"
"platform-dx/texture.cpp"
"platform-dx/blendstate.cpp"
"platform-dx/game.cpp"
"platform-dx/gdevicemanager.cpp"
"platform-dx/shader.cpp"
"platform-dx/rasterizerstate.cpp"
"platform-dx/samplerstate.cpp"
"platform-dx/sprite.cpp"
"platform-dx/depthstencilstate.cpp"
"platform-dx/keyboard.cpp"
"platform-dx/mouse.cpp"
"platform-dx/gamepad.cpp"
"platform-dx/soundeffect.cpp"
"platform-dx/displaymode.cpp"
"platform-dx/init.cpp"
"platform-dx/buffer.cpp"
"platform-dx/audioengine.cpp" )

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET xna PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.

find_package(directxtk CONFIG REQUIRED)

target_link_libraries(${PROJECT_NAME} D3d11.lib dxgi.lib dxguid.lib d3dcompiler.lib Microsoft::DirectXTK)
