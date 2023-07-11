#QT      += opengl

TEMPLATE = app
#CONFIG += console

CONFIG += c++11

LIBS += -lopengl32

INCLUDEPATH += $$PWD/../../libraries/cpplibs/glfw-3.3.4.bin.WIN32/include
INCLUDEPATH += $$PWD/../../libraries/cpplibs/glad/include
INCLUDEPATH += $$PWD/../../libraries/cpplibs/GLM/include
INCLUDEPATH += $$PWD/../../libraries/cpplibs/glm-master
INCLUDEPATH += $$PWD/../../libraries/cpplibs/stb-master
INCLUDEPATH += $$PWD/../../libraries/cpplibs/Assimp/Assimp_MinGW_32_release/include

DEPENDPATH += $$PWD/../../libraries/cpplibs/Assimp/Assimp_MinGW_32_release/include
DEPENDPATH += $$PWD/../../libraries/cpplibs/glfw-3.3.4.bin.WIN32/include

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

win32: LIBS += -L$$PWD/../../libraries/cpplibs/glfw-3.3.4.bin.WIN32/lib-mingw/ -lglfw3dll
win32: LIBS += -L$$PWD/../../libraries/cpplibs/Assimp/Assimp_MinGW_32_release/lib/ -llibassimp.dll
