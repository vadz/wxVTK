// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

// for all others, include the necessary headers (this file is usually all you
// need because it includes almost all "standard" wxWindows headers)
#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif

#include "wxVTKRenderWindowInteractor.h"
#include "vtkCamera.h"
#include "vtkRenderer.h"
#include "vtkRenderWindow.h"
#include "vtkConeSource.h"
#include "vtkPolyDataMapper.h"
#include "vtkActor.h"
#include "vtkPolyDataReader.h"

#include "vtkActor.h"
#include "vtkCamera.h"
#include "vtkCellPicker.h"
#include "vtkCommand.h"
#include "vtkImageActor.h"
#include "vtkImageMapToColors.h"
#include "vtkImagePlaneWidget.h"
#include "vtkImageReader.h"
#include "vtkInteractorEventRecorder.h"
#include "vtkLookupTable.h"
#include "vtkOutlineFilter.h"
#include "vtkPolyDataMapper.h"
#include "vtkProperty.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkRenderer.h"
#include "vtkVolume16Reader.h"
#include "vtkImageReslice.h"
#include "vtkTesting.h"
#include "vtkTestUtilities.h"

#include "vtkImageData.h"
#ifndef vtkFloatingPointType
#define vtkFloatingPointType float
#endif


// the application icon
#ifndef __WXMSW__
    #include "mondrian.xpm"
#endif

class MyApp;
class MyFrame;

// Define a new application type, each program should derive a class from wxApp
class MyApp : public wxApp
{
public:
    // this one is called on application startup and is a good place for the app
    // initialization (doing it here and not in the ctor allows to have an error
    // return: if OnInit() returns false, the application terminates)
    virtual bool OnInit();
};

// Define a new frame type: this is going to be our main frame
class MyFrame : public wxFrame
{
public:
    // ctor(s)
    MyFrame(const wxString& title, const wxPoint& pos, const wxSize& size);
    ~MyFrame();

    // event handlers (these functions should _not_ be virtual)
    void OnQuit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);

protected:
    void ConfigureVTK();

private:
  wxVTKRenderWindowInteractor *m_pVTKWindow;
  vtkImagePlaneWidget* planeWidgetX;
  vtkImagePlaneWidget* planeWidgetY;
  vtkImagePlaneWidget* planeWidgetZ;
  
private:
    // any class wishing to process wxWindows events must use this macro
    DECLARE_EVENT_TABLE()
};

// IDs for the controls and the menu commands
enum
{
    // menu items
    Minimal_Quit = 1,
    Minimal_About
};

#define MY_FRAME      101
#define MY_VTK_WINDOW 102

// the event tables connect the wxWindows events with the functions (event
// handlers) which process them. It can be also done at run-time, but for the
// simple menu events like this the static method is much simpler.
BEGIN_EVENT_TABLE(MyFrame, wxFrame)
    EVT_MENU(Minimal_Quit,  MyFrame::OnQuit)
    EVT_MENU(Minimal_About, MyFrame::OnAbout)
END_EVENT_TABLE()

// Create a new application object: this macro will allow wxWindows to create
// the application object during program execution (it's better than using a
// static object for many reasons) and also declares the accessor function
// wxGetApp() which will return the reference of the right type (i.e. MyApp and
// not wxApp)
IMPLEMENT_APP(MyApp)

// 'Main program' equivalent: the program execution "starts" here
bool MyApp::OnInit()
{
    // create the main application window
    MyFrame *frame = new MyFrame("wxWindows-VTK App",
                                 wxPoint(50, 50), wxSize(450, 340));

    // and show it (the frames, unlike simple controls, are not shown when
    // created initially)
    frame->Show(TRUE);

    // success: wxApp::OnRun() will be called which will enter the main message
    // loop and the application will run. If we returned FALSE here, the
    // application would exit immediately.
    return TRUE;
}

// frame constructor
MyFrame::MyFrame(const wxString& title, const wxPoint& pos, const wxSize& size)
       : wxFrame((wxFrame *)NULL, -1, title, pos, size)
{
#ifdef __WXMAC__
    // we need this in order to allow the about menu relocation, since ABOUT is
    // not the default id of the about menu
    wxApp::s_macAboutMenuItemId = Minimal_About;
#endif

    // set the frame icon
    SetIcon(wxICON(mondrian));

    // create a menu bar
    wxMenu *menuFile = new wxMenu("", wxMENU_TEAROFF);

    // the "About" item should be in the help menu
    wxMenu *helpMenu = new wxMenu;
    helpMenu->Append(Minimal_About, "&About...\tCtrl-A", "Show about dialog");

    menuFile->Append(Minimal_Quit, "E&xit\tAlt-X", "Quit this program");

    // now append the freshly created menu to the menu bar...
    wxMenuBar *menuBar = new wxMenuBar();
    menuBar->Append(menuFile, "&File");
    menuBar->Append(helpMenu, "&Help");

    // ... and attach this menu bar to the frame
    SetMenuBar(menuBar);

#if wxUSE_STATUSBAR
    // create a status bar just for fun (by default with 1 pane only)
    CreateStatusBar(2);
    SetStatusText("Drag the mouse here! (wxWindows 2.4.0)");
#endif // wxUSE_STATUSBAR

    m_pVTKWindow = new wxVTKRenderWindowInteractor(this, MY_VTK_WINDOW);
    //turn on mouse grabbing if possible
    m_pVTKWindow->UseCaptureMouseOn();

    ConfigureVTK();
}

MyFrame::~MyFrame()
{
  planeWidgetX->Delete();
  planeWidgetY->Delete();
  planeWidgetZ->Delete();

  if(m_pVTKWindow) m_pVTKWindow->Delete();
}

void MyFrame::ConfigureVTK()
{
  // connect the render window and wxVTK window
  vtkRenderWindow* pRenderWindow = m_pVTKWindow->GetRenderWindow();

  char* fname = vtkTestUtilities::ExpandDataFileName(0, 0, "Data/headsq/quarter");

  vtkVolume16Reader* v16 =  vtkVolume16Reader::New();
    v16->SetDataDimensions( 64, 64);
    v16->SetDataByteOrderToLittleEndian();
    v16->SetImageRange( 1, 93);
    v16->SetDataSpacing( 3.2, 3.2, 1.5);
    v16->SetFilePrefix( fname );
    v16->SetDataMask( 0x7fff);
    v16->Update();
  delete[] fname;

  vtkOutlineFilter* outline = vtkOutlineFilter::New();
    outline->SetInput(v16->GetOutput());

  vtkPolyDataMapper* outlineMapper = vtkPolyDataMapper::New();
    outlineMapper->SetInput(outline->GetOutput());

  vtkActor* outlineActor =  vtkActor::New();
    outlineActor->SetMapper( outlineMapper);

  vtkRenderer* ren1 = vtkRenderer::New();
  vtkRenderer* ren2 = vtkRenderer::New();

  pRenderWindow->AddRenderer(ren2);
  pRenderWindow->AddRenderer(ren1);

  vtkCellPicker* picker = vtkCellPicker::New();
    picker->SetTolerance(0.005);

  vtkProperty* ipwProp = vtkProperty::New();
   //assign default props to the ipw's texture plane actor

  planeWidgetX = vtkImagePlaneWidget::New();
    planeWidgetX->SetInteractor( m_pVTKWindow);
    planeWidgetX->SetKeyPressActivationValue('x');
    planeWidgetX->SetPicker(picker);
    planeWidgetX->RestrictPlaneToVolumeOn();
    planeWidgetX->GetPlaneProperty()->SetColor(1,0,0);
    planeWidgetX->SetTexturePlaneProperty(ipwProp);
    planeWidgetX->TextureInterpolateOff();
    planeWidgetX->SetResliceInterpolateToNearestNeighbour();
    planeWidgetX->SetInput(v16->GetOutput());
    planeWidgetX->SetPlaneOrientationToXAxes();
    planeWidgetX->SetSliceIndex(32);
    planeWidgetX->DisplayTextOn();
    planeWidgetX->On();
    planeWidgetX->InteractionOff();
    planeWidgetX->InteractionOn();

  planeWidgetY = vtkImagePlaneWidget::New();
    planeWidgetY->SetInteractor( m_pVTKWindow);
    planeWidgetY->SetKeyPressActivationValue('y');
    planeWidgetY->SetPicker(picker);
    planeWidgetY->GetPlaneProperty()->SetColor(1,1,0);
    planeWidgetY->SetTexturePlaneProperty(ipwProp);
    planeWidgetY->TextureInterpolateOn();
    planeWidgetY->SetResliceInterpolateToLinear();
    planeWidgetY->SetInput(v16->GetOutput());
    planeWidgetY->SetPlaneOrientationToYAxes();
    planeWidgetY->SetSlicePosition(102.4);
    planeWidgetY->SetLookupTable( planeWidgetX->GetLookupTable());
    planeWidgetY->DisplayTextOff();
    planeWidgetY->UpdatePlacement();
    planeWidgetY->On();

  planeWidgetZ = vtkImagePlaneWidget::New();
    planeWidgetZ->SetInteractor( m_pVTKWindow);
    planeWidgetZ->SetKeyPressActivationValue('z');
    planeWidgetZ->SetPicker(picker);
    planeWidgetZ->GetPlaneProperty()->SetColor(0,0,1);
    planeWidgetZ->SetTexturePlaneProperty(ipwProp);
    planeWidgetZ->TextureInterpolateOn();
    planeWidgetZ->SetResliceInterpolateToCubic();
    planeWidgetZ->SetInput(v16->GetOutput());
    planeWidgetZ->SetPlaneOrientationToZAxes();
    planeWidgetZ->SetSliceIndex(25);
    planeWidgetZ->SetLookupTable( planeWidgetX->GetLookupTable());
    planeWidgetZ->DisplayTextOn();
    planeWidgetZ->On();

  vtkFloatingPointType wl[2];
  planeWidgetZ->GetWindowLevel(wl);

  // Add a 2D image to test the GetReslice method
  //
  vtkImageMapToColors* colorMap = vtkImageMapToColors::New();
    colorMap->PassAlphaToOutputOff();
    colorMap->SetActiveComponent(0);
    colorMap->SetOutputFormatToLuminance();
    colorMap->SetInput(planeWidgetZ->GetResliceOutput());
    colorMap->SetLookupTable(planeWidgetX->GetLookupTable());

  vtkImageActor* imageActor = vtkImageActor::New();
    imageActor->PickableOff();
    imageActor->SetInput(colorMap->GetOutput());

  // Add the actors
  //
  ren1->AddActor( outlineActor);
  ren2->AddActor( imageActor);

  ren1->SetBackground( 0.1, 0.1, 0.2);
  ren2->SetBackground( 0.2, 0.1, 0.2);

//  renWin->SetSize( 600, 350);

  ren1->SetViewport(0,0,0.58333,1);
  ren2->SetViewport(0.58333,0,1,1);

  // Set the actors' postions
  //
//  renWin->Render();
  m_pVTKWindow->SetEventPosition( 175,175);
  m_pVTKWindow->SetKeyCode('r');
  m_pVTKWindow->InvokeEvent(vtkCommand::CharEvent,NULL);
  m_pVTKWindow->SetEventPosition( 475,175);
  m_pVTKWindow->SetKeyCode('r');
  m_pVTKWindow->InvokeEvent(vtkCommand::CharEvent,NULL);
  //renWin->Render();

  ren1->GetActiveCamera()->Elevation(110);
  ren1->GetActiveCamera()->SetViewUp(0, 0, -1);
  ren1->GetActiveCamera()->Azimuth(45);
  ren1->GetActiveCamera()->Dolly(1.15);
  ren1->ResetCameraClippingRange();


  // Test SetKeyPressActivationValue for one of the widgets
  //
  m_pVTKWindow->SetKeyCode('z');
  m_pVTKWindow->InvokeEvent(vtkCommand::CharEvent,NULL);
  m_pVTKWindow->SetKeyCode('z');
  m_pVTKWindow->InvokeEvent(vtkCommand::CharEvent,NULL);

  v16->Delete();
  outline->Delete();
  outlineMapper->Delete();
  outlineActor->Delete();
  ren1->Delete();
  ren2->Delete();
  picker->Delete();
  ipwProp->Delete();
  colorMap->Delete();
  imageActor->Delete();
 
}

// event handlers

void MyFrame::OnQuit(wxCommandEvent& WXUNUSED(event))
{
    // TRUE is to force the frame to close
    Close(TRUE);
}

void MyFrame::OnAbout(wxCommandEvent& WXUNUSED(event))
{
    wxString msg;
    msg.Printf( _T("This is the about dialog of wx-vtk sample.\n"));

    wxMessageBox(msg, "About wx-vtk", wxOK | wxICON_INFORMATION, this);
}
