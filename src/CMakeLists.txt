SET(SOURCE_FILES
        MetaInformation.cpp
        BencodeParser.cpp BencodeParser.h)

SET(HEADER_FILES
        MetaInformation.h
        BencodeParser.cpp BencodeParser.h)

##
## Shared Library
##
ADD_LIBRARY(SOURCE_LIBRARY
        ${SOURCE_FILES}
        ${HEADER_FILES}
        )

## Declaration of a public API for the library
#SET_TARGET_PROPERTIES(TOOLS_LIBRARY
#PROPERTIES
#PUBLIC_HEADER ${TOOLS_HEADER} )

SET_TARGET_PROPERTIES(SOURCE_LIBRARY
        PROPERTIES
        LINKER_LANGUAGE CXX
        )

SET(LINK_LIBRARIES ${QT_LIBRARIES})
TARGET_LINK_LIBRARIES(SOURCE_LIBRARY ${LINK_LIBRARIES})