//#############################################################################
//#  plugin.h
//#  ========
//#
//#  Creation:       Mon Apr 22 23:35:16 CEST 2002
//#  Author:         Ralf Schneider  (ralf.schneider@b-connected.de)
//#  Copyright:      (C) 2002 by B-connected Linux Solutions GmbH
//#
//#  ------------------------------------------------------------------------
//#
//#  This file contains the definition of the class CEnixsTemplatePlugin.
//#  This is the plugin class of the Template application of EniXs, which
//#  demonstrates howto implement your own EniXs plugins.
//#
//#  ------------------------------------------------------------------------
//#
//#  This program is free software; you can redistribute it and/or modify it
//#  under the terms of the GNU General Public License as published by the
//#  Free Software Foundation; either version 2 of the License, or (at your
//#  option) any later version.
//#
//#############################################################################

//=============================================================================
// Include files for QT.
//=============================================================================
#include <qlistview.h>
#include <qsqldatabase.h>

//=============================================================================
// Application specific includes.
//=============================================================================
#include "pluginfactory.h"
#include "userdata.h"


class QT_WIDGET_PLUGIN_EXPORT CEnixsTemplatePlugin : public CPluginFactory
{
public:
  CEnixsTemplatePlugin();

  QWidget*    create         (QWidget* parent=0, const char* name=0, int wflags=0);
  void        offeredObjects (QListViewItem* item);
  
  QString     name           () const;
  QString     group          () const;
  QPixmap     icon           () const;
  QString     toolTip        () const;
  QString     whatsThis      () const;
  QString     summary        () const;

private:
  QSqlDatabase*   mDB;
  CUserData*      mUser;
};
