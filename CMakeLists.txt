CMAKE_MINIMUM_REQUIRED(VERSION 3.21)
PROJECT(uTorrent)

SET(CMAKE_PREFIX_PATH "E:/Qt/6.2.4/msvc2019_64")
SET(BUILD_SHARED_LIBS TRUE)
SET(CMAKE_INCLUDE_CURRENT_DIR ON)
SET(CMAKE_AUTOUIC ON)
SET(CMAKE_AUTOMOC ON)
SET(CMAKE_AUTORCC ON)
SET(CMAKE_CXX_STANDARD_REQUIRED ON)

SET(CMAKE_CXX_STANDARD 20)
# SET(CMAKE_PREFIX_PATH "E:/Qt/5.15.2/msvc2019_64")

# Require certain CMake version, this is also used internally to globally set CMake policies
MESSAGE(STATUS "CMake v${CMAKE_VERSION} found on ${CMAKE_SYSTEM} ${CMAKE_SYSTEM_PROCESSOR} (${CMAKE_GENERATOR})")
IF (WIN32)
    CMAKE_MINIMUM_REQUIRED(VERSION 3.20.1 FATAL_ERROR)# CMake v3.1 has response files incoporated
    # Forces the make file generator to use response files
    SET(CMAKE_CXX_USE_RESPONSE_FILE_FOR_LIBRARIES ON)
    SET(CMAKE_CXX_USE_RESPONSE_FILE_FOR_INCLUDES ON)
ELSEIF (UNIX)
    # 2.8.7 released 2011-12-30 (required for 12.04)
    CMAKE_MINIMUM_REQUIRED(VERSION 3.23.1 FATAL_ERROR)
ELSE ()
    MESSAGE(INFO "Please note that your operating system is not officially supported!")
ENDIF ()


# Version information
SET(MODULE_NAME uTorrent)
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


OPTION(BUILD_API_DOCS "Generate API docs using Doxygen." ON)
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

SET(Qt_Version "6")

FIND_PACKAGE(Qt${Qt_Version}Core REQUIRED)
FIND_PACKAGE(Qt${Qt_Version}Widgets REQUIRED)
FIND_PACKAGE(Qt${Qt_Version}Network REQUIRED)
FIND_PACKAGE(Qt${Qt_Version}Sql REQUIRED)
FIND_PACKAGE(Qt${Qt_Version}Xml REQUIRED)
FIND_PACKAGE(Qt${Qt_Version}Concurrent REQUIRED)
FIND_PACKAGE(Qt${Qt_Version}OpenGL REQUIRED)
FIND_PACKAGE(Qt${Qt_Version}Charts REQUIRED)
FIND_PACKAGE(Qt${Qt_Version}Help REQUIRED)
FIND_PACKAGE(Qt${Qt_Version}PrintSupport REQUIRED)

SET(QT_LIBRARIES Qt${Qt_Version}::Core Qt${Qt_Version}::Widgets Qt${Qt_Version}::Network
        Qt${Qt_Version}::Sql Qt${Qt_Version}::Xml Qt${Qt_Version}::Concurrent
        Qt${Qt_Version}::Charts Qt${Qt_Version}::Help Qt${Qt_Version}::PrintSupport)

SET(QT${Qt_Version}_DIR "" CACHE PATH "Qt${Qt_Version} Directory that contains Qt${Qt_Version}Core.dll")

INCLUDE_DIRECTORIES(${QT${Qt_Version}_DIR})
SET(CMAKE_PREFIX_PATH ${QT${Qt_Version}_DIR})


SET(QT_QTGUI_LIBRARIES
        ${Qt${Qt_Version}Gui_LIBRARIES}
        ${Qt${Qt_Version}Widgets_LIBRARIES}
        ${Qt${Qt_Version}PrintSupport_LIBRARIES}
        ${Qt${Qt_Version}Svg_LIBRARIES}
        )
SET(QT_INCLUDES
        ${Qt${Qt_Version}Gui_INCLUDE_DIRS}
        ${Qt${Qt_Version}Widgets_INCLUDE_DIRS}
        ${Qt${Qt_Version}PrintSupport_INCLUDE_DIRS}
        ${Qt${Qt_Version}Svg_INCLUDE_DIRS}
        ${Qt${Qt_Version}Concurrent_INCLUDE_DIRS}
        ${Qt${Qt_Version}OpenGL_INCLUDE_DIRS}
        )

SET(QT_QTGUI_LIBRARY ${QT_QTGUI_LIBRARIES})
SET(_qt_modules
        Core
        Widgets
        Gui
        Network
        Concurrent
        Xml
        Sql
        OpenGL
        Charts
        )
FOREACH (_module ${_qt_modules})
    STRING(TOUPPER ${_module} _module_upper)
    SET(QT_QT${_module_upper}_LIBRARIES ${Qt${Qt_Version}${_module}_LIBRARIES})
    SET(QT_QT${_module_upper}_LIBRARY ${QT_QT${_module_upper}_LIBRARIES})
    LIST(APPEND QT_INCLUDES ${Qt${Qt_Version}${_module}_INCLUDE_DIRS})
    SET(QT_QT${_module_upper}_FOUND ${Qt${Qt_Version}${_module}_FOUND})
ENDFOREACH ()

GET_TARGET_PROPERTY(QT_QMAKE_EXECUTABLE Qt${Qt_Version}::qmake LOCATION)
GET_TARGET_PROPERTY(QT_RCC_EXECUTABLE Qt${Qt_Version}::rcc LOCATION)
# GET_TARGET_PROPERTY(QT_LRELEASE_EXECUTABLE Qt${Qt_Version}::lrelease LOCATION)


IF (NOT QT_TRANSLATIONS_DIR)
    FIND_PATH(QT_TRANSLATIONS_DIR "qttranslations/translations" HINT ${QT_QMAKE_EXECUTABLE})
ENDIF ()

IF (TARGET Qt${Qt_Version}::uic)
    GET_TARGET_PROPERTY(QT_UIC_EXECUTABLE Qt${Qt_Version}::uic LOCATION)
ENDIF ()
ADD_DEFINITIONS(-DQT_DISABLE_DEPRECATED_BEFORE=0)
INCLUDE_DIRECTORIES(${QT_INCLUDES})


##
## Compile the .rc.in (Resource Input) file with a custom command.
##
IF (WIN32)
    SET(${PROJECT_NAME}_RC_RES ${CMAKE_CURRENT_BINARY_DIR}/CMakeFiles/${PROJECT_NAME}.dir/${PROJECT_NAME}.rc.res)
    SET(${PROJECT_NAME}_RC_SRC ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.rc)

    CONFIGURE_FILE(
            ${CMAKE_CURRENT_SOURCE_DIR}/${PROJECT_NAME}.rc.in
            ${${PROJECT_NAME}_RC_SRC}
            @ONLY
    )

    ADD_CUSTOM_TARGET(
            ${PROJECT_NAME}_RCGEN
            COMMAND ${CMAKE_RC_COMPILER} /fo ${${PROJECT_NAME}_RC_RES} ${${PROJECT_NAME}_RC_SRC}
    )
ENDIF (WIN32)


SET(LINKED_LIST
        ${QT_LIBRARIES}
        ${SourceLibrary}
        )

IF (${Qt_Version} EQUAL "6")
    qt_add_resources(RESOURCE resources.qrc)
ELSE()
    qt5_add_resources(RESOURCE resources.qrc)
ENDIF()

ADD_SUBDIRECTORY(${SOURCE_DIR})
# ADD_SUBDIRECTORY(${BINARY_DIR})

## Include the Source Library
INCLUDE_DIRECTORIES(${SOURCE_DIR})

# targets
EXPORT(TARGETS ${LIBRARIES}
        FILE "${CMAKE_CURRENT_BINARY_DIR}/LibraryDepends.cmake")

if(${Qt_Version} GREATER_EQUAL 6)
    qt_add_executable(${CMAKE_PROJECT_NAME} main.cpp
        ${RESOURCE}
        ${${PROJECT_NAME}_RC_SRC}
        )
    # qt_create_translation(QM_FILES ${CMAKE_SOURCE_DIR})
else()
    add_executable(${CMAKE_PROJECT_NAME}
            ${RESOURCE}
            ${${PROJECT_NAME}_RC_SRC}
            )
    # qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR})
endif()


##
## Breeze Themes
##
INCLUDE(BreezeThemes)

TARGET_LINK_LIBRARIES(${PROJECT_NAME}
        PRIVATE
        # ${QT_LIBRARIES}
        ${LINKED_LIST}
        breeze_themes
        )

set_target_properties(${PROJECT_NAME} PROPERTIES
        MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
        MACOSX_BUNDLE_BUNDLE_VERSION ${${MODULE_NAME}_VERSION}
        MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
        MACOSX_BUNDLE TRUE
        WIN32_EXECUTABLE TRUE
        )


INSTALL(TARGETS ${PROJECT_NAME}
        BUNDLE DESTINATION .
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})


if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(${PROJECT_NAME})
endif()

TARGET_INCLUDE_DIRECTORIES(uTorrentSrc PRIVATE .)

SET_TARGET_PROPERTIES(${CMAKE_PROJECT_NAME} PROPERTIES LINKER_LANGUAGE CXX)
SET_TARGET_PROPERTIES(${CMAKE_PROJECT_NAME} PROPERTIES COMPILE_FLAGS "-DSORRYCLS_STATIC_DEFINE")

IF (UNIX)
    SET_TARGET_PROPERTIES(${PROJECT_NAME} PROPERTIES COMPILE_FLAGS "-DSORRYCLS_STATIC_DEFINE -DHAVE_CONFIG_H")
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