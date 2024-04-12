# CMakeList.txt : CMake project for xna, include source and define
# project specific logic here.
#

# Add source to this project's executable.
add_executable (xna WIN32 "xna.cpp" "xna.h" "platform/window-dx.cpp" "platform/device-dx.cpp"   "platform/adapter-dx.cpp" "platform/swapchain-dx.cpp" "platform/rendertarget-dx.cpp" "platform/texture-dx.cpp" "platform/blendstate-dx.cpp" "platform/game-dx.cpp" "platform/clock-dx.cpp" "csharp/stream.cpp"  "platform/gdevicemanager-dx.cpp" "platform/vertexinput-dx.cpp" "platform/shader-dx.cpp" "platform/rasterizer-dx.cpp" "platform/vertexbuffer-dx.cpp" "platform/indexbuffer-dx.cpp" "common/matrix.cpp" "platform/constbuffer-dx.cpp" "platform/databuffer-dx.cpp")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET xna PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.

# -- Biblioteca DirectxTK --
# Url: https://github.com/microsoft/DirectXTK/wiki/DirectXTK
#
# -- Instalação via vcpkg --
# Para efetuar o download do vcpkg verifique o caminho abaixo
# Url: https://learn.microsoft.com/pt-br/vcpkg/get_started/get-started?pivots=shell-cmd
#
# Siga os procedimentos da página oficial do DirectxTK para instalação via vcpkg
#
# [!] Atualize o arquivo CMakePresets.json, nos 'presets' necessários,
# para que find_package execute corretamente
#
# "cacheVariables": {
#    "CMAKE_TOOLCHAIN_FILE": "{VCPKG_DIR}\\scripts\\buildsystems\\vcpkg.cmake"
# }
#
find_package(directxtk CONFIG REQUIRED)

target_link_libraries(${PROJECT_NAME} D3d11.lib dxgi.lib d3dcompiler.lib Microsoft::DirectXTK)
