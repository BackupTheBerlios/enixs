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
//#  This file contains the implentation of the class CEnixsEmailPlugin.
//#  This is the plugin class of the Email application of EniXs, which
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
#include "email.h"


//=============================================================================
// Constructor of the plugin class.
//=============================================================================
CEnixsEmailPlugin::CEnixsEmailPlugin()
{
}

//=============================================================================
// Create the plugin window.
//=============================================================================
QWidget* CEnixsEmailPlugin::create (QWidget* parent, const char* name, int wflags)
{
  return new CEmail (parent, name, wflags);
}

//=============================================================================
// Insert the offered objects of the plugin as children of the given QListViewItem.
//=============================================================================
void CEnixsEmailPlugin::offeredObjects (QListViewItem* item)
{
  CEmail::offeredObjects (item);
}

//=============================================================================
// Return the name of the plugin.
//=============================================================================
QString CEnixsEmailPlugin::name() const
{
  return CEmail::name();
}

//=============================================================================
// Return the group of the plugin.
//=============================================================================
QString CEnixsEmailPlugin::group() const
{
  return CEmail::group();
}

//=============================================================================
// Return the icon of the plugin.
//=============================================================================
QPixmap CEnixsEmailPlugin::icon() const
{
  return CEmail::icon();
}

//=============================================================================
// Return the tooltip text of the plugin.
//=============================================================================
QString CEnixsEmailPlugin::toolTip() const
{
  return CEmail::toolTip();
}

//=============================================================================
// Return the What's This text of the plugin.
//=============================================================================
QString CEnixsEmailPlugin::whatsThis() const
{
  return CEmail::whatsThis();
}

//=============================================================================
// Return the summary information of the plugin for the What's Up page.
//=============================================================================
QString CEnixsEmailPlugin::summary() const
{
  return CEmail::summary ();
}

//=============================================================================
// Create an object of the plugin class.
//=============================================================================
extern "C" CPluginFactory * factory ()
{
  return new CEnixsEmailPlugin;
}

