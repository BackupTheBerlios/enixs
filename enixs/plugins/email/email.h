//#############################################################################
//#  email.h
//#  =======
//#
//#  Creation:       Thu Jun 27 11:35:16 CEST 2002
//#  Author:         Sven Alisch  (svenali@t-online.de)
//#  Copyright:      (C) 2002 by Sven Alisch
//#
//#  ------------------------------------------------------------------------
//#
//#  This file contains the implemenatition of the class CEmail.
//#  This is the main class of the calendar application of eniXs.
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
#include <qmenubar.h>
#include <qaction.h>
#include <qpopupmenu.h>
#include <qaccel.h>
#include <qtoolbar.h>
#include <qlistview.h>
#include <qsplitter.h>
#include <qtextedit.h>
#include <qvaluelist.h>
#include <qsqldatabase.h>
#include <qvbox.h>

//=============================================================================
// Application specific includes.
//=============================================================================
#include "userdata.h"
//#include <dbconnection.h>

//=============================================================================
// Application specific includes.
//=============================================================================
#include "userdata.h"
#include "cpop.h"
#include "bitmaps/16x16/mail_get.xpm"
#include "bitmaps/16x16/folder_new.xpm"
#include "bitmaps/16x16/filenew.xpm"
#include "bitmaps/contactnew.xpm"
#include "bitmaps/contactdelete.xpm"
#include "bitmaps/contactsave.xpm"
#include "bitmaps/contactclose.xpm"
#include "bitmaps/contactprint.xpm"
#include "bitmaps/editundo.xpm"
#include "bitmaps/editcut.xpm"
#include "bitmaps/editcopy.xpm"
#include "bitmaps/editpaste.xpm"
#include "bitmaps/editfind.xpm"
#include "bitmaps/helpcontents.xpm"
#include "bitmaps/helpcontext.xpm"
//#include <dbconnection.h>

class QLabel;


class CEmail : public QWidget
{
  Q_OBJECT    
//  Q_ENUMS(Mode)
//  Q_PROPERTY(Mode mode READ mode WRITE setMode)
//  Q_PROPERTY(QString fileName READ fileName WRITE setFileName)

public:
  CEmail (QWidget *parent=0, const char *name=0, int wflags=0);
  
  static QString name();
  static QString group();
  static QPixmap icon();
  static QString toolTip();
  static QString whatsThis();
  static QString summary();
  
 protected:
  void initMenubar();
  void initAction();
  void initToolbar();
  
public slots:
  void downloadEmails();
  void updateAllViews();
  
private slots:

private:
  QSqlDatabase*   mDB;
  CUserData*      mCurrentUser;
  bool updateTree();  
  CPOP*         myPOPConnection;
 
  QLabel*       mLabel;
  
  QMenuBar*     mMenubar;
  QToolBar*     mEmailToolbar;

  QPopupMenu*   mEmailMenu;
  QPopupMenu*   mEditMenu;
  QPopupMenu*   mEmailDirectory;
  QPopupMenu*   mMessages;

  QAction*      mEmailNew;
  QAction*      mSaveUnder;
  QAction*      mPrintMail;
  QAction*      mDownloadEmail;
  QAction*      mDownloadEmailin;
  QAction*      mUnSendMessages;
  QAction*      mClose;

  QAction*      mEditCut;
  QAction*      mEditCopy;
  QAction*      mEditUndo;
  QAction*      mEditPaste;
  QAction*      mEditSearchin;
  QAction*      mEditSearchall;

  QAction*      mDirectoryNew;
  QAction*      mProperty;
  QAction*      mCompress;
  QAction*      mMakeEmpty;
  QAction*      mDeleteFolder;
  QAction*      mHTMLView;
  QAction*      mGroupMail;

  QAction*      mNewMessages;

  QSplitter*    mVertSplitter;
  QSplitter*    mHoriSplitter;
  QListView*    mEmailTree;
  QListView*    mEmails;
  QTextEdit*    mEmailText;
  
};

#endif
