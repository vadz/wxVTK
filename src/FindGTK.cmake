#
# try to find GTK (and glib) and GTKGLArea
#

# GTK_INCLUDE_DIR   - Directories to include to use GTK
# GTK_LIBRARIES     - Files to link against to use GTK
# GTK_VERSION_2     - Use this Version
# GTK_VERSION_1     - Use this Version
# GTK_FOUND         - If false, don't try to use GTK

OPTION(GTK_VERSION_1    "Use GTK version 1.x"   1)
OPTION(GTK_VERSION_2    "Use GTK version 2.x"   2)

# Exclusion between the two version

IF(GTK_VERSION_2)
  SET(GTK_VERSION_1 0)
ENDIF(GTK_VERSION_2)

IF(WIN32)
  #win sucks !

ELSE(WIN32)

  #Find new pkg-config
  #FIND_PACKAGE(PKG) #don't know how to register !
  FIND_PROGRAM(CMAKE_PKG_CONFIG pkg-config ../gtk2/bin ../../gtk2/bin)

  #if PKG is found
  IF(CMAKE_PKG_CONFIG)

    SET(CMAKE_GTK1_CXX_FLAGS "`${CMAKE_PKG_CONFIG} --cflags gtk`")
    SET(GTK1_LIBRARIES "`${CMAKE_PKG_CONFIG} --libs gtk`")

    SET(CMAKE_GTK2_CXX_FLAGS "`${CMAKE_PKG_CONFIG} --cflags gtk+-2.0`")
    SET(GTK2_LIBRARIES "`${CMAKE_PKG_CONFIG} --libs gtk+-2.0`")
    
  ELSE(CMAKE_PKG_CONFIG)
    #we were not able to find to, lets try with:
    #old gtk-config
    FIND_PROGRAM(CMAKE_GTK_CONFIG gtk-config ../gtk/bin ../../gtk/bin)

    SET(CMAKE_GTK1_CXX_FLAGS "`${CMAKE_GTK_CONFIG} --cflags`")
    SET(GTK1_LIBRARIES "`${CMAKE_GTK_CONFIG} --libs`")

    MESSAGE(FATAL_ERROR "GTK2.x was not found but GTK1.x was, please set GTK_VERSION_1
    to ON")
  ENDIF(CMAKE_PKG_CONFIG)
  
ENDIF(WIN32)  

MARK_AS_ADVANCED(
  CMAKE_GTK_CXX_FLAGS
  CMAKE_GTK2_CXX_FLAGS
  GTK_INCLUDE_DIR
)

#IF(GTK_LIBRARIES OR GTK2_LIBRARIES)
  #only one gtk version was found !
  #default GTK to this one (even if it GTK2)
#  OPTION()
#ENDIF(GTK_LIBRARIES OR GTK2_LIBRARIES)

#IF(GTK_LIBRARIES)
#  IF((GTK_INCLUDE_DIR OR CMAKE_GTK_CXX_FLAGS) AND GTK_VERSION_2)
#    SET(GTK_FOUND 1)
#    SET (GTK_LIBRARIES ${GTK1_LIBRARIES})
#    SET(CMAKE_GTK_CXX_FLAGS ${CMAKE_GTK2_CXX_FLAGS})
#  ENDIF((GTK_INCLUDE_DIR OR CMAKE_GTK_CXX_FLAGS) AND GTK_VERSION_2)
#
#  IF(GTK2_INCLUDE_DIR OR CMAKE_GTK2_CXX_FLAGS)
#    SET(GTK2_FOUND 1)
#  ENDIF(GTK2_INCLUDE_DIR OR CMAKE_GTK2_CXX_FLAGS)
#  
#ENDIF(GTK_LIBRARIES)




IF(GTK1_LIBRARIES OR GTK2_LIBRARIES)

  IF(GTK_VERSION_2)
    #looking for GTK2.x
    IF(CMAKE_GTK2_CXX_FLAGS)
      SET(GTK_FOUND 1)
      SET (GTK_LIBRARIES ${GTK2_LIBRARIES})
      SET(CMAKE_GTK_CXX_FLAGS ${CMAKE_GTK2_CXX_FLAGS})
    ENDIF(CMAKE_GTK2_CXX_FLAGS)
  ELSE(GTK_VERSION_2)
    #looking for GTK1.x
    IF(CMAKE_GTK1_CXX_FLAGS)
      SET(GTK_FOUND 1)
      SET (GTK_LIBRARIES ${GTK1_LIBRARIES})
      SET(CMAKE_GTK_CXX_FLAGS ${CMAKE_GTK1_CXX_FLAGS})
    ENDIF(CMAKE_GTK1_CXX_FLAGS)
  ENDIF(GTK_VERSION_2)
ENDIF(GTK1_LIBRARIES OR GTK2_LIBRARIES)
