#TEMPLATE = app
#CONFIG += console c++17
#CONFIG -= app_bundle
##CONFIG -= qt

QT += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

CONFIG += console



SOURCES += \
        ../../include/sqlist3/sqlite3.c \
        ../../qweb_client.cpp \
        main.cpp

HEADERS += \
    ../../include/hv/AsyncHttpClient.h \
    ../../include/hv/Buffer.h \
    ../../include/hv/Channel.h \
    ../../include/hv/Event.h \
    ../../include/hv/EventLoop.h \
    ../../include/hv/EventLoopThread.h \
    ../../include/hv/EventLoopThreadPool.h \
    ../../include/hv/HttpClient.h \
    ../../include/hv/HttpContext.h \
    ../../include/hv/HttpMessage.h \
    ../../include/hv/HttpParser.h \
    ../../include/hv/HttpResponseWriter.h \
    ../../include/hv/HttpServer.h \
    ../../include/hv/HttpService.h \
    ../../include/hv/Status.h \
    ../../include/hv/TcpClient.h \
    ../../include/hv/TcpServer.h \
    ../../include/hv/ThreadLocalStorage.h \
    ../../include/hv/UdpClient.h \
    ../../include/hv/UdpServer.h \
    ../../include/hv/WebSocketChannel.h \
    ../../include/hv/WebSocketClient.h \
    ../../include/hv/WebSocketParser.h \
    ../../include/hv/WebSocketServer.h \
    ../../include/hv/axios.h \
    ../../include/hv/base64.h \
    ../../include/hv/hasync.h \
    ../../include/hv/hatomic.h \
    ../../include/hv/hbase.h \
    ../../include/hv/hbuf.h \
    ../../include/hv/hconfig.h \
    ../../include/hv/hdef.h \
    ../../include/hv/hdir.h \
    ../../include/hv/hendian.h \
    ../../include/hv/herr.h \
    ../../include/hv/hexport.h \
    ../../include/hv/hfile.h \
    ../../include/hv/hlog.h \
    ../../include/hv/hloop.h \
    ../../include/hv/hmain.h \
    ../../include/hv/hmap.h \
    ../../include/hv/hmath.h \
    ../../include/hv/hmutex.h \
    ../../include/hv/hobjectpool.h \
    ../../include/hv/hpath.h \
    ../../include/hv/hplatform.h \
    ../../include/hv/hproc.h \
    ../../include/hv/hscope.h \
    ../../include/hv/hsocket.h \
    ../../include/hv/hssl.h \
    ../../include/hv/hstring.h \
    ../../include/hv/hsysinfo.h \
    ../../include/hv/hthread.h \
    ../../include/hv/hthreadpool.h \
    ../../include/hv/htime.h \
    ../../include/hv/http_content.h \
    ../../include/hv/httpdef.h \
    ../../include/hv/hurl.h \
    ../../include/hv/hv.h \
    ../../include/hv/hversion.h \
    ../../include/hv/ifconfig.h \
    ../../include/hv/iniparser.h \
    ../../include/hv/json.hpp \
    ../../include/hv/md5.h \
    ../../include/hv/nlog.h \
    ../../include/hv/requests.h \
    ../../include/hv/sha1.h \
    ../../include/hv/singleton.h \
    ../../include/hv/wsdef.h \
    ../../include/sqlist3/sqlite3.h \
    ../../qweb_client.h

unix:!macx: LIBS += -L$$PWD/../../lib/ -lhv

INCLUDEPATH += $$PWD/../../include
DEPENDPATH += $$PWD/../../include
