//#############################################################################
//#  plugin.cpp
//#  ==========
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

//=============================================================================
// Application specific includes.
//=============================================================================
#include "plugin.h"
#include "email.h"

CEnixsEmailPlugin::CEnixsEmailPlugin()
{
}

QString CEnixsEmailPlugin::name() const
{
  return CEmail::name();
}

QWidget* CEnixsEmailPlugin::create (QWidget* parent, const char* name, int wflags)
{
  return new CEmail (parent, name, wflags);
}

QString CEnixsEmailPlugin::group() const
{
  return CEmail::group();
}

QPixmap CEnixsEmailPlugin::icon() const
{
  return CEmail::icon();
}

QString CEnixsEmailPlugin::toolTip() const
{
  return CEmail::toolTip();
}

QString CEnixsEmailPlugin::whatsThis() const
{
  return CEmail::whatsThis();
}

QString CEnixsEmailPlugin::summary() const
{
  return CEmail::summary ();
}

//=============================================================================
// Insert the offered objects of the plugin as children of the given QListViewItem.
//=============================================================================
void CEnixsEmailPlugin::offeredObjects (QListViewItem* item)
{
  // mach erst mal nischt ... ;
}

extern "C" CPluginFactory * factory ()
{
  return new CEnixsEmailPlugin;
}

