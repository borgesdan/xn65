# CMakeList.txt : CMake project for Xna, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

# Includes directory
set(PROJECT_INCLUDES_DIR "${CMAKE_CURRENT_SOURCE_DIR}/includes")

project ("Xna")

# Include sub-projects.
include_directories(${PROJECT_INCLUDES_DIR})

# Add source to this project's executable.
add_executable (Xna 
"Xna.cpp" 
"Xna.h" 
"sources/xna/framework/PackUtils.cpp"
"sources/xna/framework/Gjk.cpp" 
"sources/xna/framework/Plane.cpp"
"sources/xna/framework/BoundingFrustum.cpp" 
"sources/xna/framework/BoundingBox.cpp" 
"sources/xna/framework/BoundingSphere.cpp"
"sources/xna/framework/Color.cpp" 
"sources/xna/framework/Input/GamePad.cpp" 
"sources/xna/framework/Input/Keyboard.cpp"
"sources/xna/framework/Input/Mouse.cpp" 
"sources/xna/Framework/Game/GameWindow.cpp"
"sources/CSharp/Windows/Forms/SystemInformation.cpp" 
"sources/CSharp/Windows/Forms/Screen.cpp")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET Xna PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
target_compile_definitions(Xna PRIVATE USING_XINPUT)

find_package(directxtk CONFIG REQUIRED)

target_link_libraries(
	Xna Microsoft::DirectXTK
)

