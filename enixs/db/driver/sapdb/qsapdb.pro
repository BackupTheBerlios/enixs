SOURCES	   += plugin.cpp driver.cpp result.cpp util.cpp
HEADERS	   += plugin.h driver.h result.h util.h
TARGET		= qsqlsapdb
TEMPLATE	= lib
CONFIG	   += qt warn_on plugin debug
INSTALLS   += target

unix {
  OBJECTS_DIR	= obj
  LIBS 		   += -L/opt/sapdb/interfaces/odbc/lib -lsqlod
  INCLUDEPATH  += /opt/sapdb/interfaces/odbc/incl /opt/sapdb/depend/incl
}
isEmpty(plugins.path):plugins.path=/usr/lib/qt3/plugins
target.path += $$plugins.path/sqldrivers
