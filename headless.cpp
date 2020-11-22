#include <stdlib.h>
#include <iostream>
#include <gvars3/instances.h>
#include "HeadlessSystem.h"


using namespace GVars3;

int main(int argc, char** argv) {


    if (argc < 5) {
        std::cout << "Missing required parameters: configFile, rootFolder, startInit, endInit, pathFile" << std::endl;
        exit(1);
    }

    std::string configFile = argv[1];
    std::string rootFolder = argv[2];
    int start = atoi(argv[3]);
    int end = atoi(argv[4]);
    std::string pathFile = argv[5];

    std::cout << "Parsing '" << configFile << "' ..." << std::endl;
    GUI.LoadFile(configFile);

    try {
        HeadlessSystem s(rootFolder, pathFile, start, end);
        s.Run();
    } catch (CVD::Exceptions::All e) {
        std::cout << std::endl;
        std::cout << "!! Failed to run system; got exception. " << std::endl;
        std::cout << "   Exception was: " << std::endl;
        std::cout << e.what << std::endl;
    }
    std::cout << "Finished" << std::endl;
    exit(0);
}










