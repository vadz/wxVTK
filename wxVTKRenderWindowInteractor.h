/*=========================================================================

  Program:   Visualization Toolkit
  Module:    $RCSfile$
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 1993-2002 Ken Martin, Will Schroeder, Bill Lorensen 
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/

// .NAME  wxVTKRenderWindowInteractor - class to enable VTK to render to 
// and interact with wxWindow.
// .SECTION Description
//  wxVTKRenderWindowInteractor provide a VTK widget for wxWindow. This class
// was completely rewrote to have the 'Look & Feel' of the python version:
// wxVTKRenderWindowInteractor.py
// .SECTION Caveats 
//  - There is a know bug that prevent this class to works for more info see 
// WX_USE_X_CAPTURE. This bug only affect wxGTK from 2.3.2 to wxGTK 2.4.0.
//  - Furthermore this class is tempated over either wxWindows or wxGLCanvas,
// in wxWindows 2.3.1 and earlier, the wxGLCanvas had scroll bars, you can avoid
// this effect by playing with WX_BASE_CLASS at your risk (you might end up with
// lot of flickering.)
//  - This class might not be easily readable as it tried to work with VTK 3.2
//  and 4.x. This class doesn't support reparenting with VTK 4.2 and earlier.
// .SECTION see also
// wxVTKRenderWindowInteractor.py wxVTKRenderWindow.py

#ifndef _wxVTKRenderWindowInteractor_h_
#define _wxVTKRenderWindowInteractor_h_

// vtk includes
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderWindow.h>

#include "wx/timer.h"
#define ID_wxVTKRenderWindowInteractor_TIMER 1001

// Use wxGLCanvas as base class instead of wxWindow.
// This is sometimes necessary under wxGTK or the image is blank.
// Furthermore GLCanvas makes disappear flickering (reported by Prabhu
// Ramachandran) with wxGTK 2.3.1
// See: http://public.kitware.com/pipermail/vtkusers/2001-September/007895.html
// Notes: in wxWindows 2.3.1 and earlier, the GLCanvas had scroll bars)
#if wxUSE_GLCANVAS
#ifdef __WXMSW__
#define WX_BASE_CLASS wxWindow
#else
#include <wx/glcanvas.h>
#define WX_BASE_CLASS wxGLCanvas
#endif //__WXMSW__
#endif //wxUSE_GLCANVAS

// wx forward declarations
class wxPaintEvent;
class wxMouseEvent;
class wxTimerEvent;
class wxKeyEvent;
class wxSizeEvent;

class wxVTKRenderWindowInteractor : public WX_BASE_CLASS, virtual public vtkRenderWindowInteractor
{
  DECLARE_DYNAMIC_CLASS(wxVTKRenderWindowInteractor)
  
  public:
  //constructors
    wxVTKRenderWindowInteractor();
    wxVTKRenderWindowInteractor(wxWindow *parent, wxWindowID id, const wxPoint &pos = wxDefaultPosition,
	        const wxSize &size = wxDefaultSize, long style = wxWANTS_CHARS | wxNO_FULL_REPAINT_ON_RESIZE,
           const wxString &name = wxPanelNameStr);
	//vtk ::New()
    static wxVTKRenderWindowInteractor * New();
	 //destructor
    ~wxVTKRenderWindowInteractor() {};

    // vtkRenderWindowInteractor overrides
    void Initialize();
    void Enable();
    void Disable();
    void Start();
    void UpdateSize(int x, int y);
    int CreateTimer(int timertype);
    int DestroyTimer();
    void TerminateApp() {};

    // event handlers
    void OnPaint(wxPaintEvent &event);
    void OnEraseBackground (wxEraseEvent& event);
    void OnMotion(wxMouseEvent &event);

    void OnButtonDown(wxMouseEvent &event);
    void OnButtonUp(wxMouseEvent &event);
    void OnEnter(wxMouseEvent &event);
    void OnLeave(wxMouseEvent &event);
    
    void OnKeyDown(wxKeyEvent &event);
    void OnKeyUp(wxKeyEvent &event);
    void OnTimer(wxTimerEvent &event);
    void OnSize(wxSizeEvent &event);
    
    void Render();
    void SetRenderWhenDisabled(int newValue);

    // Description:
    // Prescribe that the window be created in a stereo-capable mode. This
    // method must be called before the window is realized. Default if off.
    vtkGetMacro(Stereo,int);
    vtkBooleanMacro(Stereo,int);
    virtual void SetStereo(int capable);

  protected:
    wxTimer timer;
    int ActiveButton;
    int RenderAllowed;
    long GetHandle();
    int Stereo;
    
  private:
    long handle;
    bool Created;
    int RenderWhenDisabled;

    DECLARE_EVENT_TABLE()
};

#endif //_wxVTKRenderWindowInteractor_h_
