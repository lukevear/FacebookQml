INCLUDEPATH += $$PWD/src
DEPENDPATH += $$PWD/src

HEADERS += \
    $$PWD/src/facebookqml.h \
    $$PWD/src/facebookqmlplatformimpl.h

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
        $$PWD/src/ios/facebookqmliosutils.h

    OBJECTIVE_SOURCES += \
        $$PWD/src/ios/facebookqmlplatformimpl.mm
}

android {
    QT += androidextras

    HEADERS += \
        $$PWD/src/android/facebookqmlandroidutils.h

    SOURCES += \
        $$PWD/src/android/facebookqmlplatformimpl.cpp

    # This dirty hack ensures that our .java files end up in the source android build directory
    !isEmpty(ANDROID_PACKAGE_SOURCE_DIR) {
        copydata.commands += $(COPY_DIR) $$shell_path($$PWD/src/android/bundle/*) $$shell_path($$ANDROID_PACKAGE_SOURCE_DIR/src)
        first.depends = $(first) copydata
        export(first.depends)
        export(copydata.commands)
        android:QMAKE_EXTRA_TARGETS += first copydata
    }
}
