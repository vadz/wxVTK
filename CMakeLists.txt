PROJECT (wxVTKSample)

#TODO: - try the MOTIF version
#      - try the MAC version
#      - try Andy's version of wxWindow

FIND_PACKAGE(VTK)
IF (VTK_FOUND)
  INCLUDE(${VTK_USE_FILE})
ELSE(VTK_FOUND)
  MESSAGE(FATAL_ERROR
     "Cannot build without VTK.  Please set VTK_DIR.")
ENDIF (VTK_FOUND)

SET(WXWINDOWS_USE_GL 1) 
#When http://www.cmake.org/pipermail/cmake/2003-September/004263.html
#will be solved VTK + wxWindows would hopefully fully collaborate  !
FIND_PACKAGE(wxWindows)
#INCLUDE(${wxVTKSample_SOURCE_DIR}/Modules/FindwxWindows.cmake)
IF ( CMAKE_WX_CAN_COMPILE )

IF(WXWINDOWS_INCLUDE_DIR)
  INCLUDE_DIRECTORIES(${WXWINDOWS_INCLUDE_DIR})
#ELSE(WXWINDOWS_INCLUDE_DIR)
#  #We'll use CMakeWxWindows made by Andy...
#  FIND_PATH(WX_BINARY_PATH UseWX.cmake)
#  SET(USE_BUILT_WX 1)
ENDIF(WXWINDOWS_INCLUDE_DIR)

#include stuff in all cases
#IF(WX_BINARY_PATH)
#  INCLUDE(${WX_BINARY_PATH}/UseWX.cmake)
#ENDIF(WX_BINARY_PATH)


#Add our own include path
INCLUDE_DIRECTORIES(
${wxVTKSample_SOURCE_DIR}/src/
)

IF(WIN32)
  SET(WIN32_EXECUTABLE WIN32)
ENDIF(WIN32)
IF(VTK_USE_X)
  FIND_PACKAGE(GTK)
  #I don't like also the way CMake search GTK
  #provide my own:
  #INCLUDE(${wxVTKSample_SOURCE_DIR}/Modules/FindGTK.cmake)
  IF(GTK_FOUND)
    SET(CMAKE_CXX_FLAGS "${CMAKE_GTK_CXX_FLAGS} ${CMAKE_CXX_FLAGS}")
  ENDIF(GTK_FOUND)

  IF(CMAKE_WX_CXX_FLAGS)
    SET(CMAKE_CXX_FLAGS "${CMAKE_WX_CXX_FLAGS} ${CMAKE_CXX_FLAGS}")
  ENDIF(CMAKE_WX_CXX_FLAGS)
ENDIF(VTK_USE_X)

SET(SRCS_Sample
Sample/Sample.cpp
src/wxVTKRenderWindowInteractor.cxx
)

SET(SRCS_SplitSample
Sample/SplitSample.cpp
src/wxVTKRenderWindowInteractor.cxx
)

SET(SRCS_ImagePlaneWidget
Sample/wxImagePlaneWidget.cxx
src/wxVTKRenderWindowInteractor.cxx
)

SET(SRCS_ImageViewer
Sample/wxImageViewer.cxx
src/wxVTKRenderWindowInteractor.cxx
)

SET(SRCS_Medical3
Sample/wxMedical3.cxx
src/wxVTKRenderWindowInteractor.cxx
)

LINK_LIBRARIES(
  vtkIO
  vtkHybrid
  vtkCommon
  vtkGraphics
  vtkRendering
  ${WXWINDOWS_LIBRARY}
)

ADD_EXECUTABLE(Sample ${WIN32_EXECUTABLE} ${SRCS_Sample})
ADD_EXECUTABLE(SplitSample ${WIN32_EXECUTABLE} ${SRCS_SplitSample})

ADD_EXECUTABLE(wxImagePlaneWidget ${WIN32_EXECUTABLE} ${SRCS_ImagePlaneWidget})
ADD_EXECUTABLE(wxImageViewer ${WIN32_EXECUTABLE} ${SRCS_ImageViewer})
ADD_EXECUTABLE(wxMedical3 ${WIN32_EXECUTABLE} ${SRCS_Medical3})

ELSE ( CMAKE_WX_CAN_COMPILE )

  MESSAGE("Cannot find wxWindows libraries and/or header files")

ENDIF ( CMAKE_WX_CAN_COMPILE )
