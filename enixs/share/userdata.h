//#############################################################################
//#  userdata.h
//#  ==========
//#
//#  Creation:       Tue May 14 2002
//#  Author:         Ralf Schneider  (ralf.schneider@b-connected.de)
//#  Copyright:      (C) 2002 by B-connected Linux Solutions GmbH
//#
//#  ------------------------------------------------------------------------
//#
//#  This file contains the definition of the class CUserData which
//#  holds some information about the user currently logged in.
//#
//#  ------------------------------------------------------------------------
//#
//#  This program is free software; you can redistribute it and/or modify it
//#  under the terms of the GNU General Public License as published by the
//#  Free Software Foundation; either version 2 of the License, or (at your
//#  option) any later version.
//#
//#############################################################################

#ifndef USERDATA_H
#define USERDATA_H

//=============================================================================
// Include files for QT.
//=============================================================================
#include <qstring.h>
#include <qstringlist.h>
#include <qwidget.h>


class CUserData : public QWidget
{
public: 
	CUserData();
	~CUserData();

	QString     id      () { return mID; }
	QString     name    () { return mName; }
	QString     role    () { return mRole; }
	QStringList groups  () { return mGroups; }
	QString     comment () { return mComment; }

	bool    getUserData     (QString user);

private:
	QString      mID;
	QString      mName;
	QString      mRole;
	QStringList  mGroups;
	QString      mComment;
};

#endif
