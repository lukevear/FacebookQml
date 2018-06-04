INCLUDEPATH += $$PWD/src
DEPENDPATH += $$PWD/src

HEADERS += \
    $$PWD/src/facebookqml.h

SOURCES += \
    $$PWD/src/facebookqml.cpp

ios {
    # Ensure that the Facebook SDK has been included
    !exists($$FACEBOOKSDKPATH) {
        error("[FacebookQml] Please set a valid path to the Facebook SDK.")
    }

    # The individual frameworks from the Facebook SDK
    FBQMLFRAMEWORKS = \
        Bolts.framework \
        FBSDKCoreKit.framework \
        FBSDKLoginKit.framework

    # Ensure that every framework file exists
    for(framework, FBQMLFRAMEWORKS) {
        !exists($$FACEBOOKSDKPATH/$$framework) {
            error("[FacebookQml] $$framework was not found in the Facebook SDK directory.")
        }
    }

    # Include the Facebook SDK
    QMAKE_CXXFLAGS += -std=c++11 -stdlib=libc++ -F$$FACEBOOKSDKPATH
    QMAKE_LFLAGS += -stdlib=libc++ -F$$FACEBOOKSDKPATH

    for(framework, FBQMLFRAMEWORKS) {
        LIBS += -lz -framework $$replace(framework, ".framework", "")
    }

    OBJECTIVE_HEADERS += \
        $$PWD/src/ios/facebookqmlutils.h

    OBJECTIVE_SOURCES += \
        $$PWD/src/ios/facebookqmlios.mm
}

android {
    QT += androidextras

    HEADERS += \
        $$PWD/src/android/facebookqmlutils.h

    SOURCES += \
        $$PWD/src/android/facebookqmlandroid.cpp

    include($$PWD/lib/qmakeAndroidSourcesHelper/functions.pri)
    QMAKE_EXTRA_TARGETS += $$copyAndroidSources("facebookqml", "src/com/lukevear/facebookqml", $$shell_path($$PWD/src/android/java/FacebookQml.java))
}

!ios:!android {
    SOURCES += \
        $$PWD/src/generic/facebookqmlgeneric.cpp
}
