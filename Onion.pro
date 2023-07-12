#QT      += opengl

TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt


INCLUDEPATH += $$PWD/libraries/include/
#INCLUDEPATH+= $$PWD/libraries/x32/include/glad
#INCLUDEPATH+= $$PWD/libraries/x32/include/GLM
#INCLUDEPATH+= $$PWD/libraries/x32/include/STB
win32 {
#    INCLUDEPATH+= $$PWD/libraries/x32/include/GLFW
#    INCLUDEPATH+= $$PWD/libraries/x32/include/Assimp
    LIBS += -L$$PWD/libraries/x32/bin/glfw/ -lglfw3dll
    LIBS += -L$$PWD/libraries/x32/bin/Assimp/ -llibassimp.dll
}

win64 {
#    INCLUDEPATH+= $$PWD/libraries/x64/include/Assimp
#    INCLUDEPATH+= $$PWD/libraries/x64/include/GLFW
    LIBS += -L$$PWD/libraries/x64/bin/glfw/ -lglfw3dll
    LIBS += -L$$PWD/libraries/x64/bin/Assimp/ -llibassimp.dll
}

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

#win32: INCLUDEPATH+= $$PWD/libraries/x32/include/GLFW
#INCLUDEPATH += $$PWD/libraries/cpplibs/glfw-3.3.4.bin.WIN32/include
#INCLUDEPATH += $$PWD/../../libraries/cpplibs/glad/include
#INCLUDEPATH += $$PWD/../../libraries/cpplibs/GLM/include
#INCLUDEPATH += $$PWD/../../libraries/cpplibs/glm-master
#INCLUDEPATH += $$PWD/../../libraries/cpplibs/stb-master
#INCLUDEPATH += $$PWD/../../libraries/cpplibs/Assimp/Assimp_MinGW_32_release/include
#DEPENDPATH += $$PWD/../../libraries/cpplibs/Assimp/Assimp_MinGW_32_release/include
#DEPENDPATH += $$PWD/../../libraries/cpplibs/glfw-3.3.4.bin.WIN32/include
#win32: LIBS += -L$$PWD/../../libraries/cpplibs/glfw-3.3.4.bin.WIN32/lib-mingw/ -lglfw3dll
#win32: LIBS += -L$$PWD/../../libraries/cpplibs/Assimp/Assimp_MinGW_32_release/lib/ -llibassimp.dll
