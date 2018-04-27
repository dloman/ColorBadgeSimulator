cmake_minimum_required(VERSION 3.7)

set(CMAKE_C_COMPILER /usr/local/bin/gcc)
set(CMAKE_CXX_COMPILER /usr/local/bin/g++)

#fixes wierd library shortname isssue
CMAKE_POLICY( SET CMP0060 NEW )

project(ColorThingy)

set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}")

add_definitions(-w)
add_definitions(-fPIC)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1z")
add_definitions(-std=c++1z)
#------------------------------------------------------------------------------
## WxWidgets
#------------------------------------------------------------------------------
set(wxWidgets_ROOT_DIR
  "C:/ExternalPackages/wxWidgets-3.1.0-t1-VC12"
  CACHE PATH "wxWidgets root directory"
  FORCE)

find_package(wxWidgets COMPONENTS core base REQUIRED)

# wxWidgets include (this will do all the magic to configure everything)
include( "${wxWidgets_USE_FILE}" )


#------------------------------------------------------------------------------
#------------------------------------------------------------------------------
add_executable(
  ColorThingy
  Gui.cpp
  Badge.cpp
  )

target_link_libraries(ColorThingy ${wxWidgets_LIBRARIES})
