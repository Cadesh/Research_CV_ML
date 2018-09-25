TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

QMAKE_CXXFLAGS += -fopenmp -openmp
QMAKE_LFLAGS += -fopenmp
LIBS += -fopenmp



SOURCES += main.cpp
