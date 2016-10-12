########################################################
# Files

set(AUTH_BROWSER_SRCS
  testwidget.cpp
  browser.cpp
  main.cpp
)

set(AUTH_BROWSER_UIS
  browser.ui
  testwidget.ui
)

set(AUTH_BROWSER_MOC_HDRS
  browser.h
  testwidget.h
)

########################################################
# Build

CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/config.h.in ${CMAKE_CURRENT_BINARY_DIR}/config.h)

QT4_WRAP_UI(AUTH_BROWSER_UIS_H  ${AUTH_BROWSER_UIS})

QT4_WRAP_CPP(AUTH_BROWSER_MOC_SRCS  ${AUTH_BROWSER_MOC_HDRS})

SET(IMAGE_RCCS images/images.qrc)
QT4_ADD_RESOURCES(AUTH_BROWSER_IMAGE_RCC_SRCS ${IMAGE_RCCS})

include_directories(SYSTEM
  ${QT_INCLUDE_DIR}
  ${QGIS_INCLUDE_DIR}
  ${QCA_INCLUDE_DIR}
)
include_directories(
  ${CMAKE_CURRENT_BINARY_DIR}
)

add_executable(qgisauthtestbrowser MACOSX_BUNDLE WIN32
  ${AUTH_BROWSER_SRCS}
  ${AUTH_BROWSER_MOC_SRCS}
  ${AUTH_BROWSER_UIS_H}
  ${AUTH_BROWSER_IMAGE_RCC_SRCS}
)

target_link_libraries(qgisauthtestbrowser
  ${QGIS_CORE_LIBRARY}
  ${QGIS_GUI_LIBRARY}
  ${QT_QTCORE_LIBRARY}
  ${QT_QTGUI_LIBRARY}
  ${QT_QTNETWORK_LIBRARY}
  ${QT_QTXML_LIBRARY}
  ${QT_QTWEBKIT_LIBRARY}
  ${QT_QTMAIN_LIBRARY}
)

if(APPLE)
  add_definitions(-DQT_NO_CAST_TO_ASCII)
  target_link_libraries(qgisauthtestbrowser ${APP_SERVICES_LIBRARY})
  set_target_properties(qgisauthtestbrowser PROPERTIES
    OUTPUT_NAME "Auth-Test-Browser"
  )
endif(APPLE)

########################################################
# Install

install(TARGETS qgisauthtestbrowser
  BUNDLE DESTINATION bin
  RUNTIME DESTINATION bin
)
