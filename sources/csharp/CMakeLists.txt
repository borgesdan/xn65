﻿# CMakeList.txt : CMake project for xna, include source and define
# project specific logic here.
#

# Add source to this project's executable.
add_library (CSharp++ STATIC 
	"exception.cpp"
 "io/stream.cpp" "io/binary.cpp"  "windows/forms/screen.cpp" "windows/forms/system.cpp")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET CSharp++ PROPERTY CXX_STANDARD 20)
endif()

#find_package(tinyxml2 CONFIG REQUIRED)

target_link_libraries(CSharp++ "${PROJECT_VCPKG_DIRECTORY}/installed/x64-windows/lib/tinyxml2.lib")
