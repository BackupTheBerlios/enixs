//#############################################################################
//#  widgetfactory.cpp
//#  =================
//#
//#  Creation:       Sun Apr 14 16:25:16 CEST 2002
//#  Author:         Ralf Schneider  (ralf.schneider@b-connected.de)
//#  Copyright:      (C) 2002 by B-connected Linux Solutions GmbH
//#
//#  ------------------------------------------------------------------------
//#
//#  This file contains the implementation of the class CWidgetFactory.
//#  This is the factory class for creating a new widget object from a plugin.
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
#include <qlibrary.h>
#include <qmessagebox.h>

//=============================================================================
// Application specific includes.
//=============================================================================
#include "widgetfactory.h"

//=============================================================================
// Constructor of the widget factory class.
//=============================================================================
CWidgetFactory::CWidgetFactory (const QString & libpath)
{
  mLib = new QLibrary (libpath);
  factoryFunction f = (factoryFunction) mLib->resolve ("factory");

  if (f)
  	mFactory = (CPluginFactory *)f();
  else
    QMessageBox::critical	((QWidget*)0, QObject::tr ("Plugin Error"), 
                             QObject::tr ("The plugin ") + mLib->library() + 
                             QObject::tr (" could not be loaded."),
                             QMessageBox::Ok, QMessageBox::NoButton);
}

//=============================================================================
// Destructor of the widget factory class.
//=============================================================================
CWidgetFactory::~CWidgetFactory()
{
  delete mLib;
}

//=============================================================================
// Ask the plugin for the name of the widget it offers.
//=============================================================================
QString CWidgetFactory::name()
{
  if (mFactory)
    return mFactory->name();

  return 0;
}

//=============================================================================
// Ask the plugin for the group it belongs to.
//=============================================================================
QString CWidgetFactory::group()
{
  if (mFactory)
    return mFactory->group();
  
  return 0;
}

//=============================================================================
// Ask the plugin for its iconset.
//=============================================================================
QPixmap CWidgetFactory::icon()
{
  if (mFactory)
    return mFactory->icon();
  
  return QPixmap(0);
}

//=============================================================================
// Ask the plugin for its tool tip text.
//=============================================================================
QString CWidgetFactory::toolTip()
{
  if (mFactory)
    return mFactory->toolTip();
  
  return 0;
}

//=============================================================================
// Ask the plugin for its What's This text.
//=============================================================================
QString CWidgetFactory::whatsThis()
{
  if (mFactory)
    return mFactory->whatsThis();

  return 0;
}

//=============================================================================
// Ask the plugin for the summary info.
//=============================================================================
QString CWidgetFactory::summary()
{
  if (mFactory)
    return mFactory->summary();

  return 0;
}

//=============================================================================
// Create a new object of the class offered by the plugin.
//=============================================================================
QWidget * CWidgetFactory::create (QWidget* parent, const char *name, int wflags)
{
  if (mFactory)
    return mFactory->create (parent, name, wflags);

  return 0;
}
