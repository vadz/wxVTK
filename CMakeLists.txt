PROJECT (wxVTKSample)
CMAKE_MINIMUM_REQUIRED(VERSION 2.4)

#TODO: - try the MOTIF version
#      - try the MAC version (done !)
#      - try Andy's version of wxWindow (dropped, wx wants to use bakefile)

#-----------------------------------------------------------------------------
FIND_PACKAGE(VTK REQUIRED)
INCLUDE(${VTK_USE_FILE})

#-----------------------------------------------------------------------------
SET (EXECUTABLE_OUTPUT_PATH ${wxVTKSample_BINARY_DIR}/bin CACHE PATH "Single output directory for building all executables.")
SET (LIBRARY_OUTPUT_PATH ${wxVTKSample_BINARY_DIR}/bin CACHE PATH "Single output directory for building all libraries.")
MARK_AS_ADVANCED(LIBRARY_OUTPUT_PATH EXECUTABLE_OUTPUT_PATH)

#-----------------------------------------------------------------------------

# The following allows you to access wxGLCanvas for GTK
IF(UNIX)
  SET(WXWINDOWS_USE_GL 1)
ENDIF(UNIX)

# wxWidgets is required to build the project
FIND_PACKAGE(wxWidgets REQUIRED)

IF(WXWINDOWS_FOUND)
  INCLUDE( UsewxWidgets )

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
  vtkRendering
  ${WX_LIBRARIES}
  )
IF( "${VTK_MAJOR_VERSION}" STREQUAL "5")
  LINK_LIBRARIES( vtkWidgets )
ENDIF( "${VTK_MAJOR_VERSION}" STREQUAL "5")


ADD_EXECUTABLE(wxSample ${GUI_EXECUTABLE} ${SRCS_Sample})
ADD_EXECUTABLE(wxSplitSample ${GUI_EXECUTABLE} ${SRCS_SplitSample})

ADD_EXECUTABLE(wxImagePlaneWidget ${GUI_EXECUTABLE} ${SRCS_ImagePlaneWidget})
ADD_EXECUTABLE(wxImageViewer ${GUI_EXECUTABLE} ${SRCS_ImageViewer})
ADD_EXECUTABLE(wxMedical3 ${GUI_EXECUTABLE} ${SRCS_Medical3})

ELSE(WXWINDOWS_FOUND)

  MESSAGE("Cannot find wxWindows libraries and/or header files")

ENDIF(WXWINDOWS_FOUND)
