//#############################################################################
//#  email.cpp
//#  ============
//#
//#  Creation:       Sun Apr  7 16:25:16 CEST 2002
//#  Author:         Ralf Schneider  (ralf.schneider@b-connected.de)
//#  Copyright:      (C) 2002 by B-connected Linux Solutions GmbH
//#
//#  ------------------------------------------------------------------------
//#
//#  This file contains the implemenatition of the class CEmail.
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

//=============================================================================
// Include files for QT.
//=============================================================================
#include <qaccel.h>
#include <qwhatsthis.h>
#include <qmessagebox.h>

//=============================================================================
// Application specific includes.
//=============================================================================
#include "email.h"

//=============================================================================
// Bitmaps.
//=============================================================================
#include "bitmaps/emailnew.xpm"
#include "bitmaps/emaildelete.xpm"
#include "bitmaps/emailsave.xpm"
#include "bitmaps/emailclose.xpm"
#include "bitmaps/emailprint.xpm"

#include "bitmaps/editundo.xpm"
#include "bitmaps/editcut.xpm"
#include "bitmaps/editcopy.xpm"
#include "bitmaps/editpaste.xpm"
#include "bitmaps/editfind.xpm"

#include "bitmaps/helpcontents.xpm"
#include "bitmaps/helpcontext.xpm"

#include "bitmaps/email.xpm"


//=============================================================================
// Constructor of the Email application class.
//=============================================================================
CEmail::CEmail (QWidget *parent, const char *name, int wflags, 
                      CConnection *db, CUserData *current)
    : QVBox (parent, name)
{
  setGeometry (0, 0, 400, 300);

  //----------------------------------------------------------------------------
  //  Store the database connection.
  //----------------------------------------------------------------------------
  mDB          = db;
  mCurrentUser = current;
  
  //---------------------------------------------------------------------------
  // Create the menubar and the toolbar.
  //---------------------------------------------------------------------------
  initActions();
  initMenubar();
  initToolbar();
  
  //---------------------------------------------------------------------------
  // Create the label.
  //---------------------------------------------------------------------------
  mInfo = new QLabel ("This is the Email plugin"
                      "\nthat demonstrates howto implement an EniXs plugin",
                      this, "info");
  
  initStatusbar();
}

//=============================================================================
// Initialize the menu actions.
//=============================================================================
void CEmail::initActions()
{
  QPixmap deleteIcon, saveIcon, newIcon, printIcon, closeIcon;
  QPixmap undoIcon, cutIcon, copyIcon, pasteIcon, findIcon;
  QPixmap contentsIcon, contextIcon, aboutIcon;

  //---------------------------------------------------------------------------
  // Create the icons for the actions.
  //---------------------------------------------------------------------------
  newIcon  	   = QPixmap (emailnew);
  deleteIcon   = QPixmap (emaildelete);
  saveIcon 	   = QPixmap (emailsave);
  closeIcon    = QPixmap (emailclose);
  printIcon    = QPixmap (emailprint);

  undoIcon     = QPixmap (editundo);
  cutIcon 	   = QPixmap (editcut);
  copyIcon 	   = QPixmap (editcopy);
  pasteIcon    = QPixmap (editpaste);
  findIcon 	   = QPixmap (editfind);

  contentsIcon = QPixmap (helpcontents);
  contextIcon  = QPixmap (helpcontext);
  aboutIcon    = QPixmap (email_icon);

  //---------------------------------------------------------------------------
  // Create the actions for the Email menu.
  //---------------------------------------------------------------------------
  mEmailNew = new QAction (tr("New Email"), newIcon, tr("&New"), 
                             QAccel::stringToKey (tr("Ctrl+N")), this);
  mEmailNew->setStatusTip (tr("Creates a new email"));
  mEmailNew->setWhatsThis (tr("New Email\n\nCreates a new email"));
  connect (mEmailNew, SIGNAL(activated()), this, SLOT(slotEmailNew()));

  mEmailDelete = new QAction (tr("Delete Email"), deleteIcon, tr("&Delete"), 
                                QAccel::stringToKey (tr("Ctrl+D")), this);
  mEmailDelete->setStatusTip (tr("Deletes the current email"));
  mEmailDelete->setWhatsThis (tr("Delete Email.\n\nDeletes the current email"));
  mEmailDelete->setEnabled   (false);
  connect (mEmailDelete, SIGNAL(activated()), this, SLOT(slotEmailDelete()));

  mEmailSave = new QAction (tr("Save Email"), saveIcon, tr("&Save"), 
                              QAccel::stringToKey (tr("Ctrl+S")), this);
  mEmailSave->setStatusTip (tr("Saves the current email"));
  mEmailSave->setWhatsThis (tr("Save Email.\n\nSaves the current email"));
  mEmailSave->setEnabled   (false);
  connect (mEmailSave, SIGNAL(activated()), this, SLOT(slotEmailSave()));

  mEmailPrint = new QAction (tr("Print Email"), printIcon, tr("&Print"), 
                               QAccel::stringToKey (tr("Ctrl+P")), this);
  mEmailPrint->setStatusTip (tr("Prints out the current email"));
  mEmailPrint->setWhatsThis (tr("Print Email\n\nPrints out the current email"));
  mEmailPrint->setEnabled   (false);
  connect (mEmailPrint, SIGNAL(activated()), this, SLOT(slotEmailPrint()));

  mEmailClose = new QAction (tr("Close Email"), closeIcon, tr("&Close"), 
                               QAccel::stringToKey (tr("Ctrl+W")), this);
  mEmailClose->setStatusTip (tr("Closes the email application"));
  mEmailClose->setWhatsThis (tr("Close Email\n\nCloses the email application"));
  connect (mEmailClose, SIGNAL(activated()), this, SLOT(slotEmailClose()));

  //---------------------------------------------------------------------------
  // Create the actions for the Edit menu.
  //---------------------------------------------------------------------------
  mEditCut = new QAction (tr("Cut"), cutIcon, tr("Cu&t"), 
                          QAccel::stringToKey (tr("Ctrl+X")), this);
  mEditCut->setStatusTip (tr("Cuts the selected text and puts it to the clipboard"));
  mEditCut->setWhatsThis (tr("Cut\n\nCuts the selected text and puts it to the clipboard"));
  mEditCut->setEnabled   (false);
  connect (mEditCut, SIGNAL(activated()), this, SLOT(slotEditCut()));

  mEditCopy = new QAction (tr("Copy"), copyIcon, tr("&Copy"), 
                           QAccel::stringToKey (tr("Ctrl+C")), this);
  mEditCopy->setStatusTip (tr("Copies the selected text to the clipboard"));
  mEditCopy->setWhatsThis (tr("Copy\n\nCopies the selected text to the clipboard"));
  mEditCopy->setEnabled   (false);
  connect (mEditCopy, SIGNAL(activated()), this, SLOT(slotEditCopy()));

  mEditUndo = new QAction (tr("Undo"), undoIcon, tr("&Undo"), 
                           QAccel::stringToKey (tr("Ctrl+Z")), this);
  mEditUndo->setStatusTip (tr("Reverts the last editing action"));
  mEditUndo->setWhatsThis (tr("Undo\n\nReverts the last editing action"));
  mEditUndo->setEnabled   (false);
  connect (mEditUndo, SIGNAL(activated()), this, SLOT(slotEditUndo()));

  mEditPaste = new QAction (tr("Paste"), pasteIcon, tr("&Paste"), 
                            QAccel::stringToKey (tr("Ctrl+V")), this);
  mEditPaste->setStatusTip (tr("Pastes the clipboard contents to actual position"));
  mEditPaste->setWhatsThis (tr("Paste\n\nPastes the clipboard contents to actual position"));
  connect (mEditPaste, SIGNAL(activated()), this, SLOT(slotEditPaste()));

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
  // Create the actions for the Help menu.
  //---------------------------------------------------------------------------
  mHelpContents = new QAction (tr("Contents"), contentsIcon, tr("&Contents..."), 
                               QAccel::stringToKey (tr("F1")), this);
  mHelpContents->setStatusTip (tr("Contents of the user manual"));
  mHelpContents->setWhatsThis (tr("Contents\n\nContents of the user manual"));
  connect (mHelpContents, SIGNAL(activated()), this, SLOT(slotHelpContents()));

  mHelpWhatsThis = new QAction (tr("What's This"), contextIcon, tr("What's &This"),
                                QAccel::stringToKey (tr("Shift+F1")), this);
  mHelpWhatsThis->setStatusTip (tr("Context sensitive help"));
  mHelpWhatsThis->setWhatsThis (tr("What's This\n\nContext sensitive help"));
  connect (mHelpWhatsThis, SIGNAL(activated()), this, SLOT(slotHelpWhatsThis()));

  mHelpAboutApp = new QAction (tr("About"), aboutIcon, tr("&About..."), 0, this);
  mHelpAboutApp->setStatusTip (tr("About the application"));
  mHelpAboutApp->setWhatsThis (tr("About\n\nAbout the application"));
  connect (mHelpAboutApp, SIGNAL(activated()), this, SLOT(slotHelpAbout()));
}

//=============================================================================
// Create the menubar.
//=============================================================================
void CEmail::initMenubar()
{
  //---------------------------------------------------------------------------
  // Entries for the email menu.
  //---------------------------------------------------------------------------
  mEmailMenu = new QPopupMenu();

  mEmailNew->addTo	(mEmailMenu);
  mEmailDelete->addTo(mEmailMenu);
  mEmailSave->addTo	(mEmailMenu);
  mEmailPrint->addTo	(mEmailMenu);
  mEmailMenu->insertSeparator();
  mEmailClose->addTo	(mEmailMenu);

  //---------------------------------------------------------------------------
  // Entries for the Edit menu.
  //---------------------------------------------------------------------------
  mEditMenu = new QPopupMenu();

  mEditUndo->addTo	(mEditMenu);
  mEditMenu->insertSeparator();
  mEditCut->addTo	(mEditMenu);
  mEditCopy->addTo	(mEditMenu);
  mEditPaste->addTo	(mEditMenu);

  //---------------------------------------------------------------------------
  // Entries for the View menu.
  //---------------------------------------------------------------------------
  mViewMenu = new QPopupMenu();
  mViewMenu->setCheckable (true);

  mViewToolBar->addTo	(mViewMenu);
  mViewStatusBar->addTo	(mViewMenu);

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
  mMenubar = new QMenuBar (this);
  
  mMenubar->insertItem (tr("&File"), mEmailMenu);
  mMenubar->insertItem (tr("&Edit"), mEditMenu);
  mMenubar->insertItem (tr("&View"), mViewMenu);
  mMenubar->insertItem (tr("&Help"), mHelpMenu);
}

//=============================================================================
// Create the toolbar.
//=============================================================================
void CEmail::initToolbar()
{
  mEmailToolbar = new QToolBar ("Email Operations", 0, this);
  mEmailToolbar->setFixedSize  (250, 25);
  
  mEmailNew->addTo	(mEmailToolbar);
  mEmailDelete->addTo(mEmailToolbar);
  mEmailSave->addTo	(mEmailToolbar);
  mEmailPrint->addTo	(mEmailToolbar);
  mEmailToolbar->addSeparator();
  mEditUndo->addTo	    (mEmailToolbar);
  mEditCut->addTo	    (mEmailToolbar);
  mEditCopy->addTo	    (mEmailToolbar);
  mEditPaste->addTo	    (mEmailToolbar);
  mEmailToolbar->addSeparator();
  QWhatsThis::whatsThisButton (mEmailToolbar);
}

//=============================================================================
// Initialize the statusbar.
//=============================================================================
void CEmail::initStatusbar()
{
  mStatusbar = new QStatusBar    (this, "status bar");
  mStatusbar->setFixedHeight     (20);
  mStatusbar->message            (tr("Ready"));
}

//=============================================================================
// Check if the content of one of the tab pages has changed.
//=============================================================================
bool CEmail::hasContentChanged ()
{
  // Add your own checks here.
  return false;
}

//=============================================================================
// Set the Changed flag of all pages.
//=============================================================================
void CEmail::setContentChanged (bool flag)
{
  // Add your own code here.
}

//=============================================================================
// SLOT:  Create a new email.
//=============================================================================
void CEmail::slotEmailNew()
{
  mStatusbar->message (tr("Creating new email..."));

  // Add your own code here.
  
  mStatusbar->message (tr("Ready."));
}

//=============================================================================
// SLOT:  Delete the current email.
//=============================================================================
void CEmail::slotEmailDelete()
{
  mStatusbar->message (tr("Deleting email..."));
  
  // Add your own code here.
  
  mStatusbar->message (tr("Ready."));
}

//=============================================================================
// SLOT:  Save the current email.
//=============================================================================
void CEmail::slotEmailSave()
{
  mStatusbar->message (tr("Saving email..."));
  
  // Add your own code here.
  
  mStatusbar->message (tr("Ready."));
}

//=============================================================================
// SLOT:  Close the email application.
//=============================================================================
void CEmail::slotEmailClose()
{
  mStatusbar->message (tr("Closing email..."));

  // Add your own code here.
  
  mStatusbar->message (tr("Ready."));
}

//=============================================================================
// SLOT:  Print the current email.
//=============================================================================
void CEmail::slotEmailPrint()
{
  mStatusbar->message (tr("Printing..."));
	
  // Add your own code here.

  mStatusbar->message (tr("Ready."));
}

//=============================================================================
// SLOT:  Undo the last action.
//=============================================================================
void CEmail::slotEditUndo()
{
  mStatusbar->message (tr("Reverting last action..."));
	
  // Add your own code here.

  mStatusbar->message (tr("Ready."));
}

//=============================================================================
// SLOT:  Cut the selection and insert it into the clipboard.
//=============================================================================
void CEmail::slotEditCut()
{
  mStatusbar->message (tr("Cutting selection..."));
	
  // Add your own code here.

  mStatusbar->message (tr("Ready."));
}

//=============================================================================
// SLOT:  Copy the selection into the clipboard.
//=============================================================================
void CEmail::slotEditCopy()
{
  mStatusbar->message (tr("Copying selection to clipboard..."));

  // Add your own code here.

  mStatusbar->message (tr("Ready."));
}

//=============================================================================
// SLOT:  Insert the content of the clipboard at the current position.
//=============================================================================
void CEmail::slotEditPaste()
{
  mStatusbar->message (tr("Inserting clipboard contents..."));

  // Add your own code here.

  mStatusbar->message (tr("Ready."));
}

//=============================================================================
// SLOT:  Toggle the toolbar on/off.
//=============================================================================
void CEmail::slotViewToolBar (bool toggle)
{
  mStatusbar->message (tr("Toggle toolbar..."));

  if (toggle)
    mEmailToolbar->show();
  else
    mEmailToolbar->hide();

 mStatusbar->message (tr("Ready."));
}

//=============================================================================
// SLOT:  Toggle the statusbar on/off.
//=============================================================================
void CEmail::slotViewStatusBar (bool toggle)
{
  mStatusbar->message(tr("Toggle statusbar..."));

  if (toggle)
    mStatusbar->show();
  else
    mStatusbar->hide();

  mStatusbar->message (tr("Ready."));
}

//=============================================================================
// SLOT:  Show the contents of the user manual.
//=============================================================================
void CEmail::slotHelpContents()
{
  mStatusbar->message (tr("Showing the user manual contents..."));

  // Add your own code here.

  mStatusbar->message (tr("Ready."));
}

//=============================================================================
// SLOT:  Show the What's This cursor.
//=============================================================================
void CEmail::slotHelpWhatsThis()
{
  mStatusbar->message (tr("Showing information about widgets..."));

  QWhatsThis::enterWhatsThisMode();
  
  mStatusbar->message (tr("Ready."));
}

//=============================================================================
// SLOT:  Open the about dialog of the application.
//=============================================================================
void CEmail::slotHelpAbout()
{
  QMessageBox mb (tr("About Email"),
                  tr("Email ") + VERSION + "\n\n" +
                  tr("The Email Application\n\n") +
                  tr("(c) 2002 by B-connected Linux Solutions GmbH"),
                  QMessageBox::Information,
                  QMessageBox::Ok | QMessageBox::Default,
                  QMessageBox::NoButton, QMessageBox::NoButton, this);
  mb.setIconPixmap (QPixmap (email_icon));

  mb.exec();
}

//=============================================================================
// SLOT:  Change the status message.
//=============================================================================
void CEmail::slotStatusHelpMsg (const QString &text)
{
  mStatusbar->message (text, 2000);
}

//=============================================================================
// SLOT: Enable the Save action.
//=============================================================================
void CEmail::slotEnableSaving (bool changed)
{
  mEmailSave->setEnabled (changed);
}

//=============================================================================
// SLOT: Enable the Print action.
//=============================================================================
void CEmail::slotEnablePrinting (bool changed)
{
  mEmailPrint->setEnabled (changed);
}

//=============================================================================
// SLOT: Enable the Delete action.
//=============================================================================
void CEmail::slotEnableDeleting (bool changed)
{
  mEmailDelete->setEnabled (changed);
}

//=============================================================================
// Return the name of the plugin.
//=============================================================================
QString CEmail::name()
{
  return tr("Email");
}

//=============================================================================
// Return the group of the plugin.
//=============================================================================
QString CEmail::group()
{
  return "&Examples";
}

//=============================================================================
// Return the icon of the plugin.
//=============================================================================
QPixmap CEmail::icon()
{
  return QPixmap (email_icon);
}

//=============================================================================
// Return the tooltip text of the plugin.
//=============================================================================
QString CEmail::toolTip()
{
  return tr("Email Application");
}

//=============================================================================
// Return the What's This text of the plugin.
//=============================================================================
QString CEmail::whatsThis()
{
  return tr("Email Application for demonstrating plugins");
}

//=============================================================================
// Return the summary information of the plugin.
//=============================================================================
QString CEmail::summary()
{
  return tr("Summary information");
}

//=============================================================================
// Insert the offered objects of the plugin as children into the given
// QListViewItem so that other plugins can use them for linking.
//=============================================================================
void CEmail::offeredObjects (QListViewItem *item,CConnection *db,CUserData* user)
{
}


