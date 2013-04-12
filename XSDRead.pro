#-------------------------------------------------
#
# Project created by QtCreator 2013-04-11T09:29:07
#
#-------------------------------------------------

QT       += core gui xml xmlpatterns

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = XSDRead
TEMPLATE = app


SOURCES += main.cpp\
        xsdread.cpp

HEADERS  += xsdread.h

FORMS    +=

RESOURCES += \
    xsdread.qrc

OTHER_FILES += \
    factory.xsd \
    factory.xml
