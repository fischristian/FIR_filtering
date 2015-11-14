## =============================================================================
## =============================================================================
##              Filter_Library
## =============================================================================
## =============================================================================

cmake_minimum_required (VERSION 2.8.9)
project (Filter_Library)

# Setup basic variables.
set(APP_VERSION "1.0")
set(APP_AUTHOR "Christian Fischer")


FILE(GLOB HEADER_FILES   include/*.h*)
FILE(GLOB SOURCE_FILES   source/*.cpp)

IF (MSVC)
SOURCE_GROUP("Header Files" FILES    ${HEADER_FILES} )
SOURCE_GROUP("Source Files\\XML" FILES  ${XML_FILES} )
ENDIF(MSVC)


ADD_LIBRARY(
    Filter_Library
    SHARED
    ${HEADER_FILES}
    ${SOURCE_FILES}
)

