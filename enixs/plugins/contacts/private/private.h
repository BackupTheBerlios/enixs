//#############################################################################
//#  private.h
//#  =========
//#
//#  Creation:       Sat Jun 08 23:10:16 CEST 2002
//#  Author:         Ralf Schneider  (ralf.schneider@b-connected.de)
//#  Copyright:      (C) 2002 by B-connected Linux Solutions GmbH
//#
//#  ------------------------------------------------------------------------
//#
//#  This file contains the definition of the class CPrivate.
//#  This class contains all the widgets for displaying the private details
//#  of a contact.
//#
//#  ------------------------------------------------------------------------
//#
//#  This program is free software; you can redistribute it and/or modify it
//#  under the terms of the GNU General Public License as published by the
//#  Free Software Foundation; either version 2 of the License, or (at your
//#  option) any later version.
//#
//#############################################################################

#ifndef PRIVATE_H
#define PRIVATE_H

//=============================================================================
// Include files for QT.
//=============================================================================
#include <qlabel.h>
#include <qlayout.h>
#include <qlistview.h>
#include <qcombobox.h>
#include <qlineedit.h>

//=============================================================================
// Application specific includes.
//=============================================================================
#include "userdata.h"
#include "textedit.h"
#include <dbconnection.h>


class CPrivate : public QWidget
{
  Q_OBJECT

public: 
  CPrivate (QWidget *parent=0, const char *name=0, CConnection *db=0,
            CUserData *current=0);
  ~CPrivate();

  void 		loadData      (QString id, bool readonly);
  void 		deleteData    (QString id);
  void 		newData       () { mNew = true; clearControls(); slotContentChanged(); }
  bool 		hasChanged    () { return mContentChanged; }
  void 		setChanged    (bool flag) { mContentChanged = flag; }
  void      setCurrent    (QString current) { mCurrent = current; }
  QString   saveChanges   ();
  void      clearControls ();

signals:
  void contentChanged  	  (bool);
  void contentLoaded   	  (bool);
  void treeDataChanged 	  ();
  void saveModifications  ();

protected:
  void    connectSlots    ();
  void    disconnectSlots ();
  void    setReadonly     (bool readonly);
  
protected slots:
  void slotContentChanged (const QString &text);
  void slotContentChanged ();
  
private:
  QGridLayout*  mGrid;

  QLineEdit*	mNickname;
  QLineEdit*	mSpouse;
  CTextEdit*	mChildren;
  CTextEdit*	mHobbies;

  CConnection*  mDB;
  CUserData*    mCurrentUser;
  QString       mCurrent;
  bool			mContentChanged;
  bool			mReadonly;
  bool			mNew;
};

#endif
