## =============================================================================
## =============================================================================
##              Test_Application
## =============================================================================
## =============================================================================

cmake_minimum_required (VERSION 2.8.9)
project (Test_Application)


# Setup basic variables.
set(APP_VERSION "1.0")
set(APP_AUTHOR "Christian Fischer")


FILE(GLOB HEADER_FILES   include/*.h*)
FILE(GLOB SOURCE_FILES   source/*.cpp)

IF (MSVC)
SOURCE_GROUP("Header Files" FILES    ${HEADER_FILES} )
SOURCE_GROUP("Source Files\\XML" FILES  ${XML_FILES} )
ENDIF(MSVC)

ADD_EXECUTABLE(
    Test_Application
    ${HEADER_FILES}
    ${SOURCE_FILES}
)

TARGET_LINK_LIBRARIES( Test_Application Filter_Library )

SET(CMAKE_CXX_FLAGS "-std=c++0x")