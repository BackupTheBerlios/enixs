//#############################################################################
//#  enixs.cpp
//#  =========
//#
//#  Creation:       Die Apr  2 16:25:16 CEST 2002
//#  Author:         Ralf Schneider  (ralf.schneider@b-connected.de)
//#  Copyright:      (C) 2002 by B-connected Linux Solutions GmbH
//#
//#  ------------------------------------------------------------------------
//#
//#  This file contains the implementation of the class CEnixsApp.
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

//=============================================================================
// Include files for QT.
//=============================================================================
#include <qvbox.h>
#include <qaccel.h>
#include <qtooltip.h>
#include <qsqldatabase.h>

//=============================================================================
// Application specific includes.
//=============================================================================
#include "enixsview.h"
#include "enixs.h" 
#include "plugin/widgetfactory.h"
#include "login/login.h"
#include "userdata.h"
#include "util.h"
#include "resource.h"

//=============================================================================
// Bitmaps for the actions.
//=============================================================================
#include "bitmaps/filenew.xpm"
#include "bitmaps/filequit.xpm"
#include "bitmaps/editfind.xpm"
#include "bitmaps/helpcontents.xpm"
#include "bitmaps/helpcontext.xpm"
#include "bitmaps/enixs32.xpm"

       QSqlDatabase* mDB;
       CUserData*    mCurrentUser;
extern QString       mLanguage;

//=============================================================================
// Constructor of the EniXs application class.
//=============================================================================
CEnixsApp::CEnixsApp()
{
  CLogin   login (this);
  QString  err;

  setCaption ("EniXs " VERSION);
  setIcon    (QPixmap (enixs32));
  QToolTip::setPalette (QPalette (QColorGroup::Base, QColor (235, 245, 40)));
	
  //---------------------------------------------------------------------------
  // Initialize the status bar.
  //---------------------------------------------------------------------------
  initStatusBar();
	
  //---------------------------------------------------------------------------
  // Create the database object.
  //---------------------------------------------------------------------------
  mDB = QSqlDatabase::addDatabase ("QSAPDB7");

  if (!mDB)
  {
    QMessageBox::critical ((QWidget*)0, tr ("Error during Startup"),
                           tr ("Cannot connect to database driver."),
                           QMessageBox::Ok, QMessageBox::NoButton);
    exit(1);
  }
  
  //---------------------------------------------------------------------------
  // Display the login dialog until the login was successful or aborted.
  //---------------------------------------------------------------------------
  while (!mDB->isOpen())
  {
    if (login.exec())
    {
      mDB->setUserName     (login.username());
      mDB->setPassword     (login.password());
      mDB->setDatabaseName (login.database());
      mDB->setHostName     (login.hostname());

      //-----------------------------------------------------------------------
      // Connect to the database with the entered data.
      //-----------------------------------------------------------------------
      if (!mDB->open())
      {
        if (mDB->lastError().driverText() == "QSAPDB7: Unable to connect")
          QMessageBox::critical	((QWidget*)0, tr ("Error during Login"),
                                 tr ("Database server is not available."),
                                 QMessageBox::Ok, QMessageBox::NoButton);
        else
        {
          QMessageBox::critical	((QWidget*)0, tr ("Error during Login"),
                                 tr ("Username or password is incorrect."),
                                 QMessageBox::Ok, QMessageBox::NoButton);
          login.setPassword ("");
        }
      }
      else
      {
        mStatusDB->setText   (mDB->databaseName());
        mStatusUser->setText (mDB->userName());
        
        if (!mDB->hostName().isEmpty())
          mStatusHost->setText (mDB->hostName());
        else
          mStatusHost->setText (tr("local"));

        login.setPassword ("");
        mDB->setPassword  ("");
      }
    }
    else
      exit(1);
  }
  
  //---------------------------------------------------------------------------
  // Start a new transaction. This implicity disables autocommit mode.
  //---------------------------------------------------------------------------
  if (!mDB->transaction())
  {
    QMessageBox::critical ((QWidget*)0, tr ("Error during Startup"),
                           tr ("Unable to disable autocommit mode."),
                           QMessageBox::Ok, QMessageBox::NoButton);
    exit(1);
  }
  
//      mDB->setIsolationLevel (UnCommitted);

  //---------------------------------------------------------------------------
  // Get the user information for the current user.
  //---------------------------------------------------------------------------
  mCurrentUser = new CUserData();

  if (!mCurrentUser->getUserData (mDB->userName()))
  {
    err = tr("User ") + mDB->userName() + 
          tr(" has not been created in the user management.");

    QMessageBox::critical (this, tr ("Error during Login"), err,
                           QMessageBox::Ok, QMessageBox::NoButton);
    exit(1);
  }

  mStatusRole->setText (mCurrentUser->role());

  //---------------------------------------------------------------------------
  // Store the current language in the database for being read by the plugins.
  //---------------------------------------------------------------------------
  setLanguage ();

  //---------------------------------------------------------------------------
  // Initialize the different parts of the application.
  //---------------------------------------------------------------------------
  initView	 	 ();
  initActions	 ();
  initMenuBar	 ();
  initToolBar	 ();
  resize		 (900, 700);

  mViewToolBar->setOn	(true);
  mViewStatusBar->setOn (true);

  connect (qApp, SIGNAL(aboutToQuit()), this, SLOT(slotFileQuit()));
}

//=============================================================================
// Destructor of the EniXs application class.
//=============================================================================
CEnixsApp::~CEnixsApp()
{
  //----------------------------------------------------------------------------
  // Disconnect from the database.
  //----------------------------------------------------------------------------
//  if (mDB->isConnected())
//    mDB->disconnect();
}

//=============================================================================
// Initialize the menu actions.
//=============================================================================
void CEnixsApp::initActions()
{
  QPixmap quitIcon, findIcon;
  QPixmap contentsIcon, contextIcon, aboutIcon;

  //---------------------------------------------------------------------------
  // Create the icons for the actions.
  //---------------------------------------------------------------------------
  quitIcon 	    = QPixmap (filequit);
  findIcon 	    = QPixmap (editfind);
  contentsIcon 	= QPixmap (helpcontents);
  contextIcon 	= QPixmap (helpcontext);
  aboutIcon   	= QPixmap (enixs32);

  //---------------------------------------------------------------------------
  // Create the actions for the File menu.
  //---------------------------------------------------------------------------
  mFileQuit = new QAction (tr("Exit"), quitIcon, tr("E&xit"), 
                           QKeySequence (tr("Ctrl+Q")), this);
  mFileQuit->setStatusTip (tr("Quits the application"));
  mFileQuit->setWhatsThis (tr("Exit\n\nQuits the application"));
  connect (mFileQuit, SIGNAL(activated()), this, SLOT(slotFileQuit()));

  //---------------------------------------------------------------------------
  // Create the actions for the Edit menu.
  //---------------------------------------------------------------------------
  mEditFind = new QAction (tr("Find"), findIcon, tr("&Find"), 
                           QAccel::stringToKey (tr("Ctrl+F")), this);
  mEditFind->setStatusTip (tr("Search the database for a specific EniXs object"));
  mEditFind->setWhatsThis (tr("Find\n\nSearch the database for a specific EniXs object"));
  connect (mEditFind, SIGNAL(activated()), this, SLOT(slotEditFind()));

  //---------------------------------------------------------------------------
  // Create the actions for the View menu.
  //---------------------------------------------------------------------------
  mViewToolBar = new QAction (tr("Toolbar"), tr("Tool&bar"), 0, this, 0, true);
  mViewToolBar->setStatusTip (tr("Enables/disables the toolbar"));
  mViewToolBar->setWhatsThis (tr("Toolbar\n\nEnables/disables the toolbar"));
  connect (mViewToolBar, SIGNAL(toggled(bool)), this, SLOT(slotViewToolBar(bool)));

  mViewStatusBar = new QAction (tr("Statusbar"), tr("&Statusbar"), 0, this, 0,true);
  mViewStatusBar->setStatusTip (tr("Enables/disables the statusbar"));
  mViewStatusBar->setWhatsThis (tr("Statusbar\n\nEnables/disables the statusbar"));
  connect (mViewStatusBar,SIGNAL(toggled(bool)),this,SLOT(slotViewStatusBar(bool)));

  //---------------------------------------------------------------------------
  // Create the actions for the Window menu.
  //---------------------------------------------------------------------------
  mWindowCascade = new QAction (tr("Cascade"), tr("&Cascade"), 0, this);
  mWindowCascade->setStatusTip (tr("Cascades all windows"));
  mWindowCascade->setWhatsThis (tr("Cascade\n\nCascades all windows"));
  connect (mWindowCascade, SIGNAL(activated()), mWorkspace, SLOT(cascade()));

  mWindowTile = new QAction (tr("Tile"), tr("&Tile"), 0, this);
  mWindowTile->setStatusTip (tr("Tiles all windows"));
  mWindowTile->setWhatsThis (tr("Tile\n\nTiles all windows"));
  connect (mWindowTile, SIGNAL(activated()), mWorkspace, SLOT(tile()));

  mWindowAction = new QActionGroup (this, 0, false);
  mWindowAction->insert (mWindowCascade);
  mWindowAction->insert (mWindowTile);

  //---------------------------------------------------------------------------
  // Create the actions for the Help menu.
  //---------------------------------------------------------------------------
  mHelpContents = new QAction (tr("Contents"), contentsIcon, tr("&Contents..."), 
                               QAccel::stringToKey (tr("Ctrl+F1")), this);
  mHelpContents->setStatusTip (tr("Contents of the user manual"));
  mHelpContents->setWhatsThis (tr("Contents\n\nContents of the user manual"));
  connect (mHelpContents, SIGNAL(activated()), this, SLOT(slotHelpContents()));

  mHelpWhatsThis = new QAction (tr("What's This"), contextIcon, tr("What's &This"),
                                QAccel::stringToKey (tr("Shift+F1")), this);
  mHelpWhatsThis->setStatusTip (tr("Context sensitive help"));
  mHelpWhatsThis->setWhatsThis (tr("What's This\n\nContext sensitive help"));
  connect (mHelpWhatsThis, SIGNAL(activated()), this, SLOT(whatsThis()));

  mHelpAboutApp = new QAction (tr("About"), aboutIcon, tr("&About..."), 0, this);
  mHelpAboutApp->setStatusTip (tr("About the application"));
  mHelpAboutApp->setWhatsThis (tr("About\n\nAbout the application"));
  connect (mHelpAboutApp, SIGNAL(activated()), this, SLOT(slotHelpAbout()));
}

//=============================================================================
// Initialize the menubar.
//=============================================================================
void CEnixsApp::initMenuBar()
{
  //---------------------------------------------------------------------------
  // Entries for the File menu.
  //---------------------------------------------------------------------------
  mFileMenu = new QPopupMenu();
  mFileQuit->addTo (mFileMenu);

  //---------------------------------------------------------------------------
  // Entries for the Edit menu.
  //---------------------------------------------------------------------------
  mEditMenu = new QPopupMenu();
  mEditFind->addTo	(mEditMenu);

  //---------------------------------------------------------------------------
  // Entries for the View menu.
  //---------------------------------------------------------------------------
  mViewMenu = new QPopupMenu();
  mViewMenu->setCheckable (true);

  mViewToolBar->addTo	(mViewMenu);
  mViewStatusBar->addTo	(mViewMenu);

  //---------------------------------------------------------------------------
  // Entries for the Window menu.
  //---------------------------------------------------------------------------
  mWindowMenu = new QPopupMenu (this);
  mWindowMenu->setCheckable    (true);

  connect (mWindowMenu, SIGNAL(aboutToShow()), this, SLOT(windowMenuAboutToShow()));

  //---------------------------------------------------------------------------
  // Entries for the Help menu.
  //---------------------------------------------------------------------------
  mHelpMenu=new QPopupMenu();
  mHelpContents->addTo	(mHelpMenu);
  mHelpWhatsThis->addTo	(mHelpMenu);
  mHelpMenu->insertSeparator();
  mHelpAboutApp->addTo	(mHelpMenu);

  //---------------------------------------------------------------------------
  // Insert the menus into the menubar.
  //---------------------------------------------------------------------------
  menuBar()->insertItem (tr("&File"),   mFileMenu);
  menuBar()->insertItem (tr("&Edit"),   mEditMenu);
  menuBar()->insertItem (tr("&View"),   mViewMenu);
  menuBar()->insertItem (tr("&Window"), mWindowMenu);
  menuBar()->insertItem (tr("&Help"),   mHelpMenu);
}

//=============================================================================
// Initialize the toolbar.
//=============================================================================
void CEnixsApp::initToolBar()
{
  mFileToolbar = new QToolBar (this, "file operations");
  mEditFind->addTo		      (mFileToolbar);
  mFileToolbar->addSeparator  ();
  QWhatsThis::whatsThisButton (mFileToolbar);
}

//=============================================================================
// Initialize the statusbar.
//=============================================================================
void CEnixsApp::initStatusBar()
{
  mStatusDB   = new QLabel (statusBar(), "statusbar db");
  mStatusHost = new QLabel (statusBar(), "statusbar host");
  mStatusUser = new QLabel (statusBar(), "statusbar user");
  mStatusRole = new QLabel (statusBar(), "statusbar role");

  mStatusDB->setFixedWidth   (80);
  mStatusHost->setFixedWidth (80);
  mStatusUser->setFixedWidth (80);
  mStatusRole->setFixedWidth (80);

  mStatusDB->setAlignment   (AlignCenter);
  mStatusHost->setAlignment (AlignCenter);
  mStatusUser->setAlignment (AlignCenter);
  mStatusRole->setAlignment (AlignCenter);

  QToolTip::add (mStatusDB,   tr ("Database Instance"));
  QToolTip::add (mStatusHost, tr ("Database Server"));
  QToolTip::add (mStatusUser, tr ("Username"));
  QToolTip::add (mStatusRole, tr ("Role"));

  statusBar()->addWidget (mStatusDB,   1, TRUE);
  statusBar()->addWidget (mStatusHost, 1, TRUE);
  statusBar()->addWidget (mStatusUser, 1, TRUE);
  statusBar()->addWidget (mStatusRole, 1, TRUE);

  statusBar()->message (tr(IDS_STATUS_DEFAULT));
}

//=============================================================================
// Initialize the main view.
//=============================================================================
void CEnixsApp::initView()
{
  QVBox* viewBack = new QVBox (this);
  viewBack->setFrameStyle (QFrame::StyledPanel | QFrame::Raised);

  mWorkspace = new QWorkspace (viewBack);
//  mWorkspace->setBackgroundColor ("#dcdcdc");
//  mWorkspace->setPaletteBackgroundPixmap();
  setCentralWidget (viewBack);

  mMainView = new CEnixsView    (mWorkspace, 0, WDestructiveClose);
  mMainView->setCaption         ("EniXs Center");
  mMainView->setIcon            (QPixmap (enixs32));
  mMainView->installEventFilter (this);

  if (mWorkspace->windowList().isEmpty()) // show the first window in maximized mode
    mMainView->showMaximized();
  else
    mMainView->show();

  connect (mMainView, SIGNAL(buttonClicked (QString)), 
           this,      SLOT(slotShowPlugin  (QString)));
}

//=============================================================================
// Exit the program.
//=============================================================================
bool CEnixsApp::queryExit()
{
  if (mDB->isOpen())
  {
    //--------------------------------------------------------------------------
    // Save the program settings.
    //--------------------------------------------------------------------------
//		saveOptions();

    //--------------------------------------------------------------------------
    // Check if there is unsaved data in mView.
    //--------------------------------------------------------------------------
//		if (mView->hasContentChanged())
//  	{
//  		int ret = QMessageBox::warning (this,
//  							tr ("Es wurden Daten geändert. Sollen sie gespeichert werden?"));
//
//  		switch (ret)
//  		{
//  			case 3:
//  					mView->slotSaveModifications();
//  					break;
//  			case 4:
//  					// Don't save modified data
//  					break;
//  			case 2:
//  					// Abort exiting
//  					return false;
//  		}
//  	}

    //--------------------------------------------------------------------------
    // Close the database connection.
    //--------------------------------------------------------------------------
    mDB->close();
    deleteTempFiles();
  }

  exit (0);

  return true;
}

//=============================================================================
// Handle the event for closing a client window.
//=============================================================================
bool CEnixsApp::eventFilter (QObject* object, QEvent* event)
{
  if ((event->type() == QEvent::Close) && ((CEnixsApp*)object != this))
  {
    QCloseEvent* e 		= (QCloseEvent*)event;
    QWidget*     window = (QWidget*)object;

    mWindows.remove (window->name());
    
    e->accept();
  }

  return QWidget::eventFilter (object, event);    // standard event processing
}

//=============================================================================
// SLOT:  Exit the program.
//=============================================================================
void CEnixsApp::slotFileQuit()
{
  statusBar()->message (tr("Exiting application..."));

  queryExit();

  statusBar()->message (tr("Ready."));
}

//=============================================================================
// SLOT:  Search the database for a specific EniXs object.
//=============================================================================
void CEnixsApp::slotEditFind()
{
  statusBar()->message (tr("Searching the database..."));
	
  statusBar()->message (tr("Ready."));
}

//=============================================================================
// SLOT:  Toggle the toolbar on/off.
//=============================================================================
void CEnixsApp::slotViewToolBar (bool toggle)
{
  statusBar()->message (tr("Toggle toolbar..."));

  if (toggle)
    mFileToolbar->show();
  else
    mFileToolbar->hide();

 statusBar()->message (tr("Ready."));
}

//=============================================================================
// SLOT:  Toggle the statusbar on/off.
//=============================================================================
void CEnixsApp::slotViewStatusBar (bool toggle)
{
  statusBar()->message(tr("Toggle statusbar..."));

  if (toggle)
    statusBar()->show();
  else
    statusBar()->hide();

  statusBar()->message (tr("Ready."));
}

//=============================================================================
// SLOT:  Show the contents of the user manual.
//=============================================================================
void CEnixsApp::slotHelpContents()
{
  statusBar()->message (tr("Showing the user manual contents..."));

  statusBar()->message (tr("Ready."));
}

//=============================================================================
// SLOT:  Open the about dialog of the application.
//=============================================================================
void CEnixsApp::slotHelpAbout()
{
  QMessageBox mb (tr("About EniXs"),
           tr("EniXs " VERSION "\n\n") +
           tr("The Enterprise Information eXchange System\n\n") +
           tr("EniXs is a platform for integrating enterprise applications \n") +
           tr("into a uniform system environment.\n\n") +
           tr("(c) 2002 by B-connected Linux Solutions GmbH"),
           QMessageBox::Information,
           QMessageBox::Ok | QMessageBox::Default,
           QMessageBox::NoButton, QMessageBox::NoButton, this);
  mb.setIconPixmap (QPixmap (enixs32));

  mb.exec();
}

//=============================================================================
// SLOT:  Change the status message.
//=============================================================================
void CEnixsApp::slotStatusHelpMsg (const QString &text)
{
  statusBar()->message (text, 2000);
}

//=============================================================================
// Build the Window menu when the user clicks the menu.
//=============================================================================
void CEnixsApp::windowMenuAboutToShow()
{
  int		i, id;

  mWindowMenu->clear();	
  mWindowCascade->addTo		(mWindowMenu);
  mWindowTile->addTo		(mWindowMenu);
  
  if (mWorkspace->windowList().isEmpty())
    mWindowAction->setEnabled(false);
  else
    mWindowAction->setEnabled(true);

  mWindowMenu->insertSeparator();

  QWidgetList windows = mWorkspace->windowList();
  for (i = 0; i < int(windows.count()); i++)
  {
    id = mWindowMenu->insertItem (QString("&%1 ").arg(i+1)+windows.at(i)->caption(),
                                  this, SLOT(windowMenuActivated(int)));
    mWindowMenu->setItemParameter(id, i);
    mWindowMenu->setItemChecked  (id, mWorkspace->activeWindow() == windows.at(i));
  }
}

//=============================================================================
// Select an open window and set the focus to it.
//=============================================================================
void CEnixsApp::windowMenuActivated (int id)
{
  QWidget* w = mWorkspace->windowList().at (id);

  if (w)
    w->setFocus();
}

//=============================================================================
// SLOT:  Show the given plugin.
//=============================================================================
void CEnixsApp::slotShowPlugin (QString name)
{
  CPluginDef*       pd;
  CWidgetFactory*   wf;
  QWidget*          window;

  //---------------------------------------------------------------------------
  // Check if the plugin is already loaded.
  //---------------------------------------------------------------------------
  if (mWindows.find (name))
    return;

  //---------------------------------------------------------------------------
  // Load the shared library of the plugin.
  //---------------------------------------------------------------------------
  pd = mMainView->pluginDef (name);
  wf = new CWidgetFactory   (pd->lib());

  //---------------------------------------------------------------------------
  // Create the window of the plugin.
  //---------------------------------------------------------------------------
  window = wf->create        (mWorkspace, name, WDestructiveClose);
  window->setCaption         (name);
  window->setIcon            (pd->icon());
  window->installEventFilter (this);
  mWindows.insert            (name, window);

  window->move (50, 0);
  window->show();

  connect (window, SIGNAL(selectedItem   (QString, QString)),
           this,   SLOT  (slotSetCaption (QString, QString)));
}

//=============================================================================
// SLOT:  Set the caption of the contacts window.
//=============================================================================
void CEnixsApp::slotSetCaption (QString name, QString title)
{
  QWidget*  window = mWindows.find (name);

  if (title == "")
    window->setCaption (name);
  else
    window->setCaption (name + "  -  " + title);
}

//=============================================================================
// Store the language setting in the database.
//=============================================================================
void CEnixsApp::setLanguage ()
{
  QSqlQuery query ("INSERT INTO enixs_user_config (user_id,description,conf_value) "
                   "VALUES (" + mCurrentUser->id() + ", 'language', '" + 
                   mLanguage + "') UPDATE DUPLICATES");
  
  if (!query.isActive())
  {
    SHOW_DB_ERROR(tr ("Error during writing of data"), query.lastQuery());
    return;
  }

  mDB->commit();
}

