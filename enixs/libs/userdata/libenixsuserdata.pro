TEMPLATE		= lib
CONFIG	    	= qt warn_on debug dll
HEADERS			= userdata.h
SOURCES			= userdata.cpp
TARGET          = enixsuserdata
VERSION         = 1
INCLUDEPATH     = $(QTDIR)/include /opt/sapdb/interfaces/odbc/incl ../../include
LIBS            = -lsapdb
includes.path   = ../../include
includes.files  = userdata.h
target.path     = ..
INSTALLS       += target includes
