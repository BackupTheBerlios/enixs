//#############################################################################
//#  plugindef.h
//#  ===========
//#
//#  Creation:       Sun Apr  7 16:25:16 CEST 2002
//#  Author:         Ralf Schneider  (ralf.schneider@b-connected.de)
//#  Copyright:      (C) 2002 by B-connected Linux Solutions GmbH
//#
//#  ------------------------------------------------------------------------
//#
//#  This file contains the definition of the class CPluginDef.
//#  This class contains the description of an installed plugin.
//#
//#  ------------------------------------------------------------------------
//#
//#  This program is free software; you can redistribute it and/or modify it
//#  under the terms of the GNU General Public License as published by the
//#  Free Software Foundation; either version 2 of the License, or (at your
//#  option) any later version.
//#
//#############################################################################

#ifndef PLUGINDEF_H
#define PLUGINDEF_H

//=============================================================================
// Include files for QT.
//=============================================================================
#include <qobject.h>
#include <qstring.h>
#include <qpixmap.h>


class CPluginDef : public QObject
{
  Q_OBJECT

public:
  CPluginDef ();
  ~CPluginDef();

	void setLib				(const QString text)   		{ mLibName = text; }
	void setName			(const QString text)   		{ mPluginName = text; }
	void setGroup			(const QString text)   		{ mGroupName = text; }
	void setIcon   		(const QPixmap icon)      { mIcon = icon; }
	void setToolTip		(const QString text)   		{ mToolTip = text; }
	void setWhatsThis (const QString text)   		{ mWhatsThis = text; }

	QString			lib()				{ return mLibName; }
	QString			name()			{ return mPluginName; }
	QString			group()			{ return mGroupName; }
	QPixmap 		icon() 	    { return mIcon; }
	QString			toolTip()		{ return mToolTip; }
	QString			whatsThis()	{ return mWhatsThis; }

private:
	QString			    mLibName;
	QString			    mPluginName;
	QString			    mGroupName;
	QPixmap 		    mIcon;
	QString			    mToolTip;
	QString			    mWhatsThis;
};

#endif
