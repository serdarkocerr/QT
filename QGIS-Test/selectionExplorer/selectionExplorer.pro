QT       += gui core xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
INCLUDEPATH += D:\OSGeo4W\apps\qgis-ltr-dev\include
INCLUDEPATH +=D:\OSGeo4W\include    \
              D:\OSGeo4W\apps\gdal-dev\include  \
              D:\OSGeo4W\apps\Qt5\include\QtCrypto  \
              D:\OSGeo4W\apps\apache\include    \
              D:\OSGeo4W\apps\Qt5\include\qt5keychain

LIBS += -L"D:\OSGeo4W\apps\qgis-ltr-dev\lib" -lqgis_core -lqgis_gui
LIBS += -L"D:\OSGeo4W\apps\gdal-dev\lib" -lgdal_i
#LIBS += -L"D:/OSGeo4W/apps/qgis/lib" -lqgis_core -lqgis_gui


DEFINES+=CORE_EXPORT=
DEFINES+=GUI_EXPORT=


SOURCES += \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    mainwindow.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
