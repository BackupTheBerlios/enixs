//#############################################################################
//#  login.cpp
//#  =========
//#
//#  Creation:       Mit Mai  1 14:35:16 CEST 2002
//#  Author:         Ralf Schneider  (ralf.schneider@b-connected.de)
//#  Copyright:      (C) 2002 by B-connected Linux Solutions GmbH
//#
//#  ------------------------------------------------------------------------
//#
//#  This file contains the implementation of the class CLogin that displays
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

//=============================================================================
// Include files for QT.
//=============================================================================
#include <qpushbutton.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qpixmap.h>
#include <qlayout.h>
#include <qhbox.h>

//=============================================================================
// Application specific includes.
//=============================================================================
#include "login.h"
//#include "../util.h"
#include "../bitmaps/enixs50.xpm"


//==============================================================================
// Constructor for the class CLogin.
//==============================================================================
CLogin::CLogin (QWidget *parent, const char *name)
    : QDialog (parent, name, TRUE)
{
  QGridLayout *grid;
  QPushButton *ok, *cancel, *help;
  QLabel      *picture, *instruction;
  QLabel      *label1, *label2, *label3, *label4;
  
  setCaption  (tr ("EniXs Login"));

  //----------------------------------------------------------------------------
  // Create the grid layout.
  //----------------------------------------------------------------------------
  grid = new QGridLayout (this, 8, 4, 10, 10);
  grid->addRowSpacing    (1, 20);
  grid->addRowSpacing    (6, 20);
  grid->addColSpacing    (3, 10);

  //----------------------------------------------------------------------------
  // Create bitmap.
  //----------------------------------------------------------------------------
  picture = new QLabel     (this);
  picture->setPixmap       (QPixmap (enixs50));
  grid->addMultiCellWidget (picture, 0, 0, 0, 0, AlignCenter);

  //----------------------------------------------------------------------------
  // Create the login instruction.
  //----------------------------------------------------------------------------
  instruction = new QLabel (tr ("Please enter your username and your "
                            "\npassword to log into the system."), this);
  grid->addMultiCellWidget (instruction, 0, 0, 1, 3);

  //----------------------------------------------------------------------------
  // Create the line for the username.
  //----------------------------------------------------------------------------
  label1 = new QLabel      (tr ("Username"), this);
  grid->addMultiCellWidget (label1, 2, 2, 0, 0);

  mUser  = new QLineEdit   (this, "username");
  grid->addMultiCellWidget (mUser, 2, 2, 1, 3);
	mUser->setMaxLength      (32);
  mUser->setFocus          ();

  //----------------------------------------------------------------------------
  // Create the line for the password.
  //----------------------------------------------------------------------------
  label2 = new QLabel      (tr ("Password"), this);
  grid->addMultiCellWidget (label2, 3, 3, 0, 0);

  mPass  = new QLineEdit   (this, "password");
  grid->addMultiCellWidget (mPass, 3, 3, 1, 3);
	mPass->setMaxLength      (32);
  mPass->setEchoMode       (QLineEdit::Password);

  //----------------------------------------------------------------------------
  // Create the line for the database.
  //----------------------------------------------------------------------------
  label3 = new QLabel      (tr ("Database"), this);
  grid->addMultiCellWidget (label3, 4, 4, 0, 0);

  mDB    = new QLineEdit   (this, "database");
  grid->addMultiCellWidget (mDB, 4, 4, 1, 3);
	mDB->setMaxLength 	     (8);
  mDB->setText             ("ENIXSDB");

  //----------------------------------------------------------------------------
  // Create the line for the hostname.
  //----------------------------------------------------------------------------
  label4 = new QLabel      (tr ("Server"), this);
  grid->addMultiCellWidget (label4, 5, 5, 0, 0);

  mHost   = new QLineEdit  (this, "host");
  grid->addMultiCellWidget (mHost, 5, 5, 1, 3);
	mHost->setMaxLength      (32);
//  mHost->setText          ("db001");

  //----------------------------------------------------------------------------
  // Create the buttons.
  //----------------------------------------------------------------------------
  ok = new QPushButton     (tr ("OK"), this);
  grid->addMultiCellWidget (ok, 7, 7, 0, 0);
  ok->setFixedSize         (90, 25);
  ok->setDefault           (true);
  connect                  (ok, SIGNAL (clicked()), SLOT (accept()));

  cancel = new QPushButton (tr ("Cancel"), this);
  grid->addMultiCellWidget (cancel, 7, 7, 1, 1);
  cancel->setFixedSize     (90, 25);
  connect                  (cancel, SIGNAL (clicked()), SLOT (reject()));

  help = new QPushButton   (tr ("Help"), this);
  grid->addMultiCellWidget (help, 7, 7, 3, 3);
  help->setFixedSize  	   (80, 25);
  connect                  (help, SIGNAL (clicked()), SLOT (slotHelp()));
}


//==============================================================================
// Destructor for the class CLogin.
//==============================================================================
CLogin::~CLogin()
{
}


//==============================================================================
// Returns the entered username.
//==============================================================================
QString CLogin::username()
{
  return mUser->text();
}


//==============================================================================
// Returns the entered password.
//==============================================================================
QString CLogin::password()
{
  return mPass->text();
}


//==============================================================================
// Set the password in the corresponding field.
//==============================================================================
void CLogin::setPassword(QString text)
{
  mPass->setText(text);
}


//==============================================================================
// Returns the entered database.
//==============================================================================
QString CLogin::database()
{
  return mDB->text();
}


//==============================================================================
// Returns the entered hostname.
//==============================================================================
QString CLogin::hostname()
{
  return mHost->text();
}

//=============================================================================
// SLOT: Help
//=============================================================================
void CLogin::slotHelp()
{
//	QStringList		prefixes;
//	QString				file;
//	int  					i;
//
//  //----------------------------------------------------------------------------
//  // Set the URL of help index.html.
//  //----------------------------------------------------------------------------
//	prefixes = QStringList::split (":", kapp->dirs()->kfsstnd_prefixes());
//	for (i = 0; i < (int)prefixes.count(); i++)
//	{
//		file = prefixes[i] + kapp->dirs()->kde_default("html") +
//					 QStringList::split (":", KGlobal::locale()->languages())[0] + "/enixs/login.html";
//
//		if (kapp->dirs()->exists(file))
//			break;
//	}
//
//  //----------------------------------------------------------------------------
//  // Show the HTML file.
//  //----------------------------------------------------------------------------
//	viewFile (file);
}

