TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

#QMAKE_CXXFLAGS += -D__STDC_CONSTANT_MACROS

INCLUDEPATH += /usr/include/ffmpeg

LIBS += -L/usr/include/ffmpeg -lavcodec -lavformat \
                              -lswscale -lavutil

SOURCES += main.c
