if(DEFINED INCLUDED_SETUP_POCO_CMAKE)
    return()
endif()
set(INCLUDED_SETUP_POCO_CMAKE TRUE)

########################################################################
# Find Poco with PocoConfig.cmake (installed copy)
########################################################################
if("${CMAKE_VERSION}" VERSION_LESS "3.0.0")
else()
    set(Poco_INCLUDE_DIRS "")
    find_package(Poco CONFIG COMPONENTS Foundation Util JSON XML Net)
    if (Poco_FOUND)
        return()
    endif()
endif()

########################################################################
# Find Poco with FindPoco.cmake (local copy)
########################################################################
message(STATUS "PocoConfig not found - trying with FindPoco")
list(INSERT CMAKE_MODULE_PATH 0 ${CMAKE_CURRENT_LIST_DIR})
find_package(Poco)
if (POCO_FOUND AND POCO_Util_FOUND AND POCO_Net_FOUND AND POCO_XML_FOUND AND POCO_JSON_FOUND)
    ########################################################################
    # Check version >= 1.5.4
    ########################################################################
    FILE (READ "${POCO_INCLUDE_DIRS}/Poco/Version.h" Poco_VERSION_H)
    set(_ws "[ \r\t\n^$]+")
    STRING (REGEX REPLACE "^.*#define${_ws}POCO_VERSION${_ws}(0x[0-9a-fA-F]*)${_ws}.*$" "\\1" Poco_VERSION "${Poco_VERSION_H}")
    set(_ws)
    if ("${Poco_VERSION}" LESS 0x01050400)
        return()
    endif()

    ########################################################################
    # Set variables
    ########################################################################
    set(Poco_FOUND 1)
    set(Poco_INCLUDE_DIRS ${POCO_INCLUDE_DIRS})
    set(Poco_LIBRARIES
        ${POCO_LIBRARIES}
        ${POCO_Util_LIBRARIES}
        ${POCO_Net_LIBRARIES}
        ${POCO_XML_LIBRARIES}
        ${POCO_JSON_LIBRARIES}
    )

    ########################################################################
    # Link with the thread library
    ########################################################################
    if (UNIX)
        set(CMAKE_THREAD_PREFER_PTHREAD ON)
        enable_language(C)
        find_package(Threads)
        list(APPEND Poco_LIBRARIES ${CMAKE_THREAD_LIBS_INIT})
    endif()
endif()
