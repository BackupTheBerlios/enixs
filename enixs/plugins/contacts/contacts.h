//#############################################################################
//#  contacts.h
//#  ==========
//#
//#  Creation:       Sun Apr  7 16:25:16 CEST 2002
//#  Author:         Ralf Schneider  (ralf.schneider@b-connected.de)
//#  Copyright:      (C) 2002 by B-connected Linux Solutions GmbH
//#
//#  ------------------------------------------------------------------------
//#
//#  This file contains the definition of the class CContacts.
//#  This is the main class of the contacts application of eniXs.
//#
//#  ------------------------------------------------------------------------
//#
//#  This program is free software; you can redistribute it and/or modify it
//#  under the terms of the GNU General Public License as published by the
//#  Free Software Foundation; either version 2 of the License, or (at your
//#  option) any later version.
//#
//#############################################################################

#ifndef CONTACTS_H
#define CONTACTS_H

//=============================================================================
// Include files for QT.
//=============================================================================
#include <qlistview.h>
#include <qtabwidget.h>
#include <qvbox.h>
#include <qaction.h>
#include <qmenubar.h>
#include <qpopupmenu.h>
#include <qtoolbar.h>
#include <qstatusbar.h>
#include <qsplitter.h>
#include <qasciidict.h>
#include <qsqldatabase.h>

//=============================================================================
// Application specific includes.
//=============================================================================
#include "userdata.h"
#include "general/general.h"
#include "communication/communication.h"
#include "address/address.h"
#include "job/job.h"
#include "private/private.h"
#include "security/security.h"


#define VERSION "0.1"


class CContacts : public QVBox
{
  Q_OBJECT    

public:
  CContacts (QWidget *parent=0, const char *name=0, int wflags=0);

  static QString name();
  static QString group();
  static QPixmap icon();
  static QString toolTip();
  static QString whatsThis();
  static QString summary();
  static void    offeredObjects (QListViewItem* item);

  enum   TreeView  { Alphabetical, ByCompany, ByCategory };
      
signals:
  void selectedItem (QString, QString);
  
protected:  
  void initActions       ();
  void initMenubar       ();
  void initToolbar       ();
  void initStatusbar     ();
  void loadTree          (TreeView type);
  bool checkRights       (QString id, QString owner, bool owner_read,
                          bool friends_read, bool all_read);
  QListViewItem *resetBranch (QString branch);
  void setContentChanged (bool flag);
  bool hasContentChanged ();
  bool lockEntry         (QString id);
  void unlockEntry       ();
  
protected slots:
  void slotLoadPerson       (QListViewItem *item);
  void slotEnableSaving     (bool);
  void slotEnableDeleting   (bool);
  void slotEnablePrinting   (bool);
 
private slots:
  void slotContactNew 		();
  void slotContactDelete	();
  void slotContactSave		();
  void slotContactClose		();
  void slotContactPrint		();

  void slotEditUndo			();
  void slotEditCut			();
  void slotEditCopy			();
  void slotEditPaste		();

  void slotViewAlphabetical	();
  void slotViewCompany     	();
  void slotViewCategory    	();
  void slotViewToolBar		(bool toggle);
  void slotViewStatusBar	(bool toggle);
  
  void slotHelpContents		();
  void slotHelpWhatsThis	();
  void slotHelpAbout		();
  
  void slotStatusHelpMsg	(const QString &text);
 
private:
  QSqlDatabase*   mDB;
  CUserData*      mCurrentUser;

  QMenuBar*       mMenubar;
  QPopupMenu*	  mContactMenu;
  QPopupMenu*	  mEditMenu;
  QPopupMenu*	  mViewMenu;
  QPopupMenu*	  mHelpMenu;

  QToolBar*       mContactToolbar;
  QStatusBar*     mStatusbar;
  
  QAction* 		  mContactNew;
  QAction* 		  mContactDelete;
  QAction* 		  mContactSave;
  QAction* 		  mContactClose;
  QAction* 		  mContactPrint;
                  
  QAction* 		  mEditUndo;
  QAction* 		  mEditCut;
  QAction* 		  mEditCopy;
  QAction* 		  mEditPaste;
                  
  QAction* 		  mViewAlphabetical;
  QAction* 		  mViewCompany;
  QAction* 		  mViewCategory;
  QAction* 		  mViewToolBar;
  QAction* 		  mViewStatusBar;
                  
  QAction* 		  mHelpContents;
  QAction* 		  mHelpWhatsThis;
  QAction* 		  mHelpAboutApp;

  QSplitter*      mSplitter;
  QListView*      mTree;
  QListViewItem*  mRoot;
  QListViewItem*  mSelectedItem;
  QListViewItem*  mLastSelectedItem;
  bool            mSelectedReadonly;
  TreeView        mCurrentView;
  QAsciiDict<QString> mCategories;
  
  QTabWidget*     mTab;
  CGeneral*       mGeneral;
  CCommunication* mCommunication;
  CAddress*       mAddress;
  CJob*           mJob;
  CPrivate*       mPrivate;
  CSecurity*      mSecurity;

  QPixmap*        mManIcon;
  QPixmap*        mWomanIcon;

  QString         mUnlockStatement;
};

#endif
