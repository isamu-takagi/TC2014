QT += \
    widgets \
    opengl

INCLUDEPATH += \
    ../../include \
    ../../include/gndlib \

HEADERS += \
    camera.hpp \
    window.hpp \
    controller.hpp \
    main.hpp \
    widget-msg.hpp \
    widget-img.hpp \
    ssm-message.hpp \
    widget-gl.hpp \
    ../../include/tkglib/tkg-utility.hpp \
    ../../include/tkglib/tkg-opengl.hpp \
    ../../include/tkglib/tkg-geometry.hpp \
    ../../include/tkglib/tkg-config.hpp \
    menu-handler.hpp \
    map-viewer.hpp \
    ../../include/tkglib/tkg-debug.hpp \
    data-viewer.hpp

SOURCES += \
    main.cpp \
    window.cpp \
    camera.cpp \
    widget-gl.cpp \
    widget-msg.cpp \
    widget-img.cpp \
    ssm-message.cpp \
    menu-handler.cpp \
    map-viewer.cpp \
    data-viewer.cpp

LIBS += \
    -lGLU \
    -lssm

OTHER_FILES += \
    memo.txt
