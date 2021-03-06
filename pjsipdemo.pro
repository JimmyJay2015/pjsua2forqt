######################################################################
# Automatically generated by qmake (3.0) Sun Oct 28 23:58:20 2018
######################################################################

QT       += widgets

TEMPLATE = app
TARGET = pjsipdemo
CONFIG += static thread release

CONFIG(debug, debug|release) {
    OBJECTS_DIR = ./build/DebugIntermediate
    MOC_DIR = ./build/GeneratedFiles/Debug
    RCC_DIR = ./build/GeneratedFiles
    DESTDIR = ./build/Debug
}
CONFIG(release, debug|release) {
    OBJECTS_DIR = ./build/ReleaseIntermediate
    MOC_DIR = ./build/GeneratedFiles/Release
    RCC_DIR = ./build/GeneratedFiles
    DESTDIR = ./build/Release
}


# Input
HEADERS += ./pjsipdemo/mainwindow.h \
           ./pjsipdemo/pjsipmanager/pjsipmanager.h \
           ./pjsipdemo/pjsipmanager/pjsuaaccount.h \
           ./pjsipdemo/pjsipmanager/pjsuacall.h \
           ./pjsipdemo/pjsipmanager/pjvidwidget.h 
           

SOURCES += ./pjsipdemo/main.cpp \
           ./pjsipdemo/mainwindow.cpp \
           ./pjsipdemo/pjsipmanager/pjsipmanager.cpp \
           ./pjsipdemo/pjsipmanager/pjsuaaccount.cpp \
           ./pjsipdemo/pjsipmanager/pjsuacall.cpp 


win32 {
    SOURCES += ./pjsipdemo/pjsipmanager/pjvidwidget_win.cpp
}

macx {
    QMAKE_CXXFLAGS += -ObjC++
    
    SOURCES += ./pjsipdemo/pjsipmanager/pjvidwidget_mac.mm

    INCLUDEPATH += ./pjsipdemo/pjsiplib/mac/include \
        ./pjsipdemo/sdllib/mac/include \

    LIBS += -lstdc++ -framework CoreAudio -framework CoreServices -framework AudioUnit -framework AudioToolbox -framework Foundation -framework AppKit -framework AVFoundation -framework CoreGraphics -framework QuartzCore -framework CoreVideo -framework CoreMedia -framework VideoToolbox -L/Users/jimmy-zzm/Documents/pjsip/source2/build/lib -lSDL2 -lm -Wl,-framework,CoreAudio -Wl,-framework,AudioToolbox -Wl,-framework,ForceFeedback -lobjc -Wl,-framework,CoreVideo -Wl,-framework,Cocoa -Wl,-framework,Carbon -Wl,-framework,IOKit -Wl,-weak_framework,QuartzCore -Wl,-weak_framework,Metal

    LIBS += /usr/local/lib/libopus.a

    LIBS += ./pjsipdemo/h264/mac/libopenh264.a \
        ./pjsipdemo/sdllib/mac/lib/libSDL2.a \
        ./pjsipdemo/pjsiplib/mac/lib/libpjsua2-x86_64-apple-darwin16.7.0.a \
        ./pjsipdemo/pjsiplib/mac/lib/libpjsua-x86_64-apple-darwin16.7.0.a \
        ./pjsipdemo/pjsiplib/mac/lib/libpjsip-ua-x86_64-apple-darwin16.7.0.a \
        ./pjsipdemo/pjsiplib/mac/lib/libpjsip-simple-x86_64-apple-darwin16.7.0.a \
        ./pjsipdemo/pjsiplib/mac/lib/libpj-x86_64-apple-darwin16.7.0.a \
        ./pjsipdemo/pjsiplib/mac/lib/libpjsip-x86_64-apple-darwin16.7.0.a \
        ./pjsipdemo/pjsiplib/mac/lib/libpjmedia-audiodev-x86_64-apple-darwin16.7.0.a \
        ./pjsipdemo/pjsiplib/mac/lib/libpjmedia-codec-x86_64-apple-darwin16.7.0.a \
        ./pjsipdemo/pjsiplib/mac/lib/libpjmedia-videodev-x86_64-apple-darwin16.7.0.a \
        ./pjsipdemo/pjsiplib/mac/lib/libpjmedia-x86_64-apple-darwin16.7.0.a \
        ./pjsipdemo/pjsiplib/mac/lib/libpjnath-x86_64-apple-darwin16.7.0.a \
        ./pjsipdemo/pjsiplib/mac/lib/libsrtp-x86_64-apple-darwin16.7.0.a \
        ./pjsipdemo/pjsiplib/mac/lib/libspeex-x86_64-apple-darwin16.7.0.a \
        ./pjsipdemo/pjsiplib/mac/lib/libresample-x86_64-apple-darwin16.7.0.a \
        ./pjsipdemo/pjsiplib/mac/lib/libyuv-x86_64-apple-darwin16.7.0.a \
        ./pjsipdemo/pjsiplib/mac/lib/libpjnath-x86_64-apple-darwin16.7.0.a \
        ./pjsipdemo/pjsiplib/mac/lib/libgsmcodec-x86_64-apple-darwin16.7.0.a \
        ./pjsipdemo/pjsiplib/mac/lib/libilbccodec-x86_64-apple-darwin16.7.0.a \
        ./pjsipdemo/pjsiplib/mac/lib/libg7221codec-x86_64-apple-darwin16.7.0.a \
        ./pjsipdemo/pjsiplib/mac/lib/libpjlib-util-x86_64-apple-darwin16.7.0.a
}

linux {
    SOURCES += ./pjsipdemo/pjsipmanager/pjvidwidget_linux.cpp

    INCLUDEPATH += "/usr/local/tdesktop/Qt-5.6.0/include/QtX11Extras"
    LIBS += /usr/local/tdesktop/Qt-5.6.0/lib/libQt5X11Extras.a

    LIBS += $$system(pkg-config --cflags --libs libpjproject)

}


        