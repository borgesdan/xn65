# CMakeList.txt : CMake project for CmakeSharedExeTest, include source and define
# project specific logic here.
#

# Add source to this project's executable.
add_executable (SampleApp WIN32 "xna.cpp")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET SampleApp PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
target_link_libraries(SampleApp Xn65)
