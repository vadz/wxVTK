PROJECT (wxVTKSample)

#TODO: - try the MOTIF version
#      - try the MSW version

OPTION(VTK_WRAP_PYTHON "Wrap VTK classes into the Python language." OFF)

INCLUDE (${CMAKE_ROOT}/Modules/FindVTK.cmake)
IF (USE_VTK_FILE)
  INCLUDE(${USE_VTK_FILE})
ENDIF (USE_VTK_FILE)

INCLUDE (${CMAKE_ROOT}/Modules/FindwxWindows.cmake)
IF(WXWINDOWS_INCLUDE_PATH)
  INCLUDE_DIRECTORIES(${WXWINDOWS_INCLUDE_PATH})
ENDIF(WXWINDOWS_INCLUDE_PATH)
IF(CMAKE_WX_CXX_FLAGS)
  SET(CMAKE_CXX_FLAGS "${CMAKE_WX_CXX_FLAGS} ${CMAKE_CXX_FLAGS}")
ENDIF(CMAKE_WX_CXX_FLAGS)

INCLUDE (${CMAKE_ROOT}/Modules/FindGTK.cmake)
IF(GTK_INCLUDE_DIR)
  INCLUDE_DIRECTORIES(${GTK_INCLUDE_DIR})
ENDIF(GTK_INCLUDE_DIR)

LINK_LIBRARIES(
  vtkRendering
  vtkGraphics
  vtkImaging
  vtkIO
  vtkFiltering
  vtkCommon
)

SET(SRCS_Sample
Sample/Sample.cpp
wxVTKRenderWindowInteractor.cxx
)

SET(SRCS_SplitSample
SplitSample/SplitSample.cpp
wxVTKRenderWindowInteractor.cxx
)

ADD_EXECUTABLE(Sample_gtk ${SRCS_Sample})
TARGET_LINK_LIBRARIES(Sample_gtk ${WXWINDOWS_LIBRARY} ${GTK_LIBRARIES})

ADD_EXECUTABLE(SplitSample_gtk ${SRCS_SplitSample})
TARGET_LINK_LIBRARIES(SplitSample_gtk ${WXWINDOWS_LIBRARY} ${GTK_LIBRARIES})

# Choose static or shared libraries.
OPTION(BUILD_SHARED_LIBS "Build VTK with shared libraries." OFF)
SET(VTK_BUILD_SHARED_LIBS ${BUILD_SHARED_LIBS})




#IF (VTK_WRAP_PYTHON)
#  VTK_WRAP_PYTHON2(vtk${KIT}Python KitPython_SRCS ${Kit_SRCS})
#  ADD_LIBRARY(vtk${KIT}Python MODULE ${KitPython_SRCS} ${Kit_PYTHON_EXTRA_SRCS})
#  IF(NOT APPLE)
#    TARGET_LINK_LIBRARIES (vtk${KIT}Python ${KIT_PYTHON_LIBS})
#  ENDIF(NOT APPLE)
#  IF(WIN32)
#    TARGET_LINK_LIBRARIES (vtk${KIT}Python
#                           debug ${PYTHON_DEBUG_LIBRARY}
#                           optimized ${PYTHON_LIBRARY})
#  ELSE(WIN32)
#    TARGET_LINK_LIBRARIES (vtk${KIT}Python ${PYTHON_LIBRARY})
#  ENDIF(WIN32)
#  TARGET_LINK_LIBRARIES(vtk${KIT}Python vtk${KIT})
#
#  INSTALL_TARGETS(/lib/vtk vtk${KIT}Python)
#ENDIF (VTK_WRAP_PYTHON)
#
#
#ADD_LIBRARY(RenderWindowInteractor wxVTKRenderWindowInteractor.cxx)
