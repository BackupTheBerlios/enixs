TEMPLATE		= lib
CONFIG	    	= qt warn_on debug dll
HEADERS			= util.h
SOURCES			= util.cpp
TARGET          = enixsutil
VERSION         = 1
INCLUDEPATH     = $(QTDIR)/include
includes.path   = ../../include
includes.files  = util.h
target.path     = ..
INSTALLS       += target includes
