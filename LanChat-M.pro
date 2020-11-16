QT       += core gui network multimedia
#声明需要使用的模块

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
#如果Qt的版本小于4则添加widgets模块(QtCreator自动生成的代码，用于向下兼容)

CONFIG += c++11
#声明使用C++11标准

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Swap.cpp \
    audio_input.cpp \
    audio_output.cpp \
    main.cpp \
    tcpsocket.cpp \
    udpsocket.cpp \
    widget.cpp

HEADERS += \
    audio_input.h \
    audio_output.h \
    tcpsocket.h \
    udpsocket.h \
    widget.h

FORMS += \
    widget.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

# 安卓编译使用的内容

DISTFILES += \
    android/AndroidManifest.xml \
    android/build.gradle \
    android/gradle/wrapper/gradle-wrapper.jar \
    android/gradle/wrapper/gradle-wrapper.properties \
    android/gradlew \
    android/gradlew.bat \
    android/res/values/libs.xml
