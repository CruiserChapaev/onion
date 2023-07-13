#QT      += opengl

TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt

INCLUDEPATH += $$PWD/libraries/include/
win32 {
    LIBS += -L$$PWD/libraries/bin/glfw/x32/ -lglfw3dll
    LIBS += -L$$PWD/libraries/bin/Assimp/x32/ -llibassimp.dll
}

#win64 {
#    LIBS += -L$$PWD/libraries/bin/glfw/x64/ -lglfw3dll
#    LIBS += -L$$PWD/libraries/bin/Assimp/x64/ -llibassimp.dll
#}

SOURCES += \
    Mesh.cpp \
    camera.cpp \
    glad.c \
    main.cpp \
    model.cpp \
    shader.cpp

HEADERS += \
    Mesh.hpp \
    Texture.hpp \
    Vertex.hpp \
    camera.h \
    model.h \
    shader.h

