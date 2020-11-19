# ----------------------------------------------------
# This file is generated by the Qt Visual Studio Add-in.
# ------------------------------------------------------

TEMPLATE = app
TARGET = PyramidM
DESTDIR = ./Release
QT += core gui network xml xmlpatterns webkit
CONFIG += release
DEFINES += QT_DLL
INCLUDEPATH += ./../../.. \
    ./.. \
    . \
    ./GeneratedFiles/Win32/Release \
    ./GeneratedFiles \
    ./../../../opencv/build/include/opencv \
    ./../../../opencv/build/include/opencv2/calib3d \
    ./../../../opencv/build/include/opencv2/contrib \
    ./../../../opencv/build/include/opencv2/core \
    ./../../../opencv/build/include/opencv2/features2d \
    ./../../../opencv/build/include/opencv2/flann \
    ./../../../opencv/build/include/opencv2/gpu \
    ./../../../opencv/build/include/opencv2/highgui \
    ./../../../opencv/build/include/opencv2/imgproc \
    ./../../../opencv/build/include/opencv2/legacy \
    ./../../../opencv/build/include/opencv2/ml \
    ./../../../opencv/build/include/opencv2/objdetect \
    ./../../../opencv/build/include/opencv2 \
    ./../../../opencv/build/include/opencv2/ts \
    ./../../../opencv/build/include/opencv2/video \
    ./../MSVC_2008 \
    ./../../.. \
    ./../.. \
    ./../../../../PyramidM
LIBS += -L"$(SolutionDir)$(ConfigurationName)" \
    -lPyramidWidgets
PRECOMPILED_HEADER = StdAfx.h
DEPENDPATH += .
MOC_DIR += ./GeneratedFiles/win32/release
OBJECTS_DIR += release
UI_DIR += ./GeneratedFiles
RCC_DIR += ./GeneratedFiles
RC_FILE = Pyramid.rc
include(PyramidM.pri)