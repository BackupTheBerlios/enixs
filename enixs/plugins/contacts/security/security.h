//#############################################################################
//#  security.h
//#  ==========
//#
//#  Creation:       Mon Jun 10 00:12:16 CEST 2002
//#  Author:         Ralf Schneider  (ralf.schneider@b-connected.de)
//#  Copyright:      (C) 2002 by B-connected Linux Solutions GmbH
//#
//#  ------------------------------------------------------------------------
//#
//#  This file contains the definition of the class CSecurity.
//#  This class contains all the widgets for displaying the security details
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

#ifndef SECURITY_H
#define SECURITY_H

//=============================================================================
// Include files for QT.
//=============================================================================
#include <qlabel.h>
#include <qlayout.h>
#include <qlistview.h>
#include <qcheckbox.h>
#include <qlineedit.h>
#include <qtextedit.h>
#include <qpushbutton.h>
#include <qcombobox.h>
#include <qsqldatabase.h>

//=============================================================================
// Application specific includes.
//=============================================================================
#include "userdata.h"


class CSecurity : public QWidget
{
  Q_OBJECT

public: 
  CSecurity (QWidget *parent=0, const char *name=0, QSqlDatabase *db=0,
             CUserData *current=0);
  ~CSecurity();

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
  void    loadUsers       ();
  
  QPtrList<QListViewItem> selectedFriends ();
  
protected slots:
  void slotContentChanged (const QString &text);
  void slotContentChanged ();
  void slotAddFriend      ();
  void slotRemoveFriend   ();
  void slotSetPrivate     ();
  
private:
  QGridLayout*  mGrid;

  QComboBox*    mOwner;
  QCheckBox*    mOwnerRead;
  QCheckBox*    mOwnerWrite;
  QCheckBox*    mOwnerDelete;
  QCheckBox*    mOwnerLink;
  QCheckBox*    mFriendRead;
  QCheckBox*    mFriendWrite;
  QCheckBox*    mFriendDelete;
  QCheckBox*    mFriendLink;
  QCheckBox*    mAllRead;
  QCheckBox*    mAllWrite;
  QCheckBox*    mAllDelete;
  QCheckBox*    mAllLink;
  QPushButton*  mPrivate;
  
  QListView*	mFriends;
  QPushButton*  mAdd;
  QPushButton*  mRemove;

  QPtrList<QListViewItem> 	mAddedList;
  QPtrList<QListViewItem> 	mRemovedList;
  
  QSqlDatabase* mDB;
  CUserData*    mCurrentUser;
  QString       mCurrent;
  bool			mContentChanged;
  bool			mReadonly;
  bool			mNew;
};

#endif
