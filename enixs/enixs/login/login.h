//#############################################################################
//#  login.h
//#  =======
//#
//#  Creation:       Mit Mai  1 14:35:16 CEST 2002
//#  Author:         Ralf Schneider  (ralf.schneider@b-connected.de)
//#  Copyright:      (C) 2002 by B-connected Linux Solutions GmbH
//#
//#  ------------------------------------------------------------------------
//#
//#  This file contains the definition of the class CLogin that displays
//#  the login dialog during startup and authorizes the user to the system.
//#
//#  ------------------------------------------------------------------------
//#
//#  This program is free software; you can redistribute it and/or modify it
//#  under the terms of the GNU General Public License as published by the
//#  Free Software Foundation; either version 2 of the License, or (at your
//#  option) any later version.
//#
//#############################################################################

#ifndef LOGIN_H
#define LOGIN_H

//------------------------------------------------------------------------------
//  QT Includes
//------------------------------------------------------------------------------
#include <qdialog.h>
#include <qlineedit.h>


class CLogin : public QDialog
{
  Q_OBJECT
  
public:
  CLogin (QWidget *parent = 0, const char *name=0);
  ~CLogin();

  QString   username ();
  QString   password ();
  QString   database ();
  QString   hostname ();

  void setPassword   (QString text);

protected slots:
	void slotHelp ();

private:
  QLineEdit   *mUser;
  QLineEdit   *mPass;
  QLineEdit   *mDB;
  QLineEdit   *mHost;
};

#endif 

