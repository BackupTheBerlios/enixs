//#############################################################################
//#  enixs.h
//#  =======
//#
//#  Creation:       Die Apr  2 16:25:16 CEST 2002
//#  Author:         Ralf Schneider  (ralf.schneider@b-connected.de)
//#  Copyright:      (C) 2002 by B-connected Linux Solutions GmbH
//#
//#  ------------------------------------------------------------------------
//#
//#  This file contains the definition of the class CEnixsApp.
//#  This is the main application class that contains the menubar, the
//#  toolbar, the statusbar, the key bindings and the main view class
//#  CEnixsView
//#
//#  ------------------------------------------------------------------------
//#
//#  This program is free software; you can redistribute it and/or modify it
//#  under the terms of the GNU General Public License as published by the
//#  Free Software Foundation; either version 2 of the License, or (at your
//#  option) any later version.
//#
//#############################################################################

#ifndef ENIXS_H
#define ENIXS_H

//=============================================================================
// Include files for QT.
//=============================================================================
#include <qapp.h>
#include <qmainwindow.h>
#include <qaction.h>
#include <qworkspace.h>
#include <qmenubar.h>
#include <qtoolbar.h>
#include <qstatusbar.h>
#include <qwhatsthis.h>
#include <qpopupmenu.h>
#include <qaccel.h>
#include <qtoolbutton.h>
#include <qmsgbox.h>
#include <qfiledialog.h>
#include <qprinter.h>
#include <qstring.h>
#include <qpixmap.h>
#include <qpainter.h>

//=============================================================================
// Application specific includes.
//=============================================================================
#include "plugin/widgetfactory.h"

class CEnixsView;


class CEnixsApp : public QMainWindow
{
  Q_OBJECT
  
public:
  CEnixsApp();
  ~CEnixsApp();

protected:
  bool 		   queryExit	();
  virtual bool eventFilter	(QObject* object, QEvent* event);
  void         setLanguage  ();
  
private slots:
  void slotFileQuit			();
  void slotEditFind  		();
  void slotViewToolBar		(bool toggle);
  void slotViewStatusBar	(bool toggle);
  void slotHelpContents		();
  void slotHelpAbout		();
  void slotStatusHelpMsg	(const QString &text);
  void windowMenuAboutToShow();
  void windowMenuActivated	(int id);

  void slotShowPlugin       (QString name);
  void slotSetCaption       (QString name, QString title);

private:
  void initActions					();
  void initMenuBar					();
  void initToolBar					();
  void initStatusBar				();
  void initView							();

  QPopupMenu 		*mFileMenu;
  QPopupMenu 		*mEditMenu;
  QPopupMenu 		*mViewMenu;
  QPopupMenu 		*mHelpMenu;
  QPopupMenu 		*mWindowMenu;

  QToolBar 			*mFileToolbar;

  QWorkspace 		*mWorkspace;

  QAction 			*mFileQuit;
  QAction 			*mEditFind;
  QAction 			*mViewToolBar;
  QAction 			*mViewStatusBar;
  QActionGroup 	    *mWindowAction;
  QAction 			*mWindowTile;
  QAction 			*mWindowCascade;
  QAction 			*mHelpContents;
  QAction 			*mHelpWhatsThis;
  QAction 			*mHelpAboutApp;

  QLabel            *mStatusDB;
  QLabel            *mStatusHost;
  QLabel            *mStatusUser;
  QLabel            *mStatusRole;

  CEnixsView            *mMainView;
  QAsciiDict<QWidget>    mWindows;
};
#endif

