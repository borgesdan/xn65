# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.8)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

# CMAKE_TOOLCHAIN_FILE
include("C:/vcpkg/scripts/buildsystems/vcpkg.cmake")
project ("xna")

# Include sub-projects.
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/inc)
add_subdirectory ("framework")
add_subdirectory ("samples")


