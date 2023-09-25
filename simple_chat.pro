QT += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

CONFIG += console


# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    include/sqlist3/sqlite3.c \
    main.cpp \
    make_json.cpp \
    qarea_wid.cpp \
    qbar_line.cpp \
    qbutt_line.cpp \
    qedit_txt.cpp \
    qframe_line.cpp \
    qlab_img.cpp \
    qlab_wrap.cpp \
    qmove_pos.cpp \
    qsqlist.cpp \
    qweb_client.cpp \
#    qweb_socket.cpp \
    wid_chat.cpp \
    wid_chat_input.cpp \
    wid_chat_output.cpp \
    wid_friend_butt.cpp \
    wid_friend_info.cpp \
    wid_friend_list.cpp \
    wid_message.cpp \
    wid_msg_abs.cpp \
    wid_test.cpp

HEADERS += \
    Tvlog.h \
    file_version.h \
    include/hv/AsyncHttpClient.h \
    include/hv/Buffer.h \
    include/hv/Channel.h \
    include/hv/Event.h \
    include/hv/EventLoop.h \
    include/hv/EventLoopThread.h \
    include/hv/EventLoopThreadPool.h \
    include/hv/HttpClient.h \
    include/hv/HttpContext.h \
    include/hv/HttpMessage.h \
    include/hv/HttpParser.h \
    include/hv/HttpResponseWriter.h \
    include/hv/HttpServer.h \
    include/hv/HttpService.h \
    include/hv/Status.h \
    include/hv/TcpClient.h \
    include/hv/TcpServer.h \
    include/hv/ThreadLocalStorage.h \
    include/hv/UdpClient.h \
    include/hv/UdpServer.h \
    include/hv/WebSocketChannel.h \
    include/hv/WebSocketClient.h \
    include/hv/WebSocketParser.h \
    include/hv/WebSocketServer.h \
    include/hv/axios.h \
    include/hv/base64.h \
    include/hv/hasync.h \
    include/hv/hatomic.h \
    include/hv/hbase.h \
    include/hv/hbuf.h \
    include/hv/hconfig.h \
    include/hv/hdef.h \
    include/hv/hdir.h \
    include/hv/hendian.h \
    include/hv/herr.h \
    include/hv/hexport.h \
    include/hv/hfile.h \
    include/hv/hlog.h \
    include/hv/hloop.h \
    include/hv/hmain.h \
    include/hv/hmap.h \
    include/hv/hmath.h \
    include/hv/hmutex.h \
    include/hv/hobjectpool.h \
    include/hv/hpath.h \
    include/hv/hplatform.h \
    include/hv/hproc.h \
    include/hv/hscope.h \
    include/hv/hsocket.h \
    include/hv/hssl.h \
    include/hv/hstring.h \
    include/hv/hsysinfo.h \
    include/hv/hthread.h \
    include/hv/hthreadpool.h \
    include/hv/htime.h \
    include/hv/http_content.h \
    include/hv/httpdef.h \
    include/hv/hurl.h \
    include/hv/hv.h \
    include/hv/hversion.h \
    include/hv/ifconfig.h \
    include/hv/iniparser.h \
    include/hv/json.hpp \
    include/hv/md5.h \
    include/hv/nlog.h \
    include/hv/requests.h \
    include/hv/sha1.h \
    include/hv/singleton.h \
    include/hv/wsdef.h \
    include/nlohmann/json.hpp \
    include/nlohmann/json_fwd.hpp \
    include/sqlist3/sqlite3.h \
    make_json.h \
    qarea_wid.h \
    qbar_line.h \
    qbutt_line.h \
    qedit_txt.h \
    qframe_line.h \
    qlab_img.h \
    qlab_wrap.h \
    qmove_pos.h \
    qsqlist.h \
    qweb_client.h \
#    qweb_socket.h \
    web_protocol.h \
    wid_chat.h \
    wid_chat_input.h \
    wid_chat_output.h \
    wid_friend_butt.h \
    wid_friend_info.h \
    wid_friend_list.h \
    wid_message.h \
    wid_msg_abs.h \
    wid_test.h

#INCLUDEPATH += $$PWD/include/sqlist3




unix:!macx: LIBS += -L$$PWD/lib/ -lhv

INCLUDEPATH += $$PWD/include
DEPENDPATH += $$PWD/include



# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target


