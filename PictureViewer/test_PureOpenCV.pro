TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

INCLUDEPATH += /usr/local/include/opencv2 \
               /usr/local/include/opencv

LIBS += -L/usr/local/lib64 -lopencv_core -lopencv_imgcodecs \
                           -lopencv_highgui -lopencv_imgproc

SOURCES += main.cpp
