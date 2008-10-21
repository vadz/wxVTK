PROJECT (wxVTKSample)
# Well technicall you also need:
# http://cmake.org/Bug/bug.php?op=show&bugid=3582
CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

if(COMMAND cmake_policy)
cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)


#TODO: - try the MOTIF version
#      - try the MAC version (done !)
#      - try Andy's version of wxWindow (dropped, wx wants to use bakefile)

#-----------------------------------------------------------------------------
FIND_PACKAGE(VTK REQUIRED)
INCLUDE(${VTK_USE_FILE})

IF(NOT VTK_USE_HYBRID)
  MESSAGE(FATAL_ERROR "You need to turn VTK_USE_HYBRID:ON (for vtkImagePlaneWidget.h)")
ENDIF(NOT VTK_USE_HYBRID)

#-----------------------------------------------------------------------------
SET (EXECUTABLE_OUTPUT_PATH ${wxVTKSample_BINARY_DIR}/bin CACHE PATH "Single output directory for building all executables.")
SET (LIBRARY_OUTPUT_PATH ${wxVTKSample_BINARY_DIR}/bin CACHE PATH "Single output directory for building all libraries.")
MARK_AS_ADVANCED(LIBRARY_OUTPUT_PATH EXECUTABLE_OUTPUT_PATH)

#-----------------------------------------------------------------------------

# The following allows you to access wxGLCanvas for GTK
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
    # CMake 2.6:
    # technically those packages are not required since one can still use the Motif/X11 version and not the gtk one:
    FIND_PACKAGE(PkgConfig)
    pkg_check_modules (GTK2 gtk+-2.0)
    #MESSAGE("${GTK2_INCLUDE_DIRS}")
    INCLUDE_DIRECTORIES(${GTK2_INCLUDE_DIRS})
    LINK_LIBRARIES(${GTK2_LIBRARIES})
    # Can I require all my user to have the gl lib on linux, even if they do not really need it...
    SET(WXGLCANVASLIBS "gl")
  ENDIF(APPLE)
ENDIF(WIN32)

# wxWidgets is required to build the project
# For GTK we need a couple of stuff:
# gl: GLCanvas
# adv: wxSashLayoutWindow and such...
FIND_PACKAGE(wxWidgets COMPONENTS base core adv ${WXGLCANVASLIBS})

# wx is a pain...
# if you include glcanvas.h it include GL/glu.h ... sigh I have to install glu on my linux box:
# sudo apt-get install libglu1-mesa-dev
# http://bugs.debian.org/cgi-bin/bugreport.cgi?bug=493713
IF(wxWidgets_FOUND)
  INCLUDE( ${wxWidgets_USE_FILE} )

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

SET(SRCS_VTKNotebook
  Sample/wxVTKNotebook.cxx
  src/wxVTKRenderWindowInteractor.cxx
  )

IF(NOT VTK_USE_RENDERING)
  MESSAGE(FATAL_ERROR "Cannot build wxVTK without vtkRendering (vtkWidgets)")
ENDIF(NOT VTK_USE_RENDERING)
LINK_LIBRARIES(
  vtkRendering
  ${wxWidgets_LIBRARIES}
  )
IF( "${VTK_MAJOR_VERSION}" STREQUAL "5")
  LINK_LIBRARIES( vtkWidgets )
ENDIF( "${VTK_MAJOR_VERSION}" STREQUAL "5")


ADD_EXECUTABLE(wxSample ${GUI_EXECUTABLE} ${SRCS_Sample})
ADD_EXECUTABLE(wxSplitSample ${GUI_EXECUTABLE} ${SRCS_SplitSample})

ADD_EXECUTABLE(wxImagePlaneWidget ${GUI_EXECUTABLE} ${SRCS_ImagePlaneWidget})
ADD_EXECUTABLE(wxImageViewer ${GUI_EXECUTABLE} ${SRCS_ImageViewer})
ADD_EXECUTABLE(wxMedical3 ${GUI_EXECUTABLE} ${SRCS_Medical3})
ADD_EXECUTABLE(wxVTKNotebook ${GUI_EXECUTABLE} ${SRCS_VTKNotebook})

  INSTALL(TARGETS
        wxSplitSample wxSample wxImagePlaneWidget wxImageViewer wxMedical3
	RUNTIME DESTINATION bin
	BUNDLE DESTINATION bin
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib/static
	)


ELSE(wxWidgets_FOUND)

  MESSAGE("Cannot find wxWidgets libraries and/or header files")

ENDIF(wxWidgets_FOUND)
