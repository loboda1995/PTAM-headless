// Copyright 2008 Isis Innovation Limited
#include "System.h"
#include "OpenGL.h"
#include <gvars3/instances.h>
#include <stdlib.h>
#include "ATANCamera.h"
#include "MapMaker.h"
#include "Tracker.h"
#include "MapViewer.h"

using namespace CVD;
using namespace std;
using namespace GVars3;


System::System(const std::string &rootFolder, const std::string &testFolder) {
    GUI.RegisterCommand("exit", GUICommandCallBack, this);
    GUI.RegisterCommand("quit", GUICommandCallBack, this);

    std::string filename = testFolder + "/video_nomarker.avi";
    mVideoSource.Open(filename);

    mGLWindow = new GLWindow2(mVideoSource.Size(), "PTAM");
    mimFrameBW.resize(mVideoSource.Size());
    mimFrameRGB.resize(mVideoSource.Size());
    // First, check if the camera is calibrated.
    // If not, we need to run the calibration widget.
    Vector<NUMTRACKERCAMPARAMETERS> vTest;

    vTest = GV3::get<Vector<NUMTRACKERCAMPARAMETERS> >("Camera.Parameters", ATANCamera::mvDefaultParams, HIDDEN);
    mpCamera = new ATANCamera("Camera");
    Vector<2> v2;
    if (v2 == v2);
    if (vTest == ATANCamera::mvDefaultParams) {
        cout << endl;
        cout << "! Camera.Parameters is not set, need to run the CameraCalibrator tool" << endl;
        cout << "  and/or put the Camera.Parameters= line into the appropriate .cfg file." << endl;
        exit(1);
    }

    mpMap = new Map;
    mpMapMaker = new MapMaker(*mpMap, *mpCamera);
    mpTracker = new Tracker(mVideoSource.Size(), *mpCamera, *mpMap, *mpMapMaker, mVideoSource, testFolder + "/PTAM_path.txt");
    mpMapViewer = new MapViewer(*mpMap, *mGLWindow);

    GUI.ParseLine("GLWindow.AddMenu Menu Menu");
    GUI.ParseLine("Menu.ShowMenu Root");
    GUI.ParseLine("Menu.AddMenuButton Root Reset Reset Root");
    GUI.ParseLine("Menu.AddMenuButton Root Spacebar PokeTracker Root");
    GUI.ParseLine("DrawAR=0");
    GUI.ParseLine("DrawMap=0");
    GUI.ParseLine("Menu.AddMenuToggle Root \"View Map\" DrawMap Root");
    GUI.ParseLine("Menu.AddMenuToggle Root \"Draw AR\" DrawAR Root");

    mbDone = false;
};

void System::Run() {
    bool started = false;
    int start = 40;
    int end = 90;
    while (!mbDone) {

        // We use two versions of each video frame:
        // One black and white (for processing by the tracker etc)
        // and one RGB, for drawing.

        // Grab new video frame...
        mVideoSource.GetAndFillFrameBWandRGB(mimFrameBW, mimFrameRGB);

        mGLWindow->SetupViewport();
        mGLWindow->SetupVideoOrtho();
        mGLWindow->SetupVideoRasterPosAndZoom();

        static gvar3<int> gvnDrawMap("DrawMap", 0, HIDDEN | SILENT);
        static gvar3<int> gvnDrawAR("DrawAR", 0, HIDDEN | SILENT);

        bool bDrawMap = mpMap->IsGood() && *gvnDrawMap;
        bool bDrawAR = mpMap->IsGood() && *gvnDrawAR;

        if ((!started && mVideoSource.GetFrameN() >= start) || (started && mVideoSource.GetFrameN() >= end)) {
            started = true;
            mpTracker->mbUserPressedSpacebar = true;
        }

        mpTracker->TrackFrame(mimFrameBW, !bDrawAR && !bDrawMap);

        if (bDrawMap)
            mpMapViewer->DrawMap(mpTracker->GetCurrentPose());

        string sCaption;
        if (bDrawMap)
            sCaption = mpMapViewer->GetMessageForUser();
        else
            sCaption = mpTracker->GetMessageForUser();
        mGLWindow->DrawCaption(sCaption);
        mGLWindow->DrawMenus();
        mGLWindow->swap_buffers();
        mGLWindow->HandlePendingEvents();
    }
    delete mGLWindow;
}

void System::GUICommandCallBack(void *ptr, string sCommand, string sParams) {
    if (sCommand == "quit" || sCommand == "exit")
        static_cast<System *>(ptr)->mbDone = true;
}








