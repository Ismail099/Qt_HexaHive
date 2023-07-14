# Get Python to compile the stylesheets.
# Fetch the repository, configure, compile the stylesheets.

FIND_PACKAGE(Python COMPONENTS Interpreter)

INCLUDE(FetchContent)

SET(FETCHCONTENT_QUIET
        OFF
        CACHE BOOL "Silence fetch content" FORCE)

FETCHCONTENT_DECLARE(
        breeze_stylesheets
        GIT_REPOSITORY https://github.com/Alexhuszagh/BreezeStyleSheets.git
        GIT_TAG origin/main
        GIT_PROGRESS ON
        USES_TERMINAL_DOWNLOAD TRUE
)
FETCHCONTENT_GETPROPERTIES(breeze_stylesheets)
IF (NOT breeze_stylesheets_POPULATED)
    FETCHCONTENT_POPULATE(breeze_stylesheets)

    ADD_LIBRARY(breeze_themes STATIC "${breeze_stylesheets_SOURCE_DIR/dist/qrc/breeze.qrc}")

    ADD_CUSTOM_TARGET(
            run_python_breeze ALL
            COMMAND ${Pyhon_EXECUTABLE} configure.py --extensions=<EXTENSIONS>
                    --stylesheet=<STYLES> --resource breeze.qrc
            WORKING_DIRECTORY "{breeze_stylesheets_SOURCE_DIR}"
            BYPRODUCTS "${breeze_stylesheets_SOURCE_DIR}/dist/qrc/breeze.qrc"
            COMMENT "Generating themes"
    )

    ADD_DEPENDENCIES(breeze_themes run_python_breeze)
ENDIF()