PROJECT (wxVTKSample)

#TODO: - try the MOTIF version
#      - try the MSW version
#      - try the Andy's version of wxWindow

FIND_PACKAGE(VTK)
IF (VTK_FOUND)
  INCLUDE(${VTK_USE_FILE})
ELSE(VTK_FOUND)
  MESSAGE(FATAL_ERROR "Cannot build without VTK.  Please set VTK_DIR.")
ENDIF (VTK_FOUND)

#FIND_PACKAGE(wxWindows)
#I don't like the way we should you use wxWindow+CMake
#thus I provide my own one
INCLUDE(${wxVTKSample_SOURCE_DIR}/src/FindwxWindows.cmake)
IF ( CMAKE_WX_CAN_COMPILE )

IF(WXWINDOWS_INCLUDE_PATH)
  INCLUDE_DIRECTORIES(${WXWINDOWS_INCLUDE_PATH})
#ELSE(WXWINDOWS_INCLUDE_PATH)
#  #We'll use CMakeWxWindows made by Andy...
#  FIND_PATH(WX_BINARY_PATH UseWX.cmake)
#  SET(USE_BUILT_WX 1)
ENDIF(WXWINDOWS_INCLUDE_PATH)


#Add our own include path
INCLUDE_DIRECTORIES(
${wxVTKSample_SOURCE_DIR}/src/
)

SET(SRCS_Sample
Sample/Sample.cpp
src/wxVTKRenderWindowInteractor.cxx
src/wxVTKRenderWindowInteractor.h
)

SET(SRCS_SplitSample
SplitSample/SplitSample.cpp
src/wxVTKRenderWindowInteractor.cxx
src/wxVTKRenderWindowInteractor.h
)

LINK_LIBRARIES(
  vtkCommon
  vtkGraphics
  vtkRendering
)

IF(WIN32)

  ADD_EXECUTABLE(Sample_msw WIN32 ${SRCS_Sample})
  TARGET_LINK_LIBRARIES(Sample_msw ${WXWINDOWS_LIBRARY})

  ADD_EXECUTABLE(SplitSample_msw WIN32 ${SRCS_SplitSample})
  TARGET_LINK_LIBRARIES(SplitSample_msw ${WXWINDOWS_LIBRARY})

ELSE(WIN32)

  #FIND_PACKAGE(GTK)
  #I don't like also the way CMake search GTK
  #provide my own:
  INCLUDE(${wxVTKSample_SOURCE_DIR}/src/FindGTK.cmake)
  IF(GTK_FOUND)
    SET(CMAKE_CXX_FLAGS "${CMAKE_GTK_CXX_FLAGS} ${CMAKE_CXX_FLAGS}")
  ENDIF(GTK_FOUND)

  IF(CMAKE_WX_CXX_FLAGS)
    SET(CMAKE_CXX_FLAGS "${CMAKE_WX_CXX_FLAGS} ${CMAKE_CXX_FLAGS}")
  ENDIF(CMAKE_WX_CXX_FLAGS)

  ADD_EXECUTABLE(Sample_gtk ${SRCS_Sample})
  TARGET_LINK_LIBRARIES(Sample_gtk ${WXWINDOWS_LIBRARY} ${GTK_LIBRARIES})

  ADD_EXECUTABLE(SplitSample_gtk ${SRCS_SplitSample})
  TARGET_LINK_LIBRARIES(SplitSample_gtk ${WXWINDOWS_LIBRARY} ${GTK_LIBRARIES})

ENDIF(WIN32)

ELSE ( CMAKE_WX_CAN_COMPILE )

  MESSAGE(FATAL_ERROR "Cannot find wxWindows libraries and/or header files. Please set VTK_DIR.")

ENDIF ( CMAKE_WX_CAN_COMPILE )
