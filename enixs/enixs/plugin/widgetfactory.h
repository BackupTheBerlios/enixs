//#############################################################################
//#  widgetfactory.h
//#  ===============
//#
//#  Creation:       Sun Apr 14 16:25:16 CEST 2002
//#  Author:         Ralf Schneider  (ralf.schneider@b-connected.de)
//#  Copyright:      (C) 2002 by B-connected Linux Solutions GmbH
//#
//#  ------------------------------------------------------------------------
//#
//#  This file contains the definition of the class CWidgetFactory.
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

#ifndef WIDGETFACTORY_H
#define WIDGETFACTORY_H

//=============================================================================
// Include files for QT.
//=============================================================================
#include <qwidget.h>
#include <qlibrary.h>

//=============================================================================
// Application specific includes.
//=============================================================================
#include "pluginfactory.h"


typedef CPluginFactory * (*factoryFunction) ();


class CWidgetFactory
{
public:
  CWidgetFactory (const QString & libpath);
  ~CWidgetFactory();
	
  QString  name   	  ();
  QString  group  	  ();
  QPixmap  icon   	  ();
  QString  toolTip	  ();
  QString  whatsThis  ();
  QString  summary    ();
  QWidget* create 	  (QWidget* parent, const char *name, int wflags);

private:
  QLibrary*			mLib;
  CPluginFactory*	mFactory;
};

#endif
