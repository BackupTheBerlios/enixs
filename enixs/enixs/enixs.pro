SOURCES  += main.cpp enixs.cpp enixsview.cpp
SOURCES  += login/login.cpp plugin/widgetfactory.cpp plugindef/plugindef.cpp
SOURCES  += tabpage/tabpage.cpp tabpage/pluginbutton.cpp whatsup/whatsup.cpp

HEADERS  += enixs.h enixsview.h resource.h
HEADERS  += login/login.h plugin/widgetfactory.h plugindef/plugindef.h
HEADERS  += tabpage/tabpage.h tabpage/pluginbutton.h whatsup/whatsup.h

TARGET    = enixs

TRANSLATIONS = enixs_en.ts enixs_de.ts

target.path  = /usr/local/enixs
INSTALLS    += target
TEMPLATE     = app
CONFIG      += qt warn_on debug thread
INCLUDEPATH += $(QTDIR)/tools/designer/interfaces ../share ../include
INCLUDEPATH += /opt/sapdb/interfaces/odbc/incl
LIBS         = -lsapdb -L/usr/lib -lenixsuserdata -l enixsutil -L../libs
OBJECTS_DIR  = obj
MOC_DIR      = obj
DBFILE       = enixs.db
LANGUAGE     = C++
