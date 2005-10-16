PROJECT (wxVTKSample)

#TODO: - try the MOTIF version
#      - try the MAC version
#      - try Andy's version of wxWindow

#-----------------------------------------------------------------------------
FIND_PACKAGE(VTK)
IF (VTK_FOUND)
  INCLUDE(${VTK_USE_FILE})
ELSE(VTK_FOUND)
  MESSAGE(FATAL_ERROR
     "Cannot build without VTK.  Please set VTK_DIR.")
ENDIF (VTK_FOUND)

#-----------------------------------------------------------------------------
SET (EXECUTABLE_OUTPUT_PATH ${wxVTKSample_BINARY_DIR}/bin CACHE PATH "Single output directory for building all executables.")
SET (LIBRARY_OUTPUT_PATH ${wxVTKSample_BINARY_DIR}/bin CACHE PATH "Single output directory for building all libraries.")
MARK_AS_ADVANCED(LIBRARY_OUTPUT_PATH EXECUTABLE_OUTPUT_PATH)

#-----------------------------------------------------------------------------
#-----------------------------------------------------------------------------

IF(WIN32)
  SET(GUI_EXECUTABLE WIN32)
ELSE(WIN32)
  IF(APPLE)
  SET(GUI_EXECUTABLE MACOSX_BUNDLE)
  IF(VTK_USE_COCOA)
    SET_SOURCE_FILES_PROPERTIES(
      src/wxVTKRenderWindowInteractor.cxx
      PROPERTIES COMPILE_FLAGS "-ObjC++")
  ENDIF(VTK_USE_COCOA)
  ELSE(APPLE)
    # Ok X11 for sure, but just check:
    IF(NOT VTK_USE_X)
      MESSAGE(FATAL_ERROR "You need to have VTK_USE_X")
    ENDIF(NOT VTK_USE_X)
    OPTION(GTK_VERSION_1    "Use GTK version 1.x"   OFF)
    OPTION(GTK_VERSION_2    "Use GTK version 2.x"   OFF)
    # Exclusion between the two version
    IF(GTK_VERSION_1 AND GTK_VERSION_2)
      MESSAGE(FATAL_ERROR "Cannot select both version")
    ENDIF(GTK_VERSION_1 AND GTK_VERSION_2)
    IF(NOT GTK_VERSION_1 AND NOT GTK_VERSION_2)
      MESSAGE(FATAL_ERROR "Have to select at least one version")
    ENDIF(NOT GTK_VERSION_1 AND NOT GTK_VERSION_2)

    IF(GTK_VERSION_1)
      INCLUDE(${wxVTKSample_SOURCE_DIR}/Modules/FindGTK1.cmake)
    ENDIF(GTK_VERSION_1)
    IF(GTK_VERSION_2)
      INCLUDE(${wxVTKSample_SOURCE_DIR}/Modules/FindGTK2.cmake)
    ENDIF(GTK_VERSION_2)

    # Now special trick for opengl/wx 
    SET(WX_USE_GL 1)
  ENDIF(APPLE)
ENDIF(WIN32)

INCLUDE(${wxVTKSample_SOURCE_DIR}/Modules/FindwxWidgets.cmake)

IF ( CMAKE_WX_CAN_COMPILE )

  ADD_DEFINITIONS( ${GTK_DEFINITIONS} )
  ADD_DEFINITIONS( ${WX_DEFINITIONS} )
  INCLUDE_DIRECTORIES(${WX_INCLUDE_PATH})
  INCLUDE_DIRECTORIES(${GTK_INCLUDE_PATH})

  #Add our own include path
  INCLUDE_DIRECTORIES(
    ${wxVTKSample_SOURCE_DIR}/src
    )


#-----------------------------------------------------------------------------
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

IF(NOT VTK_USE_RENDERING)
  MESSAGE(FATAL_ERROR "Cannot build wxVTK without vtkRendering (vtkWidgets)")
ENDIF(NOT VTK_USE_RENDERING)
LINK_LIBRARIES(
  vtkIO
  vtkHybrid
  vtkCommon
  vtkGraphics
  vtkRendering
  ${WX_LIBRARIES}
  ${GTK_LIBRARIES}
  )
IF( "${VTK_MAJOR_VERSION}" STREQUAL "5")
LINK_LIBRARIES( vtkWidgets )
ENDIF( "${VTK_MAJOR_VERSION}" STREQUAL "5")


ADD_EXECUTABLE(wxSample ${GUI_EXECUTABLE} ${SRCS_Sample})
ADD_EXECUTABLE(wxSplitSample ${GUI_EXECUTABLE} ${SRCS_SplitSample})

ADD_EXECUTABLE(wxImagePlaneWidget ${GUI_EXECUTABLE} ${SRCS_ImagePlaneWidget})
ADD_EXECUTABLE(wxImageViewer ${GUI_EXECUTABLE} ${SRCS_ImageViewer})
ADD_EXECUTABLE(wxMedical3 ${GUI_EXECUTABLE} ${SRCS_Medical3})

ELSE ( CMAKE_WX_CAN_COMPILE )

  MESSAGE("Cannot find wxWindows libraries and/or header files")

ENDIF ( CMAKE_WX_CAN_COMPILE )
