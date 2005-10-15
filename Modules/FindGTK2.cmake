#
# try to find GTK (and glib) and GTKGLArea
#

# GTK_INCLUDE_DIR   - Directories to include to use GTK
# GTK_LIBRARIES     - Files to link against to use GTK
# GTK_FOUND         - If false, don't try to use GTK

IF(WIN32)
  #win sucks !

ELSE(WIN32)

  #Find new pkg-config
  FIND_PROGRAM(CMAKE_PKG_CONFIG pkg-config ../gtk2/bin ../../gtk2/bin)

  #if PKG is found
  IF(CMAKE_PKG_CONFIG)
    SET(CMAKE_GTK2_CXX_FLAGS "`${CMAKE_PKG_CONFIG} --cflags gtk+-2.0`")
    SET(GTK2_LIBRARIES "`${CMAKE_PKG_CONFIG} --libs gtk+-2.0`")
  ENDIF(CMAKE_PKG_CONFIG)
  
ENDIF(WIN32)  

MARK_AS_ADVANCED(
  CMAKE_GTK_CXX_FLAGS
  GTK_INCLUDE_DIR
)

