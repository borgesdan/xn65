# CMakeList.txt : CMake project for xna, include source and define
# project specific logic here.
#

# Add source to this project's executable.
add_library (Xn65Pipeline STATIC 
"discard.cpp"
)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET Xn65Pipeline PROPERTY CXX_STANDARD 20)
endif()

find_package(directxtk CONFIG REQUIRED)

#target_link_libraries(Xn65Pipeline)
