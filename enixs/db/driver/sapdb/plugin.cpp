//#############################################################################
//#  plugin.cpp
//#  ==========
//#
//#  Creation:       Mon Jul 08 17:39:16 CEST 2002
//#  Author:         Ralf Schneider  (ralf.schneider@b-connected.de)
//#  Copyright:      (C) 2002 by B-connected Linux Solutions GmbH
//#
//#  ------------------------------------------------------------------------
//#
//#  This file contains the implementation of the class QSAPDBCDriverPlugin.
//#  This is the plugin class that implements the interface for being used
//#  as a database driver from within the Qt toolkit.
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
#include "driver.h"

//=============================================================================
// Constructor of the plugin class.
//=============================================================================
QSAPDBDriverPlugin::QSAPDBDriverPlugin()
    : QSqlDriverPlugin()
{
}

//=============================================================================
// Create a new SAPDB driver object.
//=============================================================================
QSqlDriver* QSAPDBDriverPlugin::create (const QString &name)
{
  if (name == "QSAPDB7") 
    return new QSAPDBDriver();

  return 0;
}

//=============================================================================
// Return the list of driver keys that are supported by the plugin.
//=============================================================================
QStringList QSAPDBDriverPlugin::keys() const
{
  QStringList keys;
  keys << QString ("QSAPDB7");
  return keys;
}
