# FIND_PACKAGE(Qt5Core REQUIRED)
# GET_TARGET_PROPERTY(_qmake_executable Qt5::qmake IMPORTED_LOCATION)
# GET_FILENAME_COMPONENT(_qt_bin_dir "${_qmake_executable}" DIRECTORY)
SET(_qt_bin_dir "${CMAKE_PREFIX_PATH}/bin")

MESSAGE(STATUS "WinQtDeploy_Path: ${_qt_bin_dir}")

FUNCTION(WinQtDeploy target)

    SET(_build_type "")
    IF (CMAKE_BUILD_TYPE STREQUAL "Debug")
        SET(_build_type "--debug")
    ELSEIF (CMAKE_BUILD_TYPE STREQUAL "Release")
        SET(_build_type "--release")
    ENDIF ()
    MESSAGE(STATUS "Build Type: ${_build_type}")
    ADD_CUSTOM_COMMAND(TARGET ${target} POST_BUILD
            COMMAND "${_qt_bin_dir}/windeployqt.exe"
            --verbose 1
            ${_build_type}
            \"$<TARGET_FILE:${target}>\"
            COMMENT "Deploying Qt Libraries using WinDeployQt for Compilation Target '${target}' ...")

    IF (${CMAKE_CXX_COMPILER} STREQUAL "GNU")
        ADD_CUSTOM_COMMAND(
                TARGET ${target} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_if_different ${_qt_bin_dir}/libstdc++-6.dll $<TARGET_FILE_DIR:${target}>
                COMMAND ${CMAKE_COMMAND} -E copy_if_different ${_qt_bin_dir}/libwinpthread-1.dll $<TARGET_FILE_DIR:${target}>
        )
    ENDIF ()

ENDFUNCTION()
