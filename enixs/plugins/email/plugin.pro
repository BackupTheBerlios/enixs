SOURCES  += plugin.cpp email.cpp cpop.cpp mimecodec.cpp
HEADERS  += plugin.h email.h cpop.h mimecodec.h
DESTDIR   = ..
TARGET    = email 

target.path=$$plugins.path
isEmpty(target.path):target.path=$$QT_PREFIX/plugins
INSTALLS    += target
TEMPLATE     = lib
CONFIG      += qt warn_on debug plugin thread
INCLUDEPATH += $(QTDIR)/tools/designer/interfaces ../../include
INCLUDEPATH += /opt/sapdb/interfaces/odbc/incl
DBFILE       = plugin.db
LANGUAGE     = C++
