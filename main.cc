// Copyright 2008 Isis Innovation Limited
// This is the main extry point for PTAM
#include <stdlib.h>
#include <iostream>
#include <gvars3/instances.h>
#include "System.h"


using namespace std;
using namespace GVars3;

int main() {
    cout << "  Welcome to PTAM " << endl;
    cout << "  --------------- " << endl;
    cout << "  Parallel tracking and mapping for Small AR workspaces" << endl;
    cout << "  Copyright (C) Isis Innovation Limited 2008 " << endl;
    cout << endl;
    std::string deviceFolder = "/home/luka/mag_data/devices/device_Huawei_P10";
    std::string testFolder = "/home/luka/mag_data/recordings/doorstep/test_09";

    std::cout << "  Parsing camera.cfg ...." << std::endl;
    GUI.LoadFile(deviceFolder + "/PTAM_calib.cfg");

    try {
        System s("", testFolder);
        s.Run();
    }
    catch (CVD::Exceptions::All e) {
        cout << endl;
        cout << "!! Failed to run system; got exception. " << endl;
        cout << "   Exception was: " << endl;
        cout << e.what << endl;
    }
}










