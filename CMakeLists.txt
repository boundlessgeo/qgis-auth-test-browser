cmake_minimum_required(VERSION 2.8.11)

set(CMAKE_MODULE_PATH
  ${CMAKE_SOURCE_DIR}/cmake/modules
  ${CMAKE_MODULE_PATH}
)

if(POLICY CMP0048) # in CMake >= 3.0.0
    cmake_policy(SET CMP0048 OLD) # keep project() from clearing VERSION variables
endif(POLICY CMP0048)
set(VER_MAJOR "0")
set(VER_MINOR "1")
set(VER_PATCH "0")
set(API_VER_PATCH "0")
set(CPACK_PACKAGE_VERSION_MAJOR ${VER_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${VER_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${VER_PATCH})
set(PROJECT_VERSION ${VER_MAJOR}.${VER_MINOR}.${VER_PATCH})
set(API_VERSION ${VER_MAJOR}.${VER_MINOR}.${API_VER_PATCH})

project(qgis-auth-test-browser)

if(CMAKE_BUILD_TYPE MATCHES Debug OR CMAKE_BUILD_TYPE MATCHES RelWithDebInfo)
  message(STATUS "Debug output enabled")
  add_definitions(-UQT_NO_DEBUG_OUTPUT)
  add_definitions(-DQGISDEBUG=1)
endif()

option(USE_CXX_11 "Enable C++11 compiler support" ON)

#allow override keyword if available
if (NOT USE_CXX_11)
  add_definitions("-Doverride=")
  add_definitions("-Dnullptr=0")
ENDIF()

option(ENABLE_TESTS "Enable building of unit tests" ON)

set(QGIS_PREFIX_PATH "" CACHE STRING "QGIS application install prefix")

if(MSVC)
  set(OSGEO4W_PREFIX "" CACHE STRING "OSGeo4W install prefix")
endif()

# Qt find routines from QGIS 2.14.3 code base
set(QT_MIN_VERSION 4.8.0)

option(WITH_QTWEBKIT "Enable QtWebkit support" ON)
if(WITH_QTWEBKIT)
  add_definitions(-DWITH_QTWEBKIT)
endif()

set(ENABLE_QT5 FALSE CACHE BOOL "If enabled will try to find Qt5 before looking for Qt4")
if(ENABLE_QT5)
  find_package(Qt5Core QUIET)
  find_package(Qt5Gui REQUIRED)
  find_package(Qt5Widgets REQUIRED)
  find_package(Qt5Network REQUIRED)
  find_package(Qt5Svg REQUIRED)
  find_package(Qt5Xml REQUIRED)
  find_package(Qt5Concurrent REQUIRED)
  find_package(Qt5PrintSupport REQUIRED)
  find_package(Qt5Positioning)
  if (WITH_QTWEBKIT)
    find_package(Qt5WebKit REQUIRED)
    find_package(Qt5WebKitWidgets REQUIRED)
  endif()
  find_package(Qt5Test REQUIRED)
  find_package(Qt5UiTools REQUIRED)
  find_package(Qt5Script REQUIRED)
  find_package(Qt5Sql REQUIRED)
  include("cmake/modules/ECMQt4To5Porting.cmake")
  message(STATUS "Found Qt version: ${Qt5Core_VERSION_STRING}")
else()
  find_package(Qt4 ${QT_MIN_VERSION} REQUIRED)
  set(QT_USE_QTXML 1)
  set(QT_USE_QTNETWORK 1)
  set(QT_USE_QTSVG 1)
  set(QT_USE_QTSQL 1)
  set(QT_USE_QTWEBKIT 1)
  if(NOT QT_QTXML_FOUND OR NOT QT_QTNETWORK_FOUND OR NOT QT_QTSVG_FOUND OR
      NOT QT_QTSQL_FOUND OR NOT QT_QTWEBKIT_FOUND OR
      (WITH_CUSTOM_WIDGETS AND NOT QT_QTDESIGNER_FOUND))
    message(SEND_ERROR "Some Qt4 modules haven't been found!")
  endif()

  include(${QT_USE_FILE})
  message(STATUS "Found Qt version: ${QTVERSION}")

endif()

find_package(QCA REQUIRED)

if(APPLE)
  #this will define ${APP_SERVICES_LIBRARY}
  find_library(APP_SERVICES_LIBRARY ApplicationServices )
  if(EXISTS "${QGIS_PREFIX_PATH}/CMakeCache.txt")
    set(QGIS_BUILD_PATH "${QGIS_PREFIX_PATH}")
    add_definitions("-DQGIS_BUILD_PATH=\"${QGIS_BUILD_PATH}\"")
  elseif(EXISTS "${QGIS_PREFIX_PATH}/QGIS.app")
    set(QGIS_MAC_PATH "${QGIS_PREFIX_PATH}/QGIS.app/Contents")
  endif()
  if (USE_CXX_11)
    # allow c++11
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wno-error=c++11-narrowing -Wno-return-type-c-linkage")
  endif ()
elseif(MSVC AND OSGEO4W_PREFIX)
  set(OSGEO4W_ROOT "${OSGEO4W_PREFIX}")
endif()

find_package(QGIS REQUIRED)

if(WIN32)
  set(DLLIMPORT "__declspec(dllimport)")
endif(WIN32)
add_definitions("-DCORE_EXPORT=${DLLIMPORT}")
add_definitions("-DGUI_EXPORT=${DLLIMPORT}")

add_subdirectory(auth-test-browser)

if (ENABLE_TESTS)
  include(CTest)
  add_subdirectory(tests)
  set(QT_USE_QTTEST TRUE)
  set(CTEST_BINARY_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/tests" )
  message(STATUS "Ctest Binary Directory set to: ${CTEST_BINARY_DIRECTORY}")
endif()
