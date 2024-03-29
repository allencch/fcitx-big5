# This is a template of a simple template of a fcitx module with name "example"
# Please remember to replace all "[Ee]xample"'s in the files and file names
# to your addon's real name.
cmake_minimum_required(VERSION 2.6)

project(fcitx-big5)
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})
find_package(Fcitx 4.2.7 REQUIRED)

# This will add a uninstall target. Comment out this line if other cmake packages
# you are using already provides such target (e.g. KDE).
_fcitx_add_uninstall_target()
include_directories(${FCITX4_FCITX_INCLUDE_DIRS})
include_directories(${FCITX4_FCITX_CONFIG_INCLUDE_DIRS})
include_directories(${FCITX4_FCITX_UTILS_INCLUDE_DIRS})
include_directories("${PROJECT_BINARY_DIR}")

set(CMAKE_C_FLAGS "-Wall -Wextra -Wno-sign-compare -Wno-unused-parameter -fvisibility=hidden ${CMAKE_C_FLAGS}")
set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wno-sign-compare -Wno-unused-parameter -fvisibility=hidden ${CMAKE_CXX_FLAGS}")
set(CMAKE_SHARED_LINKER_FLAGS "-Wl,--as-needed ${CMAKE_SHARED_LINKER_FLAGS}")
set(CMAKE_MODULE_LINKER_FLAGS "-Wl,--as-needed ${CMAKE_MODULE_LINKER_FLAGS}")
add_definitions("-DLOCALEDIR=\"${CMAKE_INSTALL_PREFIX}/share/locale\"")

configure_file(config.h.in config.h
  IMMEDIATE @ONLY)

# The po dir (and targets, if any) should be added before any other translate
# related sources are processed.
add_subdirectory(po)
add_subdirectory(src)
