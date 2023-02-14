TEMPLATE = lib

CONFIG += qt warn_on

QMAKE_CXXFLAGS += /std:c++17

QT += core gui widgets

INCLUDEPATH +=  src/ \
                ./ \
                ../../include \
                ../../qscint/src \
                ../../qscint/src/Qsci

RESOURCES += \
    src/image.qrc

FORMS += \
    src/docktitlewidget.ui \
    src/jsonviewsettings.ui

HEADERS += \
    src/define.h \
    src/docktitlewidget.h \
    src/jsonhandler.h \
    src/jsonnode.h \
    src/jsonviewsettings.h \
    src/nddjsonplugin.h \
    src/pluginGl.h \
    src/qjsonmodel.h \
    src/rapidjsonhandler.h \
    src/scintillaeditor.h

SOURCES += \
    src/dllmain.cpp \
    src/docktitlewidget.cpp \
    src/jsonhandler.cpp \
    src/jsonviewsettings.cpp \
    src/nddjsonplugin.cpp \
    src/qjsonmodel.cpp \
    src/rapidjsonhandler.cpp \
    src/scintillaeditor.cpp

win32 {
   if(contains(QMAKE_HOST.arch, x86_64)){
    CONFIG(Debug, Debug|Release){
        DESTDIR = ../../x64/Debug/plugin
                LIBS += -L../../x64/Debug
                LIBS += -lqmyedit_qt5d
    }else{
        DESTDIR = ../../x64/Release/plugin
                LIBS += -L../../x64/Release
                LIBS += -lqmyedit_qt5
    }
   }
}

unix {
  UI_DIR = .ui
  MOC_DIR = .moc
  OBJECTS_DIR = .obj
}

