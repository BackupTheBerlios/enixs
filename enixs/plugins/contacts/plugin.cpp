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
//#  This file contains the implentation of the class CEnixsContactsPlugin.
//#  This is the plugin class of the contacts management of eniXs.
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
#include "contacts.h"


//=============================================================================
// Constructor of the plugin class.
//=============================================================================
CEnixsContactsPlugin::CEnixsContactsPlugin()
{
}

//=============================================================================
// Set the connection data and load the language.
//=============================================================================
void CEnixsContactsPlugin::setConnection (CConnection *db, CUserData* current)
{
  mDB   = db;
  mUser = current;
}

//=============================================================================
// Create the plugin window.
//=============================================================================
QWidget* CEnixsContactsPlugin::create (QWidget* parent, const char* name, 
                                       int wflags, CConnection *db, 
                                       CUserData* current)
{
  return new CContacts (parent, name, wflags, db, current);
}

//=============================================================================
// Insert the offered objects of the plugin as children of the given QListViewItem.
//=============================================================================
void CEnixsContactsPlugin::offeredObjects (QListViewItem* item)
{
  CContacts::offeredObjects (item, mDB, mUser);
}

//=============================================================================
// Return the name of the plugin.
//=============================================================================
QString CEnixsContactsPlugin::name() const
{
  return CContacts::name();
}

//=============================================================================
// Return the group of the plugin.
//=============================================================================
QString CEnixsContactsPlugin::group() const
{
  return CContacts::group();
}

//=============================================================================
// Return the icon of the plugin.
//=============================================================================
QPixmap CEnixsContactsPlugin::icon() const
{
  return CContacts::icon();
}

//=============================================================================
// Return the tooltip text of the plugin.
//=============================================================================
QString CEnixsContactsPlugin::toolTip() const
{
  return CContacts::toolTip();
}

//=============================================================================
// Return the What's This text of the plugin.
//=============================================================================
QString CEnixsContactsPlugin::whatsThis() const
{
  return CContacts::whatsThis();
}

//=============================================================================
// Return the summary information of the plugin for the What's Up page.
//=============================================================================
QString CEnixsContactsPlugin::summary() const
{
  return CContacts::summary ();
}

//=============================================================================
// Create an object of the plugin class.
//=============================================================================
extern "C" CPluginFactory * factory ()
{
  return new CEnixsContactsPlugin;
}

