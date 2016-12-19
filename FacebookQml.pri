INCLUDEPATH += $$PWD/src
DEPENDPATH += $$PWD/src

HEADERS += \
    $$PWD/src/facebookqml.h \
    $$PWD/src/login/facebooklogin.h

SOURCES += \
    $$PWD/src/facebookqml.cpp \
    $$PWD/src/login/facebooklogin.cpp

ios {
    HEADERS += \
        $$PWD/src/login/ios/facebookloginios.h

    SOURCES += \
        $$PWD/src/login/ios/facebookloginios.mm
}

android {
    HEADERS += \
        $$PWD/src/login/android/facebookloginandroid.h

    SOURCES += \
        $$PWD/src/login/android/facebookloginandroid.cpp

    OTHER_FILES += \
        $$PWD/src/android/com/lukevear/facebookqml/login/FacebookLoginCallbackManager.java
}