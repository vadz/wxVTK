#
# try to find GTK (and glib) and GTKGLArea
#

# GTK_INCLUDE_DIR   - Directories to include to use GTK
# GTK_LIBRARIES     - Files to link against to use GTK
# GTK_FOUND         - If false, don't try to use GTK

IF(WIN32)
  #win sucks !

ELSE(WIN32)

  INCLUDE( "${CMAKE_CURRENT_SOURCE_DIR}/Modules/UseConfig.cmake")
  #Find new pkg-config
  #  FIND_PROGRAM(CMAKE_PKG_CONFIG pkg-config 
  #    ../gtk2/bin 
  #    ../../gtk2/bin
  #    )

  #if PKG is found
  IF(CMAKE_PKG_CONFIG)

    #SET(CMAKE_GTK1_CXX_FLAGS "`${CMAKE_PKG_CONFIG} --cflags gtk`")
    #SET(GTK1_LIBRARIES "`${CMAKE_PKG_CONFIG} --libs gtk`")
    MESSAGE("${CONFIG_INCLUDE_PATH}")
    FIND_LIBPATH( ${CMAKE_PKG_CONFIG}  "--libs gtk+")
    FIND_INCLUDEPATH(${CMAKE_WX_CONFIG} "--cflags gtk+")
    MESSAGE("${CONFIG_INCLUDE_PATH}")
    #MESSAGE("${CONFIG_LIBRARIES}")
    SET(GTK_INCLUDE_PATH ${CONFIG_INCLUDE_PATH})
    SET(GTK_LIBRARIES ${CONFIG_LIBRARIES})

  ELSE(CMAKE_PKG_CONFIG)
    #we were not able to find to, lets try with:
    #old gtk-config
    FIND_PROGRAM(CMAKE_GTK_CONFIG gtk-config ../gtk/bin ../../gtk/bin)

    #SET(CMAKE_GTK1_CXX_FLAGS "`${CMAKE_GTK_CONFIG} --cflags`")
    #SET(GTK1_LIBRARIES "`${CMAKE_GTK_CONFIG} --libs`")
    FIND_LIBPATH( ${CMAKE_GTK_CONFIG}  "--libs")
    FIND_INCLUDEPATH(${CMAKE_GTK_CONFIG} "--cflags")
    SET(GTK_INCLUDE_PATH ${CONFIG_INCLUDE_PATH})
    SET(GTK_LIBRARIES ${CONFIG_LIBRARIES})

  ENDIF(CMAKE_PKG_CONFIG)
  
ENDIF(WIN32)  

MARK_AS_ADVANCED(
  CMAKE_PKG_CONFIG
  CMAKE_GTK_CONFIG
)


