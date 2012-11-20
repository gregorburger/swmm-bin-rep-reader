#-------------------------------------------------
#
# Project created by QtCreator 2012-11-16T09:25:37
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = swwm-bin-read
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

QMAKE_CFLAGS += -std=c99
QMAKE_CXXFLAGS += -std=c++11

SOURCES += main.cpp \
    reader.c \
    datetime.c

OTHER_FILES += \
    flow_units.def \
    input_data_type.def \
    subcatch_result_type.def \
    node_result_type.def \
    element_type.def

HEADERS += \
    reader.h \
    enums.h \
    parse_types.h \
    datetime.h
