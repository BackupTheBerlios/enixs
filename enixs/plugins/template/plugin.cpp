//#############################################################################
//#  plugin.cpp
//#  ==========
//#
//#  Creation:       Mon Apr 22 23:35:16 CEST 2002
//#  Author:         Ralf Schneider  (ralf.schneider@b-connected.de)
//#  Copyright:      (C) 2002 by B-connected Linux Solutions GmbH
//#
//#  ------------------------------------------------------------------------
//#
//#  This file contains the implentation of the class CEnixsTemplatePlugin.
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
#include <qobject.h>

//=============================================================================
// Application specific includes.
//=============================================================================
#include "plugin.h"
#include "template.h"


//=============================================================================
// Constructor of the plugin class.
//=============================================================================
CEnixsTemplatePlugin::CEnixsTemplatePlugin()
{
}

//=============================================================================
// Set the connection data and load the language.
//=============================================================================
void CEnixsTemplatePlugin::setConnection (CConnection *db, CUserData* current)
{
  mDB   = db;
  mUser = current;
}

//=============================================================================
// Create the plugin window.
//=============================================================================
QWidget* CEnixsTemplatePlugin::create (QWidget* parent, const char* name, 
                                       int wflags, CConnection *db, 
                                       CUserData* current)
{
  return new CTemplate (parent, name, wflags, db, current);
}

//=============================================================================
// Insert the offered objects of the plugin as children of the given QListViewItem.
//=============================================================================
void CEnixsTemplatePlugin::offeredObjects (QListViewItem* item)
{
  CTemplate::offeredObjects (item, mDB, mUser);
}

//=============================================================================
// Return the name of the plugin.
//=============================================================================
QString CEnixsTemplatePlugin::name() const
{
  return CTemplate::name();
}

//=============================================================================
// Return the group of the plugin.
//=============================================================================
QString CEnixsTemplatePlugin::group() const
{
  return CTemplate::group();
}

//=============================================================================
// Return the icon of the plugin.
//=============================================================================
QPixmap CEnixsTemplatePlugin::icon() const
{
  return CTemplate::icon();
}

//=============================================================================
// Return the tooltip text of the plugin.
//=============================================================================
QString CEnixsTemplatePlugin::toolTip() const
{
  return CTemplate::toolTip();
}

//=============================================================================
// Return the What's This text of the plugin.
//=============================================================================
QString CEnixsTemplatePlugin::whatsThis() const
{
  return CTemplate::whatsThis();
}

//=============================================================================
// Return the summary information of the plugin for the What's Up page.
//=============================================================================
QString CEnixsTemplatePlugin::summary() const
{
  return CTemplate::summary ();
}

//=============================================================================
// Create an object of the plugin class.
//=============================================================================
extern "C" CPluginFactory * factory ()
{
  return new CEnixsTemplatePlugin;
}

