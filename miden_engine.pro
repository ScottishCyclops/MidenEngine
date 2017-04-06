QT += core
QT -= gui

CONFIG += console c++11
LIBS += -lSDL2

TARGET = miden_engine
#CONFIG -= qt
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    display.cpp \
    color.cpp \
    emath.cpp \
    vector3.cpp \
    vertex.cpp \
    renderer.cpp \
    mesh.cpp

HEADERS += \
    display.h \
    color.h \
    emath.h \
    vector3.h \
    vertex.h \
    includes.h \
    renderer.h \
    mesh.h
DEFINES += QT_DEPRECATED_WARNINGS
