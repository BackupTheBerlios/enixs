//#############################################################################
//#  template.cpp
//#  ============
//#
//#  Creation:       Sun Apr  7 16:25:16 CEST 2002
//#  Author:         Ralf Schneider  (ralf.schneider@b-connected.de)
//#  Copyright:      (C) 2002 by B-connected Linux Solutions GmbH
//#
//#  ------------------------------------------------------------------------
//#
//#  This file contains the implemenatition of the class CTemplate.
//#  This is the main class of the Template application of EniXs, which
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
#include "template.h"

//=============================================================================
// Bitmaps.
//=============================================================================
#include "bitmaps/templatenew.xpm"
#include "bitmaps/templatedelete.xpm"
#include "bitmaps/templatesave.xpm"
#include "bitmaps/templateclose.xpm"
#include "bitmaps/templateprint.xpm"

#include "bitmaps/editundo.xpm"
#include "bitmaps/editcut.xpm"
#include "bitmaps/editcopy.xpm"
#include "bitmaps/editpaste.xpm"
#include "bitmaps/editfind.xpm"

#include "bitmaps/helpcontents.xpm"
#include "bitmaps/helpcontext.xpm"

#include "bitmaps/template.xpm"


//=============================================================================
// Constructor of the <Template> application class.
//=============================================================================
CTemplate::CTemplate (QWidget *parent, const char *name, int wflags, 
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
  mInfo = new QLabel ("This is the <Template> plugin"
                      "\nthat demonstrates howto implement an EniXs plugin",
                      this, "info");
  
  initStatusbar();
}

//=============================================================================
// Initialize the menu actions.
//=============================================================================
void CTemplate::initActions()
{
  QPixmap deleteIcon, saveIcon, newIcon, printIcon, closeIcon;
  QPixmap undoIcon, cutIcon, copyIcon, pasteIcon, findIcon;
  QPixmap contentsIcon, contextIcon, aboutIcon;

  //---------------------------------------------------------------------------
  // Create the icons for the actions.
  //---------------------------------------------------------------------------
  newIcon  	   = QPixmap (templatenew);
  deleteIcon   = QPixmap (templatedelete);
  saveIcon 	   = QPixmap (templatesave);
  closeIcon    = QPixmap (templateclose);
  printIcon    = QPixmap (templateprint);

  undoIcon     = QPixmap (editundo);
  cutIcon 	   = QPixmap (editcut);
  copyIcon 	   = QPixmap (editcopy);
  pasteIcon    = QPixmap (editpaste);
  findIcon 	   = QPixmap (editfind);

  contentsIcon = QPixmap (helpcontents);
  contextIcon  = QPixmap (helpcontext);
  aboutIcon    = QPixmap (template_icon);

  //---------------------------------------------------------------------------
  // Create the actions for the <Template> menu.
  //---------------------------------------------------------------------------
  mTemplateNew = new QAction (tr("New <Template>"), newIcon, tr("&New"), 
                             QAccel::stringToKey (tr("Ctrl+N")), this);
  mTemplateNew->setStatusTip (tr("Creates a new <template>"));
  mTemplateNew->setWhatsThis (tr("New <Template>\n\nCreates a new <template>"));
  connect (mTemplateNew, SIGNAL(activated()), this, SLOT(slotTemplateNew()));

  mTemplateDelete = new QAction (tr("Delete <Template>"), deleteIcon, tr("&Delete"), 
                                QAccel::stringToKey (tr("Ctrl+D")), this);
  mTemplateDelete->setStatusTip (tr("Deletes the current <template>"));
  mTemplateDelete->setWhatsThis (tr("Delete <Template>.\n\nDeletes the current <template>"));
  mTemplateDelete->setEnabled   (false);
  connect (mTemplateDelete, SIGNAL(activated()), this, SLOT(slotTemplateDelete()));

  mTemplateSave = new QAction (tr("Save <Template>"), saveIcon, tr("&Save"), 
                              QAccel::stringToKey (tr("Ctrl+S")), this);
  mTemplateSave->setStatusTip (tr("Saves the current <template>"));
  mTemplateSave->setWhatsThis (tr("Save <Template>.\n\nSaves the current <template>"));
  mTemplateSave->setEnabled   (false);
  connect (mTemplateSave, SIGNAL(activated()), this, SLOT(slotTemplateSave()));

  mTemplatePrint = new QAction (tr("Print <Template>"), printIcon, tr("&Print"), 
                               QAccel::stringToKey (tr("Ctrl+P")), this);
  mTemplatePrint->setStatusTip (tr("Prints out the current <template>"));
  mTemplatePrint->setWhatsThis (tr("Print <Template>\n\nPrints out the current <template>"));
  mTemplatePrint->setEnabled   (false);
  connect (mTemplatePrint, SIGNAL(activated()), this, SLOT(slotTemplatePrint()));

  mTemplateClose = new QAction (tr("Close <Template>"), closeIcon, tr("&Close"), 
                               QAccel::stringToKey (tr("Ctrl+W")), this);
  mTemplateClose->setStatusTip (tr("Closes the <template> application"));
  mTemplateClose->setWhatsThis (tr("Close <Template>\n\nCloses the <template> application"));
  connect (mTemplateClose, SIGNAL(activated()), this, SLOT(slotTemplateClose()));

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
void CTemplate::initMenubar()
{
  //---------------------------------------------------------------------------
  // Entries for the <template> menu.
  //---------------------------------------------------------------------------
  mTemplateMenu = new QPopupMenu();

  mTemplateNew->addTo	(mTemplateMenu);
  mTemplateDelete->addTo(mTemplateMenu);
  mTemplateSave->addTo	(mTemplateMenu);
  mTemplatePrint->addTo	(mTemplateMenu);
  mTemplateMenu->insertSeparator();
  mTemplateClose->addTo	(mTemplateMenu);

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
  
  mMenubar->insertItem (tr("&File"), mTemplateMenu);
  mMenubar->insertItem (tr("&Edit"), mEditMenu);
  mMenubar->insertItem (tr("&View"), mViewMenu);
  mMenubar->insertItem (tr("&Help"), mHelpMenu);
}

//=============================================================================
// Create the toolbar.
//=============================================================================
void CTemplate::initToolbar()
{
  mTemplateToolbar = new QToolBar ("<Template> Operations", 0, this);
  mTemplateToolbar->setFixedSize  (250, 25);
  
  mTemplateNew->addTo	(mTemplateToolbar);
  mTemplateDelete->addTo(mTemplateToolbar);
  mTemplateSave->addTo	(mTemplateToolbar);
  mTemplatePrint->addTo	(mTemplateToolbar);
  mTemplateToolbar->addSeparator();
  mEditUndo->addTo	    (mTemplateToolbar);
  mEditCut->addTo	    (mTemplateToolbar);
  mEditCopy->addTo	    (mTemplateToolbar);
  mEditPaste->addTo	    (mTemplateToolbar);
  mTemplateToolbar->addSeparator();
  QWhatsThis::whatsThisButton (mTemplateToolbar);
}

//=============================================================================
// Initialize the statusbar.
//=============================================================================
void CTemplate::initStatusbar()
{
  mStatusbar = new QStatusBar    (this, "status bar");
  mStatusbar->setFixedHeight     (20);
  mStatusbar->message            (tr("Ready"));
}

//=============================================================================
// Check if the content of one of the tab pages has changed.
//=============================================================================
bool CTemplate::hasContentChanged ()
{
  // Add your own checks here.
  return false;
}

//=============================================================================
// Set the Changed flag of all pages.
//=============================================================================
void CTemplate::setContentChanged (bool flag)
{
  // Add your own code here.
}

//=============================================================================
// SLOT:  Create a new <template>.
//=============================================================================
void CTemplate::slotTemplateNew()
{
  mStatusbar->message (tr("Creating new <template>..."));

  // Add your own code here.
  
  mStatusbar->message (tr("Ready."));
}

//=============================================================================
// SLOT:  Delete the current <template>.
//=============================================================================
void CTemplate::slotTemplateDelete()
{
  mStatusbar->message (tr("Deleting <template>..."));
  
  // Add your own code here.
  
  mStatusbar->message (tr("Ready."));
}

//=============================================================================
// SLOT:  Save the current <template>.
//=============================================================================
void CTemplate::slotTemplateSave()
{
  mStatusbar->message (tr("Saving <template>..."));
  
  // Add your own code here.
  
  mStatusbar->message (tr("Ready."));
}

//=============================================================================
// SLOT:  Close the <template> application.
//=============================================================================
void CTemplate::slotTemplateClose()
{
  mStatusbar->message (tr("Closing <template>..."));

  // Add your own code here.
  
  mStatusbar->message (tr("Ready."));
}

//=============================================================================
// SLOT:  Print the current <template>.
//=============================================================================
void CTemplate::slotTemplatePrint()
{
  mStatusbar->message (tr("Printing..."));
	
  // Add your own code here.

  mStatusbar->message (tr("Ready."));
}

//=============================================================================
// SLOT:  Undo the last action.
//=============================================================================
void CTemplate::slotEditUndo()
{
  mStatusbar->message (tr("Reverting last action..."));
	
  // Add your own code here.

  mStatusbar->message (tr("Ready."));
}

//=============================================================================
// SLOT:  Cut the selection and insert it into the clipboard.
//=============================================================================
void CTemplate::slotEditCut()
{
  mStatusbar->message (tr("Cutting selection..."));
	
  // Add your own code here.

  mStatusbar->message (tr("Ready."));
}

//=============================================================================
// SLOT:  Copy the selection into the clipboard.
//=============================================================================
void CTemplate::slotEditCopy()
{
  mStatusbar->message (tr("Copying selection to clipboard..."));

  // Add your own code here.

  mStatusbar->message (tr("Ready."));
}

//=============================================================================
// SLOT:  Insert the content of the clipboard at the current position.
//=============================================================================
void CTemplate::slotEditPaste()
{
  mStatusbar->message (tr("Inserting clipboard contents..."));

  // Add your own code here.

  mStatusbar->message (tr("Ready."));
}

//=============================================================================
// SLOT:  Toggle the toolbar on/off.
//=============================================================================
void CTemplate::slotViewToolBar (bool toggle)
{
  mStatusbar->message (tr("Toggle toolbar..."));

  if (toggle)
    mTemplateToolbar->show();
  else
    mTemplateToolbar->hide();

 mStatusbar->message (tr("Ready."));
}

//=============================================================================
// SLOT:  Toggle the statusbar on/off.
//=============================================================================
void CTemplate::slotViewStatusBar (bool toggle)
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
void CTemplate::slotHelpContents()
{
  mStatusbar->message (tr("Showing the user manual contents..."));

  // Add your own code here.

  mStatusbar->message (tr("Ready."));
}

//=============================================================================
// SLOT:  Show the What's This cursor.
//=============================================================================
void CTemplate::slotHelpWhatsThis()
{
  mStatusbar->message (tr("Showing information about widgets..."));

  QWhatsThis::enterWhatsThisMode();
  
  mStatusbar->message (tr("Ready."));
}

//=============================================================================
// SLOT:  Open the about dialog of the application.
//=============================================================================
void CTemplate::slotHelpAbout()
{
  QMessageBox mb (tr("About <Template>"),
                  tr("<Template> ") + VERSION + "\n\n" +
                  tr("The <Template> Application\n\n") +
                  tr("(c) 2002 by B-connected Linux Solutions GmbH"),
                  QMessageBox::Information,
                  QMessageBox::Ok | QMessageBox::Default,
                  QMessageBox::NoButton, QMessageBox::NoButton, this);
  mb.setIconPixmap (QPixmap (template_icon));

  mb.exec();
}

//=============================================================================
// SLOT:  Change the status message.
//=============================================================================
void CTemplate::slotStatusHelpMsg (const QString &text)
{
  mStatusbar->message (text, 2000);
}

//=============================================================================
// SLOT: Enable the Save action.
//=============================================================================
void CTemplate::slotEnableSaving (bool changed)
{
  mTemplateSave->setEnabled (changed);
}

//=============================================================================
// SLOT: Enable the Print action.
//=============================================================================
void CTemplate::slotEnablePrinting (bool changed)
{
  mTemplatePrint->setEnabled (changed);
}

//=============================================================================
// SLOT: Enable the Delete action.
//=============================================================================
void CTemplate::slotEnableDeleting (bool changed)
{
  mTemplateDelete->setEnabled (changed);
}

//=============================================================================
// Return the name of the plugin.
//=============================================================================
QString CTemplate::name()
{
  return tr("<Template>");
}

//=============================================================================
// Return the group of the plugin.
//=============================================================================
QString CTemplate::group()
{
  return "&Examples";
}

//=============================================================================
// Return the icon of the plugin.
//=============================================================================
QPixmap CTemplate::icon()
{
  return QPixmap (template_icon);
}

//=============================================================================
// Return the tooltip text of the plugin.
//=============================================================================
QString CTemplate::toolTip()
{
  return tr("<Template> Application");
}

//=============================================================================
// Return the What's This text of the plugin.
//=============================================================================
QString CTemplate::whatsThis()
{
  return tr("<Template> Application for demonstrating plugins");
}

//=============================================================================
// Return the summary information of the plugin.
//=============================================================================
QString CTemplate::summary()
{
  return tr("Summary information");
}

//=============================================================================
// Insert the offered objects of the plugin as children into the given
// QListViewItem so that other plugins can use them for linking.
//=============================================================================
void CTemplate::offeredObjects (QListViewItem *item,CConnection *db,CUserData* user)
{
}


