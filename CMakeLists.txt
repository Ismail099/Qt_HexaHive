CMAKE_MINIMUM_REQUIRED(VERSION 3.21)
PROJECT(uTorrent)

STRING(APPEND LEGAL_COPYRIGHT "© Haidar Ibo <haidaribo@protonmail.com>")
STRING(APPEND OWNER "Haidar Ibo")
STRING(APPEND WEB "n.A.")
STRING(APPEND DESCRIPTION "Torrent Client and Server using complex TCP/IP operations.")
STRING(TIMESTAMP TODAY "%Y-%m-%d")

SET(CMAKE_CXX_STANDARD 20)
SET(CMAKE_PREFIX_PATH "E:/Qt/5.15.2/msvc2019_64")
SET(BUILD_SHARED_LIBS FALSE)

# Require certain CMake version, this is also used internally to globally set CMake policies
MESSAGE(STATUS "CMake v${CMAKE_VERSION} found on ${CMAKE_SYSTEM} ${CMAKE_SYSTEM_PROCESSOR} (${CMAKE_GENERATOR})")
IF (WIN32 OR UNIX)
    CMAKE_MINIMUM_REQUIRED(VERSION 3.1 FATAL_ERROR)   # CMake v3.1 has response files incorporated
    SET(CMAKE_CXX_USE_RESPONSE_FILE_FOR_LIBRARIES ON) # Forces the make file generator to use response files
    SET(CMAKE_CXX_USE_RESPONSE_FILE_FOR_INCLUDES ON)
ELSE ()
    MESSAGE(INFO "Please note that your operating system is not officially supported!")
ENDIF ()

# Version information
SET(MODULE_NAME QLinac)
SET(${MODULE_NAME}_MAJOR_VERSION 1)
SET(${MODULE_NAME}_MINOR_VERSION 0)
SET(${MODULE_NAME}_PATCH_VERSION 0)
STRING(APPEND ${MODULE_NAME}_VERSION ${${MODULE_NAME}_MAJOR_VERSION}.${${MODULE_NAME}_MINOR_VERSION}.${${MODULE_NAME}_PATCH_VERSION})
STRING(APPEND ${MODULE_NAME}_RELEASE_DATE ${TODAY}) # format: yyyy-MM-dd
SET(${MODULE_NAME}_VERSION ${${MODULE_NAME}_MAJOR_VERSION}.${${MODULE_NAME}_MINOR_VERSION}.${${MODULE_NAME}_REVISION})
ADD_DEFINITIONS(-D${MODULE_NAME}_MAJOR_VERSION=${${MODULE_NAME}_MAJOR_VERSION})
ADD_DEFINITIONS(-D${MODULE_NAME}_MINOR_VERSION=${${MODULE_NAME}_MINOR_VERSION})
ADD_DEFINITIONS(-D${MODULE_NAME}_PATCH_VERSION=${${MODULE_NAME}_PATCH_VERSION})
ADD_DEFINITIONS(-D${MODULE_NAME}_VERSION=${${MODULE_NAME}_VERSION})
ADD_DEFINITIONS(-D${MODULE_NAME}_RELEASE_DATE=\"${${MODULE_NAME}_RELEASE_DATE}\")
ADD_DEFINITIONS(-D${MODULE_NAME}_VERSION=${${MODULE_NAME}_VERSION})


SET(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src CACHE INTERNAL "")
SET(BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR}/src CACHE INTERNAL "")

OPTION(BUILD_TESTING "Build ${PROJECT_NAME} unit tests for regression tests and automatic software verification." ON)
SET(TESTING_ENABLED OFF)


OPTION(BUILD_API_DOCS "Generate API docs using Doxygen." OFF)
IF (BUILD_API_DOCS)
    FIND_PACKAGE(Doxygen)
    IF (NOT DOXYGEN_FOUND)
        MESSAGE(SEND_ERROR "Doxygen is needed to build the ${PROJECT_NAME} API documentation. Please install it correctly")
    ELSE ()

        # If Graphviz is installed use the 'dot' tool to generate graphs
        # NOTE: CMake is unable to find DOT on Windows.
        # NOTE: On windows the DOT_PATH must be set in the doxyfile otherwise errors occur.
        IF (DOXYGEN_DOT_FOUND OR (DOXYGEN_DOT_EXECUTABLE AND DOXYGEN_DOT_PATH))
            SET(HAVEDOT "YES")
            SET(DOTPATH ${DOXYGEN_DOT_PATH})
        ELSE ()
            SET(HAVEDOT "NO")
        ENDIF ()

        # Link to external library documentations through tag-files generated with
        # doxytag -t doc.tag path_to_html_doc.
        # NOTE: The tag files are not contained in the SVN repository.
        IF (EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/doc/gdcm-2.0.14.tag)
            LIST(APPEND TAGFILES "\"${CMAKE_CURRENT_SOURCE_DIR}/doc/gdcm-2.0.14.tag=http://gdcm.sourceforge.net/2.0/html\"")
        ENDIF ()

        IF (EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/doc/itk-3.20.tag)
            LIST(APPEND TAGFILES "\"${CMAKE_CURRENT_SOURCE_DIR}/doc/itk-3.20.tag=http://www.itk.org/Doxygen320/html\"")
        ENDIF ()

        IF (EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/doc/vtk-5.10.tag)
            LIST(APPEND TAGFILES "\"${CMAKE_CURRENT_SOURCE_DIR}/doc/vtk-5.10.tag=http://www.vtk.org/doc/release/5.10/html\"")
        ENDIF ()


        # NOTE: Starting with Qt 4.x the html doc was changed to qch files
        # (not possible to generate tag files for Qt from qch)
        # One must download the separate html documentation for tag file generation
        IF (EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/doc/qt-4.8.4.tag)
            LIST(APPEND TAGFILES "\"${CMAKE_CURRENT_SOURCE_DIR}/doc/qt-4.8.4.tag=http://qt-project.org/doc/qt-4.8\"")
        ENDIF (EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/doc/qt-4.8.4.tag)

        GET_FILENAME_COMPONENT(DOXYGEN_STRIP_FROM_PATH ${CMAKE_CURRENT_SOURCE_DIR} PATH)
        ADD_CUSTOM_TARGET(doc ALL
                ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/doc/Doxyfile
                WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/doc
                COMMENT "Generating CSPI API documentation with doxygen"
                VERBATIM)
    ENDIF (NOT DOXYGEN_FOUND)
ENDIF (BUILD_API_DOCS)


# Add custom cmake modules to cmake search path
LIST(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}")
LIST(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/CMakeModules")
INCLUDE(WinQtDeploy)


FIND_PACKAGE(Qt5Core REQUIRED)
FIND_PACKAGE(Qt5Widgets REQUIRED)
FIND_PACKAGE(Qt5Network REQUIRED)
FIND_PACKAGE(Qt5Test REQUIRED)
FIND_PACKAGE(Qt5Sql REQUIRED)
FIND_PACKAGE(Qt5Xml REQUIRED)
FIND_PACKAGE(Qt5LinguistTools REQUIRED)
FIND_PACKAGE(Qt5Designer REQUIRED)
FIND_PACKAGE(Qt5Concurrent REQUIRED)
FIND_PACKAGE(Qt5OpenGL REQUIRED)
FIND_PACKAGE(Qt5Charts REQUIRED)
FIND_PACKAGE(Qt5WebEngineWidgets REQUIRED)
SET(QT_LIBRARIES Qt5::Core Qt5::Widgets Qt5::Network Qt5::Test Qt5::Sql Qt5::Xml Qt5::Designer Qt5::Concurrent
        Qt5::OpenGL Qt5::Charts Qt5::WebEngineCore Qt5::WebEngineWidgets)
IF (WIN32)
    SET(QT_LIBRARIES ${QT_LIBRARIES} Qt5::WinMain)
ENDIF ()
SET(QT5_DIR "" CACHE PATH "Qt5 Directory that contains Qt5Core.dll")

INCLUDE_DIRECTORIES(${QT5_DIR})
SET(CMAKE_PREFIX_PATH ${QT5_DIR})

SET(QT_QTGUI_LIBRARIES
        ${Qt5Gui_LIBRARIES}
        ${Qt5Widgets_LIBRARIES}
        ${Qt5PrintSupport_LIBRARIES}
        ${Qt5Svg_LIBRARIES}
        ${Qt5Designer_LIBRARIES}
        ${QtWebEngineWidgets_LIBRARIES}
        )
SET(QT_INCLUDES
        ${Qt5Gui_INCLUDE_DIRS}
        ${Qt5Widgets_INCLUDE_DIRS}
        ${Qt5PrintSupport_INCLUDE_DIRS}
        ${Qt5Svg_INCLUDE_DIRS}
        ${Qt5Designer_INCLUDE_DIRS}
        ${Qt5Concurrent_INCLUDE_DIRS}
        ${Qt5OpenGL_INCLUDE_DIRS}
        )
SET(QT_QTGUI_LIBRARY ${QT_QTGUI_LIBRARIES})
SET(_qt_modules
        Core
        #Declarative
        Widgets
        Gui
        # Script
        # ScriptTools
        #DBus
        Network
        Test
        #Designer
        Concurrent
        #Xml
        #UiTools
        #Qml
        #Quick
        #WebKit
        #WebKitWidgets
        # Qt5QuickCompiler
        # Qt5QmlImportScanner
        Sql
        #OpenGL
        # Charts
        #WebEngineWidgets
        )

FOREACH (_module ${_qt_modules})
    STRING(TOUPPER ${_module} _module_upper)
    SET(QT_QT${_module_upper}_LIBRARIES ${Qt5${_module}_LIBRARIES})
    SET(QT_QT${_module_upper}_LIBRARY ${QT_QT${_module_upper}_LIBRARIES})
    LIST(APPEND QT_INCLUDES ${Qt5${_module}_INCLUDE_DIRS})
    SET(QT_QT${_module_upper}_FOUND ${Qt5${_module}_FOUND})
ENDFOREACH ()

GET_TARGET_PROPERTY(QT_QMAKE_EXECUTABLE Qt5::qmake LOCATION)
GET_TARGET_PROPERTY(QT_RCC_EXECUTABLE Qt5::rcc LOCATION)
GET_TARGET_PROPERTY(QT_LRELEASE_EXECUTABLE Qt5::lrelease LOCATION)

IF (NOT QT_TRANSLATIONS_DIR)
    FIND_PATH(QT_TRANSLATIONS_DIR "qttranslations/translations" HINT ${QT_QMAKE_EXECUTABLE})
ENDIF ()

IF (TARGET Qt5::uic)
    GET_TARGET_PROPERTY(QT_UIC_EXECUTABLE Qt5::uic LOCATION)
ENDIF ()

ADD_DEFINITIONS(-DQT_DISABLE_DEPRECATED_BEFORE=0)
INCLUDE_DIRECTORIES(${QT_INCLUDES})


SET(${PROJECT_NAME}_LINKED_LIST
        ${QT_LIBRARIES}
        ${SOURCE_LIBRARY}
        )
qt5_add_resources(RESOURCE resources.qrc)

ADD_SUBDIRECTORY(${SOURCE_DIR})
# ADD_SUBDIRECTORY(${BINARY_DIR})

ADD_EXECUTABLE(${CMAKE_PROJECT_NAME} main.cpp
        ${RESOURCE}
        )

TARGET_LINK_LIBRARIES(${PROJECT_NAME}
        # ${QT_LIBRARIES}
        ${${PROJECT_NAME}_LINKED_LIBS}
        )
SET_TARGET_PROPERTIES(${CMAKE_PROJECT_NAME} PROPERTIES LINKER_LANGUAGE CXX)
SET_TARGET_PROPERTIES(${CMAKE_PROJECT_NAME} PROPERTIES COMPILE_FLAGS "-DSORRYCLS_STATIC_DEFINE")

IF (UNIX)
    SET_TARGET_PROPERTIES(${QLinac_EXE_NAME} PROPERTIES COMPILE_FLAGS "-DSORRYCLS_STATIC_DEFINE -DHAVE_CONFIG_H")
ENDIF ()

IF (WIN32)
    WinQtDeploy(${CMAKE_PROJECT_NAME})
ENDIF ()


##
## Testing
##
IF (TESTING_ENABLED)
    MESSAGE(STATUS "Testing for ${CMAKE_PROJECT_NAME} is being built...")
    ADD_SUBDIRECTORY(tst)
    INCLUDE(GoogleTest)
    ENABLE_TESTING()
    ADD_EXECUTABLE(${CMAKE_PROJECT_NAME}_tst ${TEST_SOURCE})
    # TARGET_LINK_LIBRARIES(${CMAKE_PROJECT_NAME}_tst PUBLIC gtest  ${TEST_LIBRARIES})
    # ADD_TEST(NAME ${CMAKE_PROJECT_NAME}_tst COMMAND ${CMAKE_PROJECT_NAME}_tst)
    # ADD_EXECUTABLE(${CMAKE_PROJECT_NAME}_test ${TEST_SOURCES})
    # ADD_EXECUTABLE(${CMAKE_PROJECT_NAME}_tst ${TEST_SOURCES} tst/main.cpp)
    # ADD_TEST(NAME ${CMAKE_PROJECT_NAME}_tst COMMAND ${CMAKE_PROJECT_NAME}_tst)
    # gtest_discover_tests(${CMAKE_PROJECT_NAME}_tst)
ENDIF ()