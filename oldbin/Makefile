# linux info
CC = g++
LIB_APPEND =
WXCONFIG = /opt/wxgtk240/bin/wx-config

WX_USE      = gtk
WX_CFLAGS   = `$(WXCONFIG) --cflags`
WX_CPPFLAGS = `$(WXCONFIG) --cppflags` `gtk-config --cflags`
WX_LDFLAGS  = 
WX_LDLIBS   = `$(WXCONFIG) --libs --gl-libs`

VTK_CFLAGS   = -I/opt/vtk32/include
VTK_CPPFLAGS = -I/opt/vtk32/include
VTK_LDFLAGS  = -L/opt/vtk32/lib$(LIB_APPEND)
VTK_LDLIBS   = -lVTKCommon -lVTKGraphics -lVTKImaging

APP_CFLAGS   = -g -Wall -I. -I../ -I/usr/X11R6/include
APP_CPPFLAGS = -g -Wall -I. -I../
APP_LDFLAGS  = -L/usr/X11R6/lib$(LIB_APPEND)
APP_LDLIBS   = -lGL -lGLU -lXm -lXpm  -lXmu -lXext -lXt -lX11 -ldl -lpthread -lm

CFLAGS   = $(APP_CFLAGS)   $(WX_CFLAGS)   $(VTK_CFLAGS)
CPPFLAGS = $(APP_CPPFLAGS) $(WX_CPPFLAGS) $(VTK_CPPFLAGS)
LDFLAGS  = $(WX_LDFLAGS)  $(APP_LDFLAGS)  $(VTK_LDFLAGS)
LDLIBS   = $(WX_LDLIBS)   $(VTK_LDLIBS)   $(APP_LDLIBS)

Sample_$(WX_USE): Sample_$(WX_USE).o ../wxVTKRenderWindowInteractor_$(WX_USE).o
	$(CC) -o Sample_$(WX_USE) Sample_$(WX_USE).o ../wxVTKRenderWindowInteractor_$(WX_USE).o $(LDFLAGS) $(LDLIBS)

Sample_$(WX_USE).o: ../Sample/Sample.cpp ../wxVTKRenderWindowInteractor.h
	$(CC) -c $(CPPFLAGS) -o Sample_$(WX_USE).o ../Sample/Sample.cpp

../wxVTKRenderWindowInteractor_$(WX_USE).o: ../wxVTKRenderWindowInteractor.cxx ../wxVTKRenderWindowInteractor.h
	$(CC) -c $(CPPFLAGS) -o ../wxVTKRenderWindowInteractor_$(WX_USE).o ../wxVTKRenderWindowInteractor.cxx

clean:
	rm *$(WX_USE).o ../../*$(WX_USE).o Sample_$(WX_USE)
