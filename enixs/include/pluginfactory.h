//#############################################################################
//#  pluginfactory.h
//#  ===============
//#
//#  Creation:       Sun Apr 14 16:25:16 CEST 2002
//#  Author:         Ralf Schneider  (ralf.schneider@b-connected.de)
//#  Copyright:      (C) 2002 by B-connected Linux Solutions GmbH
//#
//#  ------------------------------------------------------------------------
//#
//#  This file contains the definition of the class CPluginFactory.
//#  This is the base factory class for all plugins.
//#
//#  ------------------------------------------------------------------------
//#
//#  This program is free software; you can redistribute it and/or modify it
//#  under the terms of the GNU General Public License as published by the
//#  Free Software Foundation; either version 2 of the License, or (at your
//#  option) any later version.
//#
//#############################################################################

#ifndef PLUGINFACTORY_H
#define PLUGINFACTORY_H

//=============================================================================
// Include files for QT.
//=============================================================================
#include <qstring.h>
//#include <qwidget.h>
#include <qpixmap.h>
#include <qlistview.h>

//=============================================================================
// Application specific includes.
//=============================================================================
#include "userdata.h"
#include <dbconnection.h>


#ifdef Q_WS_WIN
#  ifdef QT_PLUGIN
#    define QT_WIDGET_PLUGIN_EXPORT __declspec(dllexport)
#  else
#    define QT_WIDGET_PLUGIN_EXPORT __declspec(dllimport)
#  endif
#else
#  define QT_WIDGET_PLUGIN_EXPORT
#endif


class QT_WIDGET_PLUGIN_EXPORT CPluginFactory
{
public:
  CPluginFactory()
  {
  }
	
  virtual ~CPluginFactory()
  {
  }
	
  virtual void     setConnection  (CConnection* db=0, CUserData *current=0) = 0;
  virtual QWidget* create         (QWidget* parent=0, const char* name=0,
                                   int wflags=0, CConnection* db=0,
                                   CUserData *current=0) = 0;
  virtual void     offeredObjects (QListViewItem* item) = 0;

  virtual QString  name           () const = 0;
  virtual QString  group          () const = 0;
  virtual QPixmap  icon           () const = 0;
  virtual QString  toolTip        () const = 0;
  virtual QString  whatsThis      () const = 0;
  virtual QString  summary        () const = 0;
};

#endif
