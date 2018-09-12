#-------------------------------------------------
#
# Project created by QtCreator 2018-09-10T22:01:20
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = face_recognition
TEMPLATE = app

CONFIG += c++11

#OpenCV
LIBS += `pkg-config opencv --libs`

#Dlib
PKGCONFIG += dlib-1

#INCLUDEPATH += /usr/local/include/dlib
LIBS += -L/usr/local/lib -ldlib
LIBS += -L/usr/lib -lopenblas
LIBS += -L/usr/local/cuda/lib64 -lcublas -lcusolver -lcurand

SOURCES += main.cpp\
        mainwindow.cpp \
    facerecognizer.cpp

HEADERS  += mainwindow.h \
    defineddnn.h \
    facerecognizer.h

FORMS    += mainwindow.ui
