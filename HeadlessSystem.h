#ifndef __HEADLESS_SYSTEM_H
#define __HEADLESS_SYSTEM_H
#include "MapViewer.h"
#include "Tracker.h"

#include <cvd/image.h>
#include <cvd/rgb.h>
#include <cvd/byte.h>

class ATANCamera;
class Map;
class MapMaker;
class Tracker;
class MapViewer;

class HeadlessSystem
{
public:
    HeadlessSystem(const std::string &rootFolder, const std::string &pathFile, int startInit, int endInit);
    void Run();

private:
    VideoSource mVideoSource;
    CVD::Image<CVD::Rgb<CVD::byte> > mimFrameRGB;
    CVD::Image<CVD::byte> mimFrameBW;

    Map *mpMap;
    MapMaker *mpMapMaker;
    Tracker *mpTracker;
    ATANCamera *mpCamera;

    bool mbDone;
    int lastFrameN = -1;
    int startInit = -1;
    int endInit = -1;
};



#endif
