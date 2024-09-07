# CMakeList.txt : CMake project for xna, include source and define
# project specific logic here.
#

# Add source to this project's executable.
add_library (Xn65DX STATIC 
"window.cpp"
"device.cpp"
"adapter.cpp"
"swapchain.cpp"
"rendertarget.cpp"
"texture.cpp"
"blendstate.cpp"
"game.cpp"
"gdevicemanager.cpp"
"rasterizerstate.cpp"
"samplerstate.cpp"
"sprite.cpp"
"depthstencilstate.cpp"
"keyboard.cpp"
"mouse.cpp"
"gamepad.cpp"
"soundeffect.cpp"
"displaymode.cpp"
"init.cpp"
"audioengine.cpp"
"effect.cpp"
"screen.cpp")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET Xn65DX PROPERTY CXX_STANDARD 20)
endif()

find_package(directxtk CONFIG REQUIRED)

target_link_libraries(
	Xn65DX Xn65 D3d11.lib dxgi.lib dxguid.lib d3dcompiler.lib Microsoft::DirectXTK dxguid.lib 
)
