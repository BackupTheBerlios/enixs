//#############################################################################
//#  email.cpp
//#  =========
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

//=============================================================================
// Include files for QT.
//=============================================================================
#include <qlabel.h>

//=============================================================================
// Application specific includes.
//=============================================================================
#include "email.h"
#include "util.h"
#include "email.xpm"

//=============================================================================
// Constructor of the calendar application class.
//=============================================================================
CEmail::CEmail (QWidget *parent, const char *name, int wflags)
  : QWidget (parent, name, wflags)
{
  //----------------------------------------------------------------------------
  //  Store the database connection.
  //----------------------------------------------------------------------------
  mDB          = QSqlDatabase::database();
  mCurrentUser = new CUserData();
  mCurrentUser->getUserData (mDB->userName());  
  mLabel = new QLabel (tr ("Email application"), this);

  //this->resize(780,550);
  setGeometry (0, 0, 780, 550);

  initAction();
  initMenubar();
  initToolbar();

  //----------------------------------------------------------------------------
  //  Init Email ListView
  //----------------------------------------------------------------------------

  mVertSplitter = new QSplitter(QSplitter::Vertical, this, "Vertical Splitter");
  mVertSplitter->setGeometry(0,65,776,450);
  mHoriSplitter = new QSplitter(QSplitter::Horizontal, mVertSplitter, "Horizontal Splitter");
  //mHoriSplitter->setGeometry(0,65,780,580);  

  mEmailTree = new QListView(mHoriSplitter,"EMailTree",0);
  mEmailTree->addColumn("Folder",250);
  mEmailTree->setMinimumSize(250,200);

  mEmails = new QListView(mHoriSplitter,"EMailTree",0);
  mEmails->setMinimumSize(526,200);
  mEmails->addColumn("Subject",250);
  mEmails->addColumn("From",150);
  mEmails->addColumn("Date",250);
  
  mEmailText = new QTextEdit(mVertSplitter,"Email Text");
  mEmailText->setMinimumSize(776,280);
  //mVertSplitter->setOpaqueResize(true);
  //mHoriSplitter->setOpaqueResize(true);

  updateTree();
}

//=============================================================================
// Initialize the menu actions.
//=============================================================================
void CEmail::initAction() {
  QPixmap deleteIcon, saveIcon, newIcon, printIcon, closeIcon, mailgetIcon;
  QPixmap undoIcon, cutIcon, copyIcon, pasteIcon, findIcon;
  QPixmap contentsIcon, contextIcon;
  QPixmap folderNew,newMessageIcon;
  
  newIcon     = QPixmap (contactnew);
  deleteIcon  = QPixmap (contactdelete);
  saveIcon    = QPixmap (contactsave);
  closeIcon   = QPixmap (contactclose);
  printIcon   = QPixmap (contactprint);
  mailgetIcon = QPixmap (mail_getIcon);

  undoIcon    = QPixmap (editundo);
  cutIcon     = QPixmap (editcut);
  copyIcon    = QPixmap (editcopy);
  pasteIcon   = QPixmap (editpaste);
  findIcon    = QPixmap (editfind);

  contentsIcon = QPixmap (helpcontents);
  contextIcon  = QPixmap (helpcontext);

  folderNew   = QPixmap (folder_newIcon);
  newMessageIcon = QPixmap (filenew_Icon);

  //---------------------------------------------------------------------------
  // Create the actions for the Email menu.
  //---------------------------------------------------------------------------

  mEmailNew = new QAction (tr("New Email Window"), newIcon, tr("&New Window"), QAccel::stringToKey (tr("Ctrl+N")), this);
  mEmailNew->setStatusTip (tr("Creates a new email window"));
  mEmailNew->setWhatsThis (tr("New Email Window\n\nCreates a new email window"));

  mSaveUnder = new QAction (tr("Save Email"), saveIcon, tr("&Save Email"), QAccel::stringToKey (tr("Ctrl+S")), this);
  mSaveUnder->setStatusTip (tr("Save email sepraratly"));
  mSaveUnder->setWhatsThis (tr("Save Email\n\nThe user can save his email separatly in a file."));

  mPrintMail = new QAction (tr("Print Email"), printIcon, tr("&Print Email"), QAccel::stringToKey (tr("Ctrl+P")), this);
  mPrintMail->setStatusTip (tr("Print the Email"));
  mPrintMail->setWhatsThis (tr("Print Email\n\nThe user can print his email."));
  
  mDownloadEmail=new QAction(tr("Download Emails"),mailgetIcon,tr("&Download Email"),QAccel::stringToKey(tr("Ctrl+D")),this);
  connect(mDownloadEmail,SIGNAL(activated()),this,SLOT(downloadEmails()));
  mDownloadEmail->setStatusTip (tr("Download Emails"));
  mDownloadEmail->setWhatsThis (tr("Download Email\n\nThe user can download his emails."));

  mDownloadEmailin=new QAction(tr("Download Emails in"),mailgetIcon,tr("&Download Email in"),QAccel::stringToKey(tr("Ctrl+I")),this);
  mDownloadEmailin->setStatusTip (tr("Download Emails in"));
  mDownloadEmailin->setWhatsThis (tr("Download Email in\n\nThe user can download his emails from a special account."));

  mUnSendMessages = new QAction(tr("Unsend Messeages send"), tr("&Unsend Messages send"),QAccel::stringToKey(tr("Ctrl+U")),this);
  mUnSendMessages->setStatusTip (tr("Unsend Messages send"));
  mUnSendMessages->setWhatsThis (tr("Unsend Messages send\n\nThe user can send all unsent messages."));

  mClose = new QAction(tr("Exit"), closeIcon, tr("E&xit"),QAccel::stringToKey(tr("Ctrl+X")),this);
  mClose->setStatusTip (tr("Exit the Application"));
  mClose->setWhatsThis (tr("Exit\n\nExits the Application"));

  //---------------------------------------------------------------------------
  // Create the actions for the Edit menu.
  //---------------------------------------------------------------------------
  mEditCut = new QAction (tr("Cut"), cutIcon, tr("Cu&t"), QAccel::stringToKey (tr("Ctrl+X")), this);
  mEditCut->setStatusTip (tr("Cuts the selected text and puts it to the clipboard"));
  mEditCut->setWhatsThis (tr("Cut\n\nCuts the selected text and puts it to the clipboard"));
  mEditCut->setEnabled   (false);
  //connect (mEditCut, SIGNAL(activated()), this, SLOT(slotEditCut()));

  mEditCopy = new QAction (tr("Copy"), copyIcon, tr("&Copy"), QAccel::stringToKey (tr("Ctrl+C")), this);
  mEditCopy->setStatusTip (tr("Copies the selected text to the clipboard"));
  mEditCopy->setWhatsThis (tr("Copy\n\nCopies the selected text to the clipboard"));
  mEditCopy->setEnabled   (false);
  //connect (mEditCopy, SIGNAL(activated()), this, SLOT(slotEditCopy()));

  mEditUndo = new QAction (tr("Undo"), undoIcon, tr("&Undo"), QAccel::stringToKey (tr("Ctrl+Z")), this);
  mEditUndo->setStatusTip (tr("Reverts the last editing action"));
  mEditUndo->setWhatsThis (tr("Undo\n\nReverts the last editing action"));
  mEditUndo->setEnabled   (false);
  //connect (mEditUndo, SIGNAL(activated()), this, SLOT(slotEditUndo()));

  mEditPaste = new QAction (tr("Paste"), pasteIcon, tr("&Paste"), QAccel::stringToKey (tr("Ctrl+V")), this);
  mEditPaste->setStatusTip (tr("Pastes the clipboard contents to actual position"));
  mEditPaste->setWhatsThis (tr("Paste\n\nPastes the clipboard contents to actual position"));
  //connect (mEditPaste, SIGNAL(activated()), this, SLOT(slotEditPaste()));

  mEditSearchin = new QAction (tr("Search ..."), findIcon, tr("&Search ..."), QAccel::stringToKey (tr("Ctrl+E")), this);
  mEditSearchin->setStatusTip (tr("Searches text in this Email"));
  mEditSearchin->setWhatsThis (tr("Search\n\nSearches text in this Email"));
  
  mEditSearchall = new QAction (tr("Search in all Emails ..."), findIcon, tr("S&earch in all Emails ..."), QAccel::stringToKey (tr("Ctrl+A")), this);
  mEditSearchall->setStatusTip (tr("Searches in all Email"));
  mEditSearchall->setWhatsThis (tr("Search in all Emails\n\nSearches in all Emails"));

  //---------------------------------------------------------------------------
  // Create the actions for the Folder menu.
  //---------------------------------------------------------------------------
  
  mDirectoryNew = new QAction (tr("New Folder"), folderNew, tr("Ne&w folder"), QAccel::stringToKey (tr("Ctrl+F")), this);
  mDirectoryNew->setStatusTip (tr("New Folder for  Emails"));
  mDirectoryNew->setWhatsThis (tr("New Folder for  Emails\n\nCreates a new Folder for Emails"));

  mProperty = new QAction (tr("Properties"), tr("&Properties"), QAccel::stringToKey (tr("P")), this);
  mProperty->setStatusTip (tr("Properties for Folder"));
  mProperty->setWhatsThis (tr("Properties for Folder\n\nChange Properties for folder"));

  mCompress = new QAction (tr("Compress"), tr("&Compress"), QAccel::stringToKey (tr("C")), this);
  mCompress->setStatusTip (tr("Compress Folder"));
  mCompress->setWhatsThis (tr("Compress Folder\n\nCompresses big Email folders"));

  mMakeEmpty = new QAction (tr("Empty"), tr("&Empty"), QAccel::stringToKey (tr("E")), this); 
  mMakeEmpty->setStatusTip (tr("Delete Folder Content"));
  mMakeEmpty->setWhatsThis (tr("Delete Folder Contents\n\nDelete the folder content"));

  mDeleteFolder = new QAction (tr("Delete"), tr("&Delete"), QAccel::stringToKey (tr("D")), this);
  mDeleteFolder->setStatusTip (tr("Delete Folder"));
  mDeleteFolder->setWhatsThis (tr("Delete Folder\n\nDelete the whole folder"));

  mHTMLView = new QAction (tr("HTML Viewing EMails"), tr("&HTML Viewing EMails"), QAccel::stringToKey (tr("CTRL+C")), this);
  mHTMLView->setStatusTip (tr("HTML Viewing EMails"));
  mHTMLView->setWhatsThis (tr("HTML Viewing EMails\n\nViews Emails as HTML Pages"));

  mGroupMail = new QAction (tr("Show Messages in a group"), tr("&Show Messages in a group"), QAccel::stringToKey (tr("CTRL+G")), this);
  mGroupMail->setStatusTip (tr("Show Messages in a group"));
  mGroupMail->setWhatsThis (tr("Show Messages in a group\n\nShow Messages sorted in a group"));

  //---------------------------------------------------------------------------
  // Create the actions for the Messages menu.
  //---------------------------------------------------------------------------

  mNewMessages = new QAction (tr("New Message"), newMessageIcon, tr("&New Message ..."), QAccel::stringToKey (tr("CTRL+N")), this);
  
}

//=============================================================================
// Create the menubar.
//=============================================================================
void CEmail::initMenubar() {
  
  //---------------------------------------------------------------------------
  // Entries for the Email menu.
  //---------------------------------------------------------------------------
  mEmailMenu = new QPopupMenu();
  mEmailNew->addTo(mEmailMenu);
  mEmailMenu->insertSeparator();
  mSaveUnder->addTo(mEmailMenu);
  mPrintMail->addTo(mEmailMenu);
  mEmailMenu->insertSeparator();
  mDownloadEmail->addTo(mEmailMenu);
  mDownloadEmailin->addTo(mEmailMenu);
  mUnSendMessages->addTo(mEmailMenu);
  mEmailMenu->insertSeparator();
  mClose->addTo(mEmailMenu);

  //---------------------------------------------------------------------------
  // Entries for the Edit menu.
  //---------------------------------------------------------------------------
  mEditMenu = new QPopupMenu();

  mEditUndo->addTo	(mEditMenu);
  mEditMenu->insertSeparator();
  mEditCut->addTo	(mEditMenu);
  mEditCopy->addTo	(mEditMenu);
  mEditPaste->addTo	(mEditMenu);
  mEditMenu->insertSeparator();
  mEditSearchin->addTo(mEditMenu);
  mEditSearchall->addTo(mEditMenu);
  
  //---------------------------------------------------------------------------
  // Entries for the Directory menu.
  //---------------------------------------------------------------------------

  mEmailDirectory = new QPopupMenu();
  mDirectoryNew->addTo(mEmailDirectory);
  mCompress->addTo(mEmailDirectory);
  mEmailDirectory->insertSeparator();
  mMakeEmpty->addTo(mEmailDirectory);
  mDeleteFolder->addTo(mEmailDirectory);
  mEmailDirectory->insertSeparator();
  mHTMLView->addTo(mEmailDirectory);
  mGroupMail->addTo(mEmailDirectory);;
 
  //---------------------------------------------------------------------------
  // Entries for the Messages menu.
  //---------------------------------------------------------------------------

  mMessages = new QPopupMenu();
  mNewMessages->addTo(mMessages);
  

  //---------------------------------------------------------------------------
  // Insert the menus into the menubar.
  //---------------------------------------------------------------------------
  mMenubar = new QMenuBar (this);

  mMenubar->insertItem( tr("&File"), mEmailMenu );
  mMenubar->insertItem( tr("&Edit"), mEditMenu );
  mMenubar->insertItem( tr("&Directory"), mEmailDirectory);
  mMenubar->insertItem( tr("&Messages"), mMessages);
}

//=============================================================================
// Create the toolbar.
//=============================================================================
void CEmail::initToolbar() {
  mEmailToolbar = new QToolBar ("Email Operations",0,this);
  mEmailToolbar->setGeometry(0,25,200,30);
  
  mEmailNew->addTo(mEmailToolbar);
  mEmailToolbar->addSeparator();
  mSaveUnder->addTo(mEmailToolbar);
  mPrintMail->addTo(mEmailToolbar);
  mEmailToolbar->addSeparator();
  mDownloadEmail->addTo(mEmailToolbar);
  //mDownloadEmailin->addTo(mEmailToolbar);
  //mUnSendMessages->addTo(mEmailToolbar);
  mEmailToolbar->addSeparator();
  mClose->addTo(mEmailToolbar);
}

//=============================================================================
// Downloads emails
//=============================================================================
void CEmail::downloadEmails() {
  QString popacc = "pop.t-online.de";
  myPOPConnection = new CPOP(this, popacc);
  std::cout << "Jetzt müsste was kommen !!!" << endl;
  std::cout << "Die E-Mail:" << myPOPConnection->get_email() << endl;;
  mEmailText->setText(myPOPConnection->get_email());
}

//=============================================================================
// Update all Views
//=============================================================================
void CEmail::updateAllViews() {
  mEmailText->setText(myPOPConnection->get_email());
}

//=============================================================================
// Update the Tree
//=============================================================================
bool CEmail::updateTree() {
  QString sql;QStringList record;
  QSqlQuery query ("SELECT boxnames FROM mailfolder");
        
  //std::cout << sql << endl;

  if (!query.isActive())
  {
    SHOW_DB_ERROR(tr ("Error during database query"), query.lastQuery())
    return false;
  }

  while (query.next())
  {
    //std::cout << record[0] << endl;
    QListViewItem* item = new QListViewItem (mEmailTree, query.value(0).toString());
  }
  return true;
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
  return "&PIM";
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
  return tr("EMail Management");
}

//=============================================================================
// Return the What's This text of the plugin.
//=============================================================================
QString CEmail::whatsThis()
{
  return tr("Application for managing emails");
}

//=============================================================================
// Return the summary information of the plugin.
//=============================================================================
QString CEmail::summary()
{
  return tr("Summary information");
}

