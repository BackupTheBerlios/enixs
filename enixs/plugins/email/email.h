//#############################################################################
//#  calendar.h
//#  ==========
//#
//#  Creation:       Mon Apr 22 23:35:16 CEST 2002
//#  Author:         Ralf Schneider  (ralf.schneider@b-connected.de)
//#  Copyright:      (C) 2002 by B-connected Linux Solutions GmbH
//#
//#  ------------------------------------------------------------------------
//#
//#  This file contains the definition of the class CEmail.
//#  This is the main class of the Email application of EniXs, which
//#  demonstrates howto implement your own EniXs plugins.
//#
//#  ------------------------------------------------------------------------
//#
//#  This program is free software; you can redistribute it and/or modify it
//#  under the terms of the GNU General Public License as published by the
//#  Free Software Foundation; either version 2 of the License, or (at your
//#  option) any later version.
//#
//#############################################################################

#ifndef EMAIL_H
#define EMAIL_H

//=============================================================================
// Include files for QT.
//=============================================================================
#include <qwidget.h>
#include <qvbox.h>
#include <qaction.h>
#include <qmenubar.h>
#include <qtoolbar.h>
#include <qstatusbar.h>
#include <qlistview.h>
#include <qlabel.h>

//=============================================================================
// Application specific includes.
//=============================================================================
#include "userdata.h"
#include <dbconnection.h>


#define VERSION "0.1"


class CEmail : public QVBox
{
  Q_OBJECT    

public:
  CEmail (QWidget *parent=0, const char *name=0, int wflags=0,
             CConnection *dbc=0, CUserData* current=0);

  static QString name();
  static QString group();
  static QPixmap icon();
  static QString toolTip();
  static QString whatsThis();
  static QString summary();
  static void    offeredObjects (QListViewItem* item, CConnection* db,
                                 CUserData* user);
  
protected:  
  void initActions       ();
  void initMenubar       ();
  void initToolbar       ();
  void initStatusbar     ();
  void setContentChanged (bool flag);
  bool hasContentChanged ();
  
protected slots:
  void slotEnableSaving     (bool);
  void slotEnableDeleting   (bool);
  void slotEnablePrinting   (bool);
 
private slots:
  void slotEmailNew 		();
  void slotEmailDelete	();
  void slotEmailSave		();
  void slotEmailClose	();
  void slotEmailPrint	();

  void slotEditUndo			();
  void slotEditCut			();
  void slotEditCopy			();
  void slotEditPaste		();

  void slotViewToolBar		(bool toggle);
  void slotViewStatusBar	(bool toggle);
  
  void slotHelpContents		();
  void slotHelpWhatsThis	();
  void slotHelpAbout		();
  
  void slotStatusHelpMsg	(const QString &text);
 
private:
  CConnection*    mDB;
  CUserData*      mCurrentUser;

  QMenuBar*       mMenubar;
  QPopupMenu*	  mEmailMenu;
  QPopupMenu*	  mEditMenu;
  QPopupMenu*	  mViewMenu;
  QPopupMenu*	  mHelpMenu;

  QToolBar*       mEmailToolbar;
  QStatusBar*     mStatusbar;
  
  QAction* 		  mEmailNew;
  QAction* 		  mEmailDelete;
  QAction* 		  mEmailSave;
  QAction* 		  mEmailClose;
  QAction* 		  mEmailPrint;
                  
  QAction* 		  mEditUndo;
  QAction* 		  mEditCut;
  QAction* 		  mEditCopy;
  QAction* 		  mEditPaste;
                  
  QAction* 		  mViewToolBar;
  QAction* 		  mViewStatusBar;
                  
  QAction* 		  mHelpContents;
  QAction* 		  mHelpWhatsThis;
  QAction* 		  mHelpAboutApp;

  QLabel*         mInfo;
};

#endif
