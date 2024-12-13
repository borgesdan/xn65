# CMakeList.txt : CMake project for xna, include source and define
# project specific logic here.
#

# Add source to this project's executable.
add_library (Xn65 STATIC 
"game/component.cpp"
"game/servicecontainer.cpp"
"content/manager.cpp"
"content/reader.cpp"
"content/lzx/decoder.cpp"
"content/typereadermanager.cpp"
"common/color.cpp"
"common/collision.cpp" 
"common/gjk.cpp"
"common/numerics.cpp"
"common/packedvalue.cpp"
"graphics/displaymode.cpp"
)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET Xn65 PROPERTY CXX_STANDARD 20)
endif()

#find_package(tinyxml2 CONFIG REQUIRED)

target_link_libraries(Xn65 CSharp++ "${PROJECT_VCPKG_DIRECTORY}/installed/x64-windows/lib/tinyxml2.lib")
