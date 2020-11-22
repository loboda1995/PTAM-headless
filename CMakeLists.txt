cmake_minimum_required(VERSION 3.10)
project(PTAM)

set(CMAKE_CXX_STANDARD 17)

find_package( OpenCV REQUIRED )

SET(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-Wno-enum-compare")


add_executable(PTAM main.cc GLWindow2.cc GLWindowMenu.cc VideoSource.cpp System.cc ATANCamera.cc KeyFrame.cc MapPoint.cc Map.cc SmallBlurryImage.cc ShiTomasi.cc HomographyInit.cc MapMaker.cc Bundle.cc PatchFinder.cc Relocaliser.cc MiniPatch.cc MapViewer.cc Tracker.cc)
target_include_directories(PTAM PUBLIC /usr/local/include/eigen3)
target_link_libraries(PTAM GVars3 cvd GL lapack ${OpenCV_LIBS} opencv_videoio)

add_executable(HeadlessPTAM headless.cpp VideoSource.cpp HeadlessSystem.cpp ATANCamera.cc KeyFrame.cc MapPoint.cc Map.cc SmallBlurryImage.cc ShiTomasi.cc HomographyInit.cc MapMaker.cc Bundle.cc PatchFinder.cc Relocaliser.cc MiniPatch.cc Tracker.cc)
target_include_directories(HeadlessPTAM PUBLIC /usr/local/include/eigen3)
target_link_libraries(HeadlessPTAM GVars3 cvd GL lapack ${OpenCV_LIBS} opencv_videoio)