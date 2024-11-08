# CMakeList.txt : CMake project for xna, include source and define
# project specific logic here.
#

# Add source to this project's executable.
add_library (Xn65Pipeline STATIC 
"writer.cpp"
"compiler.cpp"
)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET Xn65Pipeline PROPERTY CXX_STANDARD 20)
endif()

#target_link_libraries(Xn65Pipeline)
