cmake_minimum_required(VERSION 3.8.2)
project(ds VERSION 0.2.2 LANGUAGES C)

if(POLICY CMP0076)
  cmake_policy(SET CMP0076 NEW)
endif()

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED YES)
set(DEFAULT_BUILD_TYPE "Release")

set(CMAKE_C_FLAGS_DEBUG "-g")
set(CMAKE_C_FLAGS_RELEASE "-O3")

set(DS_BUILD)
option(DS_SHARED "Shared build" ON)
option(DS_STATIC "Static build" OFF)
option(DS_USE_C99 "" OFF)
option(DS_USE_TEST "Enable Tests" OFF)

if(NOT DS_STATIC AND DS_SHARED)
  set(DS_BUILD SHARED)
else(DS_STATIC)
  set(DS_BUILD STATIC)
endif()

if(DS_USE_C99)
  set(C_STANDARD 99)
endif()

if(MSVC)
  add_definitions(-DNDEBUG -D_WINDOWS -D_USRDLL -DDS_EXPORTS)
  add_compile_options(/W3 /Ox /Gy /Oi /TC)
  
  # Ref: https://skia.googlesource.com/third_party/sdl/+/refs/heads/master/CMakeLists.txt#225
  # Make sure /RTC1 is disabled, otherwise it will use functions from the CRT
  foreach(flag_var
      CMAKE_C_FLAGS CMAKE_C_FLAGS_DEBUG CMAKE_C_FLAGS_RELEASE
      CMAKE_C_FLAGS_MINSIZEREL CMAKE_C_FLAGS_RELWITHDEBINFO)
    string(REGEX REPLACE "/RTC(su|[1su])" "" ${flag_var} "${${flag_var}}")
  endforeach(flag_var)
else()
  add_compile_options(-Wall -Wextra -O3 -Wstrict-aliasing=2 -Wno-overlength-strings -Wmissing-declarations)
endif()

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message(STATUS "Setting build type to '${DEFAULT_BUILD_TYPE}' as none was specified.")
  set(CMAKE_BUILD_TYPE "${DEFAULT_BUILD_TYPE}" CACHE STRING "Choose the type of build." FORCE)
  # Set the possible values of build type for cmake-gui
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

include(GNUInstallDirs)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})

if(NOT CPack_CMake_INCLUDED)
  include(CPack)
endif()

# Target Start
add_library(${PROJECT_NAME}
  ${DS_BUILD}
  src/default/default.c
  src/allocator.c
  src/util.c
  src/rb.c
  src/forward_list.c
  src/flist_separate.c
  src/hash_table.c
  src/hash_funcs.c
  src/sort/sort.c
  )

if(DS_SHARED)
  add_definitions(-DDS_EXPORTS)
else()
  target_compile_definitions(${PROJECT_NAME} PUBLIC -DDS_STATIC)
endif()

set_target_properties(${PROJECT_NAME} PROPERTIES
                              VERSION ${PROJECT_VERSION} 
                            SOVERSION ${PROJECT_VERSION_MAJOR})

target_include_directories(${PROJECT_NAME}
    PUBLIC 
        $<INSTALL_INTERFACE:include>    
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# Test Configuration
if(DS_USE_TEST)
  include(CTest)
  enable_testing()
  add_subdirectory(test)
endif()

# Install 
install(TARGETS ${PROJECT_NAME}
        EXPORT  ${PROJECT_NAME}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_LIBDIR})

install(DIRECTORY include/${PROJECT_NAME} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
        PATTERN ".*" EXCLUDE)

# Config
export(TARGETS ${PROJECT_NAME}
       NAMESPACE ${PROJECT_NAME}::
       FILE "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
)

install(EXPORT      ${PROJECT_NAME}
        NAMESPACE   ${PROJECT_NAME}::
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/${PROJECT_NAME}/cmake)

