SET(SourceLibrary uTorrentSrc)

SET(SOURCE_FILES
        MetaInformation.cpp
        BencodeParser.cpp
        Exceptions/UnImplementedException.cpp
        PeerWireClient.cpp
        AddTorrentForm.cpp
        )

SET(HEADER_FILES
        MetaInformation.h
        BencodeParser.h
        Exceptions/UnImplementedException.hpp
        PeerWireClient.hpp
        AddTorrentForm.hpp
        AddTorrentForm.ui)

##
## Shared Library
##
ADD_LIBRARY(${SourceLibrary}
        ${SOURCE_FILES}
        ${HEADER_FILES}
        )

## Declaration of a public API for the library
#SET_TARGET_PROPERTIES(TOOLS_LIBRARY
#PROPERTIES
#PUBLIC_HEADER ${TOOLS_HEADER} )

SET_TARGET_PROPERTIES(${SourceLibrary}
        PROPERTIES
        LINKER_LANGUAGE CXX
        )
SET_TARGET_PROPERTIES(${SourceLibrary}
        PROPERTIES
        COMPILE_FLAGS "-DSORRYCLS_STATIC_DEFINE"
        )

IF (UNIX)
    SET_TARGET_PROPERTIES(${SourceLibrary}
            PROPERTIES
            COMPILE_FALGS "-DSORRYCLS_STATIC_DEFINE -DHAVE_CONFIG_H")
ENDIF ()


SET(LINK_LIBRARIES ${QT_LIBRARIES})
TARGET_LINK_LIBRARIES(${SourceLibrary} ${LINK_LIBRARIES})