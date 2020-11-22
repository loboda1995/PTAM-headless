// Copyright 2008 Isis Innovation Limited
#include "HeadlessSystem.h"
#include <gvars3/instances.h>
#include "ATANCamera.h"
#include "MapMaker.h"

using namespace CVD;
using namespace GVars3;


HeadlessSystem::HeadlessSystem(const std::string &rootFolder, const std::string &pathFile, int start, int end) {

    std::string filename = rootFolder+"/video_nomarker.avi";
    mVideoSource.Open(filename);

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
        std::cout << std::endl;
        std::cout << "! Camera.Parameters is not set, need to run the CameraCalibrator tool" << std::endl;
        std::cout << "  and/or put the Camera.Parameters= line into the appropriate .cfg file." << std::endl;
        exit(1);
    }

    mpMap = new Map;
    mpMapMaker = new MapMaker(*mpMap, *mpCamera);
    mpTracker = new Tracker(mVideoSource.Size(), *mpCamera, *mpMap, *mpMapMaker, mVideoSource, pathFile);

    mbDone = false;
    startInit = start;
    endInit = end;
};


void HeadlessSystem::Run() {
    static bool bSkipFrames = true;
    bool started = false;

    while (!mbDone) {
        // Grab new video frame...
        if (!mVideoSource.GetAndFillFrameBWandRGB(mimFrameBW, mimFrameRGB)) {
            mpMapMaker->Stop();
            mbDone = true;
            std::cout << "Reached end of video. Exiting... " << std::endl;
        } else if (!bSkipFrames || lastFrameN != mVideoSource.GetFrameN()){
            lastFrameN = mVideoSource.GetFrameN();
            if ((!started && lastFrameN >= startInit) || (started && lastFrameN >= endInit)) {
                started = true;
                mpTracker->mbUserPressedSpacebar = true;
            }
            mpTracker->TrackFrame(mimFrameBW, false);
        }
    }
}









