cmake_minimum_required(VERSION 3.16)
project(ds VERSION 0.2.2 LANGUAGES C)

if(POLICY CMP0069)
  cmake_policy(SET CMP0069 NEW)
endif()
if(POLICY CMP0076)
  cmake_policy(SET CMP0076 NEW)
endif()

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED YES)
set(DEFAULT_BUILD_TYPE "Release")

get_directory_property(DS_HAS_PARENT PARENT_DIRECTORY)

set(CMAKE_C_FLAGS_DEBUG "-g")
if(MSVC)
  set(CMAKE_C_FLAGS_RELEASE "/Ox /DNDEBUG")
else()
  set(CMAKE_C_FLAGS_RELEASE "-O3 -DNDEBUG")
endif()

set(DS_BUILD)
option(DS_SHARED "Shared build" ON)
option(DS_STATIC "Static build" OFF)
option(DS_USE_C99 "" OFF)
option(DS_USE_TEST "Enable Tests" OFF)
option(DS_ENABLE_LTO "Enable link-time optimization for release builds" OFF)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")

if(NOT DS_STATIC AND DS_SHARED)
  set(DS_BUILD SHARED)
else(DS_STATIC)
  set(DS_BUILD STATIC)
endif()

if(DS_USE_C99)
  set(CMAKE_C_STANDARD 99)
endif()

if(MSVC)
  # Ref: https://skia.googlesource.com/third_party/sdl/+/refs/heads/master/CMakeLists.txt#225
  # Make sure /RTC1 is disabled, otherwise it will use functions from the CRT
  foreach(flag_var
      CMAKE_C_FLAGS CMAKE_C_FLAGS_DEBUG CMAKE_C_FLAGS_RELEASE
      CMAKE_C_FLAGS_MINSIZEREL CMAKE_C_FLAGS_RELWITHDEBINFO)
    string(REGEX REPLACE "/RTC(su|[1su])" "" ${flag_var} "${${flag_var}}")
  endforeach(flag_var)
endif()

if(NOT DS_HAS_PARENT AND NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message(STATUS "Setting build type to '${DEFAULT_BUILD_TYPE}' as none was specified.")
  set(CMAKE_BUILD_TYPE "${DEFAULT_BUILD_TYPE}" CACHE STRING "Choose the type of build." FORCE)
  # Set the possible values of build type for cmake-gui
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

if(DS_ENABLE_LTO)
  if(CMAKE_VERSION VERSION_LESS 3.9)
    message(WARNING "LTO was requested but CMake >= 3.9 is required for CheckIPOSupported.")
  else()
    include(CheckIPOSupported)
    check_ipo_supported(RESULT DS_LTO_OK OUTPUT DS_LTO_MSG)
    if(DS_LTO_OK)
      set(CMAKE_INTERPROCEDURAL_OPTIMIZATION_RELEASE ON)
      set(CMAKE_INTERPROCEDURAL_OPTIMIZATION_RELWITHDEBINFO ON)
    else()
      message(WARNING "LTO was requested but is not supported: ${DS_LTO_MSG}")
    endif()
  endif()
endif()

function(ds_target_options target)
  if(MSVC)
    target_compile_options(${target}
        PRIVATE
            /W3
            /TC
            $<$<CONFIG:Release>:/Gy>
            $<$<CONFIG:Release>:/Oi>
            $<$<CONFIG:RelWithDebInfo>:/Gy>
            $<$<CONFIG:RelWithDebInfo>:/Oi>)
  else()
    target_compile_options(${target}
        PRIVATE
            -Wall
            -Wextra
            -Wstrict-aliasing=2
            -Wno-overlength-strings
            -Wmissing-declarations)
  endif()
endfunction()

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
add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})
ds_target_options(${PROJECT_NAME})

if(DS_SHARED)
  target_compile_definitions(${PROJECT_NAME} PRIVATE DS_EXPORTS)
  if(MSVC)
    target_compile_definitions(${PROJECT_NAME}
        PRIVATE
            _WINDOWS
            _USRDLL)
  endif()
else()
  target_compile_definitions(${PROJECT_NAME} PUBLIC DS_STATIC)
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
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

install(DIRECTORY include/${PROJECT_NAME} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
        PATTERN ".*" EXCLUDE)

# Config
export(TARGETS ${PROJECT_NAME}
       NAMESPACE ${PROJECT_NAME}::
       FILE "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
)

install(EXPORT      ${PROJECT_NAME}
        FILE        "${PROJECT_NAME}Config.cmake"
        NAMESPACE   ${PROJECT_NAME}::
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME})
