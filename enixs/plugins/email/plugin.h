//#############################################################################
//#  plugin.h
//#  ========
//#
//#  Creation:       Thu Jun 27 11:35:16 CEST 2002
//#  Author:         Sven Alisch  (svenali@t-online.de)
//#  Copyright:      (C) 2002 by Sven Alisch
//#
//#  ------------------------------------------------------------------------
//#
//#  This file contains the implentation of the class CEnixsEmailPlugin.
//#  This is the plugin class of the calendar application of eniXs.
//#
//#  ------------------------------------------------------------------------
//#
//#  This program is free software; you can redistribute it and/or modify it
//#  under the terms of the GNU General Public License as published by the
//#  Free Software Foundation; either version 2 of the License, or (at your
//#  option) any later version.
//#
//#############################################################################

#ifndef PLUGIN_H
#define PLUGIN_H

//=============================================================================
// Application specific includes.
//=============================================================================
#include "pluginfactory.h"
#include "userdata.h"
//#include <dbconnection.h>

//=============================================================================
// Include files for QT.
//=============================================================================
#include <qlistview.h>
#include <qsqldatabase.h>

class QT_WIDGET_PLUGIN_EXPORT CEnixsEmailPlugin : public CPluginFactory
{
public:
  CEnixsEmailPlugin();

  QString     name        () const;
  QWidget*    create      (QWidget* parent=0, const char* name=0, int wflags=0);
  
  void        offeredObjects (QListViewItem* item);
  QString     group       () const;
  QPixmap     icon        () const;
  QString     toolTip     () const;
  QString     whatsThis   () const;
  QString     summary     () const;

private:
  QSqlDatabase*   mDB;
  CUserData*      mUser;
};

#endif
