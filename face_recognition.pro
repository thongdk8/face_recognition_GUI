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

#MongoDB
#INCLUDEPATH += /usr/local/include/mongocxx/v_noabi /usr/local/include/bsoncxx/v_noabi /usr/local/include/libbson-1.0 /usr/local/include/libmongoc-1.0
#LIBS += -L/usr/local/lib -lmongocxx -lbsoncxx -lmongoc-1.0

SOURCES += enrolface.cpp\
      main.cpp\
        mainwindow.cpp \
    facerecognizer.cpp \
#    dbconector.cpp \
#    facedatamodel.cpp
 #   enrolface.cpp

HEADERS  += mainwindow.h \
    defineddnn.h \
    facerecognizer.h \
#    dbconector.h \
#    facedatamodel.h
    datamanager.h \
    hnswlib/bruteforce.h \
    hnswlib/hnswalg.h \
    hnswlib/hnswlib.h \
    hnswlib/space_ip.h \
    hnswlib/space_l2.h \
    hnswlib/visited_list_pool.h \
    cnndetector.h

FORMS    += mainwindow.ui
