SOURCES  += plugin.cpp email.cpp
HEADERS  += plugin.h email.h
DESTDIR   = ..
TARGET    = email

TRANSLATIONS = email_en.ts email_de.ts

target.path=$$plugins.path
isEmpty(target.path):target.path=$$QT_PREFIX/plugins
INSTALLS    += target
TEMPLATE     = lib
CONFIG      += qt warn_on debug plugin thread
INCLUDEPATH += $(QTDIR)/tools/designer/interfaces ../../include
INCLUDEPATH += ../../share ../../share/validators ../../share/widgets
INCLUDEPATH += /opt/sapdb/interfaces/odbc/incl
OBJECTS_DIR  = obj
MOC_DIR      = obj
DBFILE       = plugin.db
LANGUAGE     = C++
