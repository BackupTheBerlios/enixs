SOURCES  += plugin.cpp contacts.cpp
SOURCES  += general/general.cpp communication/communication.cpp address/address.cpp
SOURCES  += job/job.cpp private/private.cpp security/security.cpp
SOURCES  += friendlist/friendlist.cpp
SOURCES  += widgets/commline.cpp widgets/addressbox.cpp widgets/jobbox.cpp
SOURCES  += ../../share/validators/datevalidator.cpp
SOURCES  += ../../share/validators/phonevalidator.cpp
SOURCES  += ../../share/widgets/textedit.cpp

HEADERS  += plugin.h contacts.h
HEADERS  += general/general.h communication/communication.h address/address.h
HEADERS  += job/job.h private/private.h security/security.h
HEADERS  += friendlist/friendlist.h
HEADERS  += widgets/commline.h widgets/addressbox.h widgets/jobbox.h
HEADERS  += ../../share/validators/datevalidator.h 
HEADERS  += ../../share/validators/phonevalidator.h
HEADERS  += ../../share/widgets/textedit.h

DESTDIR   = ..
TARGET    = contacts

TRANSLATIONS = contacts_en.ts contacts_de.ts

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
