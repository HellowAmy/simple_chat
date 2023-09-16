QT += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    qarea_wid.cpp \
    qbar_line.cpp \
    qbutt_line.cpp \
    qedit_txt.cpp \
    qframe_line.cpp \
    qlab_img.cpp \
    qlab_wrap.cpp \
    qmove_pos.cpp \
    qsqlist.cpp \
    third_party/sqlite3/sqlite3.c \
    wid_chat.cpp \
    wid_chat_input.cpp \
#    wid_chat_list.cpp \
    wid_chat_output.cpp \
    wid_friend_butt.cpp \
    wid_friend_info.cpp \
    wid_friend_list.cpp \
    wid_message.cpp \
    wid_msg_abs.cpp \
    wid_test.cpp

HEADERS += \
    Tvlog.h \
    qarea_wid.h \
    qbar_line.h \
    qbutt_line.h \
    qedit_txt.h \
    qframe_line.h \
    qlab_img.h \
    qlab_wrap.h \
    qmove_pos.h \
    qsqlist.h \
    third_party/sqlite3/sqlite3.h \
    wid_chat.h \
    wid_chat_input.h \
#    wid_chat_list.h \
    wid_chat_output.h \
    wid_friend_butt.h \
    wid_friend_info.h \
    wid_friend_list.h \
    wid_message.h \
    wid_msg_abs.h \
    wid_test.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
