//#############################################################################
//#  contacts.cpp
//#  ============
//#
//#  Creation:       Sun Apr  7 16:25:16 CEST 2002
//#  Author:         Ralf Schneider  (ralf.schneider@b-connected.de)
//#  Copyright:      (C) 2002 by B-connected Linux Solutions GmbH
//#
//#  ------------------------------------------------------------------------
//#
//#  This file contains the implemenatition of the class CContacts.
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

//=============================================================================
// Include files for QT.
//=============================================================================
#include <qheader.h>
#include <qaccel.h>
#include <qwhatsthis.h>

//=============================================================================
// Application specific includes.
//=============================================================================
#include "contacts.h"
#include "util.h"

//=============================================================================
// Bitmaps.
//=============================================================================
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

#include "bitmaps/contacts.xpm"
#include "bitmaps/group.xpm"
#include "bitmaps/man.xpm"
#include "bitmaps/woman.xpm"


//=============================================================================
// Constructor of the contacts application class.
//=============================================================================
CContacts::CContacts (QWidget *parent, const char *name, int wflags)
    : QVBox (parent, name)
{
  QValueList<int>  sizes;

  setGeometry (0, 0, 400, 300);

  //----------------------------------------------------------------------------
  //  Store the database connection.
  //----------------------------------------------------------------------------
  mDB          = QSqlDatabase::database();
  mCurrentUser = new CUserData();
  mCurrentUser->getUserData (mDB->userName());
  
  //---------------------------------------------------------------------------
  // Create the menubar and the toolbar.
  //---------------------------------------------------------------------------
  initActions();
  initMenubar();
  initToolbar();
  
  //---------------------------------------------------------------------------
  // Create the splitter.
  //---------------------------------------------------------------------------
  mSplitter = new QSplitter    (this, "splitter");
  mSplitter->setBackgroundMode (PaletteBase);
  
  initStatusbar();
  
  //---------------------------------------------------------------------------
  // Create the tree on the left side of the contacts window.
  //---------------------------------------------------------------------------
  mTree = new QListView (mSplitter, "tree view");

  mTree->addColumn ("", 1000);
  mTree->addColumn ("", 200);
  mTree->header()->close();
  mTree->setColumnWidthMode (0, QListView::Manual);
  mTree->setHScrollBarMode  (QListView::AlwaysOff);
  mTree->setBackgroundColor (QColor (255, 255, 255));
  mTree->setSorting         (-1);
  mTree->setTreeStepSize	(15);
  mTree->setMinimumWidth    (200);
  mTree->show();

  mLastSelectedItem = 0;
  mSelectedItem     = 0;
  mRoot             = 0;

  mManIcon   = new QPixmap (man_icon);
  mWomanIcon = new QPixmap (woman_icon);
  
  //---------------------------------------------------------------------------
  // Load the tree items.
  //---------------------------------------------------------------------------
  updateTree();

  //---------------------------------------------------------------------------
  // Create the tab widget on the right side of the contacts window.
  //---------------------------------------------------------------------------
  mTab = new QTabWidget (mSplitter, "tab widget");
  mTab->setMinimumWidth (550);
  
  //---------------------------------------------------------------------------
  // Create the tab with the general information of a contact.
  //---------------------------------------------------------------------------
  mGeneral = new CGeneral (mTab, "general page", mDB, mCurrentUser);
  mTab->addTab            (mGeneral, tr("&General"));

  connect (mGeneral, SIGNAL (contentChanged     (bool)),
           this,     SLOT   (slotEnableSaving   (bool)));
  connect (mGeneral, SIGNAL (contentLoaded      (bool)),
           this,     SLOT   (slotEnablePrinting (bool)));
  connect (mGeneral, SIGNAL (contentLoaded      (bool)),
           this,     SLOT   (slotEnableDeleting (bool)));
  
  //---------------------------------------------------------------------------
  // Create the tab with the communication information of a contact.
  //---------------------------------------------------------------------------
  mCommunication = new CCommunication (mTab, "communication page",mDB,mCurrentUser);
  mTab->addTab                        (mCommunication, tr("&Communication"));

  connect (mCommunication, SIGNAL (contentChanged     (bool)),
           this,           SLOT   (slotEnableSaving   (bool)));
  
  //---------------------------------------------------------------------------
  // Create the tab with the address information of a contact.
  //---------------------------------------------------------------------------
  mAddress = new CAddress (mTab, "address page", mDB, mCurrentUser);
  mTab->addTab            (mAddress, tr("&Address"));

  connect (mAddress, SIGNAL (contentChanged     (bool)),
           this,     SLOT   (slotEnableSaving   (bool)));
  
  //---------------------------------------------------------------------------
  // Create the tab with the job information of a contact.
  //---------------------------------------------------------------------------
  mJob = new CJob (mTab, "job page", mDB, mCurrentUser);
  mTab->addTab    (mJob, tr("&Job"));

  connect (mJob, SIGNAL (contentChanged     (bool)),
           this, SLOT   (slotEnableSaving   (bool)));
  
  //---------------------------------------------------------------------------
  // Create the tab with the private information of a contact.
  //---------------------------------------------------------------------------
  mPrivate = new CPrivate (mTab, "private page", mDB, mCurrentUser);
  mTab->addTab            (mPrivate, tr("&Private"));

  connect (mPrivate, SIGNAL (contentChanged     (bool)),
           this,     SLOT   (slotEnableSaving   (bool)));
  
  //---------------------------------------------------------------------------
  // Create the tab with the security information of a contact.
  //---------------------------------------------------------------------------
  mSecurity = new CSecurity (mTab, "security page", mDB, mCurrentUser);
  mTab->addTab              (mSecurity, tr("&Security"));

  connect (mSecurity, SIGNAL (contentChanged     (bool)),
           this,      SLOT   (slotEnableSaving   (bool)));

  //---------------------------------------------------------------------------
  // Set the width of the tree view.
  //---------------------------------------------------------------------------
  sizes.append      (500);
  sizes.append      (800);
  mSplitter->setSizes          (sizes);
  mSplitter->setResizeMode     (mTree, QSplitter::Stretch);
}

//=============================================================================
// Initialize the menu actions.
//=============================================================================
void CContacts::initActions()
{
  QPixmap deleteIcon, saveIcon, newIcon, printIcon, closeIcon;
  QPixmap undoIcon, cutIcon, copyIcon, pasteIcon, findIcon;
  QPixmap contentsIcon, contextIcon, aboutIcon;

  //---------------------------------------------------------------------------
  // Create the icons for the actions.
  //---------------------------------------------------------------------------
  newIcon  	   = QPixmap (contactnew);
  deleteIcon   = QPixmap (contactdelete);
  saveIcon 	   = QPixmap (contactsave);
  closeIcon    = QPixmap (contactclose);
  printIcon    = QPixmap (contactprint);

  undoIcon     = QPixmap (editundo);
  cutIcon 	   = QPixmap (editcut);
  copyIcon 	   = QPixmap (editcopy);
  pasteIcon    = QPixmap (editpaste);
  findIcon 	   = QPixmap (editfind);

  contentsIcon = QPixmap (helpcontents);
  contextIcon  = QPixmap (helpcontext);
  aboutIcon    = QPixmap (contacts_icon);

  //---------------------------------------------------------------------------
  // Create the actions for the Contact menu.
  //---------------------------------------------------------------------------
  mContactNew = new QAction (tr("New Contact"), newIcon, tr("&New"), 
                             QAccel::stringToKey (tr("Ctrl+N")), this);
  mContactNew->setStatusTip (tr("Creates a new contact"));
  mContactNew->setWhatsThis (tr("New Contact\n\nCreates a new contact"));
  connect (mContactNew, SIGNAL(activated()), this, SLOT(slotContactNew()));

  mContactDelete = new QAction (tr("Delete Contact"), deleteIcon, tr("&Delete"), 
                                QAccel::stringToKey (tr("Ctrl+D")), this);
  mContactDelete->setStatusTip (tr("Deletes the current contact"));
  mContactDelete->setWhatsThis (tr("Delete Contact.\n\nDeletes the current contact"));
  mContactDelete->setEnabled   (false);
  connect (mContactDelete, SIGNAL(activated()), this, SLOT(slotContactDelete()));

  mContactSave = new QAction (tr("Save Contact"), saveIcon, tr("&Save"), 
                              QAccel::stringToKey (tr("Ctrl+S")), this);
  mContactSave->setStatusTip (tr("Saves the current contact"));
  mContactSave->setWhatsThis (tr("Save Contact.\n\nSaves the current contact"));
  mContactSave->setEnabled   (false);
  connect (mContactSave, SIGNAL(activated()), this, SLOT(slotContactSave()));

  mContactPrint = new QAction (tr("Print Contact"), printIcon, tr("&Print"), 
                               QAccel::stringToKey (tr("Ctrl+P")), this);
  mContactPrint->setStatusTip (tr("Prints out the current contact"));
  mContactPrint->setWhatsThis (tr("Print Contact\n\nPrints out the current contact"));
  mContactPrint->setEnabled   (false);
  connect (mContactPrint, SIGNAL(activated()), this, SLOT(slotContactPrint()));

  mContactClose = new QAction (tr("Close Contacts"), closeIcon, tr("&Close"), 
                               QAccel::stringToKey (tr("Ctrl+W")), this);
  mContactClose->setStatusTip (tr("Closes the contacts application"));
  mContactClose->setWhatsThis (tr("Close Contacts\n\nCloses the contacts application"));
  connect (mContactClose, SIGNAL(activated()), this, SLOT(slotContactClose()));

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
void CContacts::initMenubar()
{
  //---------------------------------------------------------------------------
  // Entries for the Contact menu.
  //---------------------------------------------------------------------------
  mContactMenu = new QPopupMenu();

  mContactNew->addTo	(mContactMenu);
  mContactDelete->addTo	(mContactMenu);
  mContactSave->addTo	(mContactMenu);
  mContactPrint->addTo	(mContactMenu);
  mContactMenu->insertSeparator();
  mContactClose->addTo	(mContactMenu);

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
  
  mMenubar->insertItem (tr("&File"), mContactMenu);
  mMenubar->insertItem (tr("&Edit"), mEditMenu);
  mMenubar->insertItem (tr("&View"), mViewMenu);
  mMenubar->insertItem (tr("&Help"), mHelpMenu);
}

//=============================================================================
// Create the toolbar.
//=============================================================================
void CContacts::initToolbar()
{
  mContactToolbar = new QToolBar ("Contact Operations", 0, this);
  mContactToolbar->setFixedSize  (250, 25);
  
  mContactNew->addTo	(mContactToolbar);
  mContactDelete->addTo	(mContactToolbar);
  mContactSave->addTo	(mContactToolbar);
  mContactPrint->addTo	(mContactToolbar);
  mContactToolbar->addSeparator();
  mEditUndo->addTo	    (mContactToolbar);
  mEditCut->addTo	    (mContactToolbar);
  mEditCopy->addTo	    (mContactToolbar);
  mEditPaste->addTo	    (mContactToolbar);
  mContactToolbar->addSeparator();
  QWhatsThis::whatsThisButton (mContactToolbar);
}

//=============================================================================
// Initialize the statusbar.
//=============================================================================
void CContacts::initStatusbar()
{
  mStatusbar = new QStatusBar    (this, "status bar");
  mStatusbar->setFixedHeight     (20);
  mStatusbar->message            (tr("Ready"));
}

//=============================================================================
// Update the tree view.
//=============================================================================
bool CContacts::updateTree ()
{
  QString          label;
  QListViewItem*   item = 0;
  
  //----------------------------------------------------------------------------
  // Disconnect all slots during loading.
  //----------------------------------------------------------------------------
  mTree->disconnect();
  
  //----------------------------------------------------------------------------
  // Remove all items from the tree.
  //----------------------------------------------------------------------------
  mRoot = resetBranch ("root");

  //----------------------------------------------------------------------------
  // Load the items.
  //----------------------------------------------------------------------------
  QSqlQuery query ("SELECT A.name, A.first_name, A.name_suffix, A.person_id, "
                   "A.male, A.owner, B.owner_read, B.friend_read, B.all_read "
                   "FROM   contacts_persons A, contacts_rights B "
                   "WHERE  A.person_id = B.person_id "
                   "ORDER BY A.name, A.first_name");
  
  if (!query.isActive())
  {
    SHOW_DB_ERROR(tr ("Error during database query"), query.lastQuery())
    return false;
  }
  
  while (query.next())
  {
    //--------------------------------------------------------------------------
    // Check the user rights.
    //--------------------------------------------------------------------------
    if (!checkRights (query.value(3).toString(), query.value(5).toString(), 
                      query.value(6).toBool(), query.value(7).toBool(), 
                      query.value(8).toBool()))
      continue;
    
    //--------------------------------------------------------------------------
    // Insert a new item into the tree.
    //--------------------------------------------------------------------------
    if (query.value(1).toString().isEmpty())
      label = query.value(0).toString();
    else
      label = query.value(0).toString() + ", " + query.value(1).toString();
    
    if (!query.value(2).toString().isEmpty())
      label += " (" + query.value(2).toString() + ")";
    
    item = new QListViewItem (mRoot, item, label, query.value(3).toString(), 
                              "person");

    //--------------------------------------------------------------------------
    // Distinguish between male and female contacts.
    //--------------------------------------------------------------------------
    if (query.value(4).toBool())
      item->setPixmap (0, *mManIcon);
    else
      item->setPixmap (0, *mWomanIcon);
  }
  
  mTree->setOpen (mRoot, TRUE);

  connect (mTree, SIGNAL (clicked        (QListViewItem *)),
           this,  SLOT   (slotLoadPerson (QListViewItem *)));
  connect (mTree, SIGNAL (returnPressed  (QListViewItem *)),
           this,  SLOT   (slotLoadPerson (QListViewItem *)));

  return true;
}

//=============================================================================
// Check the user rights.
//=============================================================================
bool CContacts::checkRights (QString id, QString owner, bool owner_read, 
                             bool friends_read, bool all_read)
{
  //----------------------------------------------------------------------------
  // Check if all users may read the item.
  //----------------------------------------------------------------------------
  if (all_read)
    return true;
  
  //----------------------------------------------------------------------------
  // Check if I'm the owner and the owner may read the item.
  //----------------------------------------------------------------------------
  if ((owner == mCurrentUser->id()) && owner_read)
    return true;

  //----------------------------------------------------------------------------
  // Check if the friends may read the item and if I'm a friend.
  //----------------------------------------------------------------------------
  if (friends_read)
  {
    QSqlQuery q1 ("SELECT id, type FROM contacts_friends WHERE person_id = " + id);
  
    if (!q1.isActive())
    {
      SHOW_DB_ERROR(tr ("Error during database query"), q1.lastQuery());
      return false;
    }
  
    while (q1.next())
    {
      if (q1.value(1).toInt() == 0)
      {
        //----------------------------------------------------------------------
        // The friend is a single person.
        //----------------------------------------------------------------------
        if (q1.value(0).toString() == mCurrentUser->id())
          return true;
      }
      else
      {
        //----------------------------------------------------------------------
        // The friend is a group.
        //----------------------------------------------------------------------
        QSqlQuery q2 ("SELECT user_id FROM enixs_users_in_groups "
                      "WHERE  group_id = " + q1.value(0).toString());
  
        if (!q2.isActive())
        {
          SHOW_DB_ERROR(tr ("Error during database query"), q2.lastQuery());
          return false;
        }
  
        while (q2.next())
        {
          if (q2.value(0).toString() == mCurrentUser->id())
            return true;
        }
      }
    }
  }
  
  return false;
}

//=============================================================================
// Remove all items from the given branch of the tree.
//=============================================================================
QListViewItem * CContacts::resetBranch (QString branch)
{
  QListViewItem	*item;

  if (branch == "root")
  {
    FREE_PTR(mRoot);
    item = new QListViewItem (mTree, tr ("Contacts"), "", "root");
    item->setPixmap (0, QPixmap (group_icon));
    return item;
  }

  return 0;
}

//=============================================================================
// SLOT: Load the selected person.
//=============================================================================
void CContacts::slotLoadPerson (QListViewItem *item)
{
  int      ret;
  QString  id, title;
  bool     lockError = false;

  if (!item)
    return;
  
  //----------------------------------------------------------------------------
  // Unlock a previously locked contact.
  //----------------------------------------------------------------------------
  unlockEntry();

  //---------------------------------------------------------------------------
  // If the currently selected item is not a contact item, nothing has to be done.
  //---------------------------------------------------------------------------
  if (item->text(2) != "person")
  {
    mGeneral->clearControls();
    slotEnablePrinting (false);
    slotEnableDeleting (false);
    slotEnableSaving   (false);
    emit selectedItem  (tr("Contacts"), "");
    return;
  }
  
  //---------------------------------------------------------------------------
  // If the currently selected item was already loaded, nothing has to be done.
  //---------------------------------------------------------------------------
  if (mLastSelectedItem == item)
    return;

  mSelectedItem = item;

  if (item)
  {
    title = item->text(0);
    id    = item->text(1);
  }

  //----------------------------------------------------------------------------
  // Check first if there are modifications to be saved.
  //----------------------------------------------------------------------------
  if (hasContentChanged())
  {
    ret = QMessageBox::warning (this, tr ("Unsaved Modifications"),
              tr ("Data has been changed. Would you like to save it?"),
              QMessageBox::Yes | QMessageBox::Default,
              QMessageBox::No, QMessageBox::Cancel);

    switch (ret)
    {
      case QMessageBox::Yes:
        slotContactSave();
//        bUpdateTree = true;
        break;
      case QMessageBox::No:
        // Don't save modified data
        setContentChanged (false);
        slotEnableSaving  (false);
        break;
      case 2:
        // Abort saving
        return;
    }
  }

  //----------------------------------------------------------------------------
  // If item is NULL --> do nothing.
  //----------------------------------------------------------------------------
  if (!item)
    return;

  //----------------------------------------------------------------------------
  // Lock the person
  //----------------------------------------------------------------------------
  mUnlockStatement  = "";
  mSelectedReadonly = false;
  
  if (!lockEntry (id))
  {
    lockError          = true;
    mSelectedReadonly  = true;
  }

  //----------------------------------------------------------------------------
  // Load the data for the tab pages.
  //----------------------------------------------------------------------------
  mGeneral->loadData       (id, mSelectedReadonly);
  mCommunication->loadData (id, mSelectedReadonly);
  mAddress->loadData       (id, mSelectedReadonly);
  mJob->loadData           (id, mSelectedReadonly);
  mPrivate->loadData       (id, mSelectedReadonly);
  mSecurity->loadData      (id, mSelectedReadonly);
  
  //----------------------------------------------------------------------------
  // If the locking was not successful show the error message.
  //----------------------------------------------------------------------------
  if (lockError)
    QMessageBox::warning (this, tr ("Access Conflict"),
              tr ("The contact is currently used by another user.\n") + 
              tr ("It will be opened in readonly mode."),
              QMessageBox::Ok | QMessageBox::Default,
              QMessageBox::NoButton, QMessageBox::NoButton);

  //----------------------------------------------------------------------------
  // Emit the signal for changing the window caption.
  //----------------------------------------------------------------------------
  emit selectedItem (tr("Contacts"), 
                    title + (mSelectedReadonly ? tr (" (Readonly)") : QString("")));
}

//=============================================================================
// Check if the content of one of the tab pages has changed.
//=============================================================================
bool CContacts::hasContentChanged ()
{
  if (mGeneral->hasChanged())
    return true;

  if (mCommunication->hasChanged())
    return true;

  if (mAddress->hasChanged())
    return true;

  if (mJob->hasChanged())
    return true;

  if (mPrivate->hasChanged())
    return true;

  if (mSecurity->hasChanged())
    return true;

  return false;
}

//=============================================================================
// Set the Changed flag of all pages.
//=============================================================================
void CContacts::setContentChanged (bool flag)
{
  mGeneral->setChanged       (flag);
  mCommunication->setChanged (flag);
  mAddress->setChanged       (flag);
  mJob->setChanged           (flag);
  mPrivate->setChanged       (flag);
  mSecurity->setChanged      (flag);
}

//=============================================================================
// Lock the given person.
//=============================================================================
bool CContacts::lockEntry (QString id)
{
  QSqlQuery query ("LOCK (NOWAIT) ROW contacts_persons KEY person_id = " + id + 
                   " IN EXCLUSIVE MODE");

  if (!query.isActive())
  {
    if (query.lastError().number() != 400)
      SHOW_DB_ERROR(tr ("Error during locking"), query.lastQuery());

    return false;
  }
  
  mUnlockStatement = "UNLOCK ROW contacts_persons KEY person_id = " + id +
                     " IN EXCLUSIVE MODE";

  return true;
}

//=============================================================================
// Unlock the last locked person.
//=============================================================================
void CContacts::unlockEntry()
{
  if (!mUnlockStatement.isEmpty())
  {
    QSqlQuery query (mUnlockStatement);
    
    if (!query.isActive())
    {
      SHOW_DB_ERROR(tr ("Error during unlocking"), mUnlockStatement);
      return;
    }
  }
}

//=============================================================================
// SLOT:  Create a new contact.
//=============================================================================
void CContacts::slotContactNew()
{
  mStatusbar->message (tr("Creating new contact..."));

  mGeneral->newData();
  mCommunication->newData();
  mAddress->newData();
  mJob->newData();
  mPrivate->newData();
  mSecurity->newData();
  
  mStatusbar->message (tr("Ready."));
}

//=============================================================================
// SLOT:  Delete the current contact.
//=============================================================================
void CContacts::slotContactDelete()
{
  mStatusbar->message (tr("Deleting contact..."));
  
  mGeneral->deleteData         (mSelectedItem->text(1));
  mGeneral->clearControls      ();
  mCommunication->deleteData   (mSelectedItem->text(1));
  mCommunication->clearControls();
  mAddress->deleteData         (mSelectedItem->text(1));
  mAddress->clearControls      ();
  mJob->deleteData             (mSelectedItem->text(1));
  mJob->clearControls          ();
  mPrivate->deleteData         (mSelectedItem->text(1));
  mPrivate->clearControls      ();
  mSecurity->deleteData        (mSelectedItem->text(1));
  mSecurity->clearControls     ();
  
  updateTree();
  
  mStatusbar->message (tr("Ready."));
}

//=============================================================================
// SLOT:  Save the current contact.
//=============================================================================
void CContacts::slotContactSave()
{
  QString current;

  mStatusbar->message (tr("Saving contact..."));
  
  current = mGeneral->saveChanges();

  mCommunication->setCurrent (current);
  mCommunication->saveChanges();
  mAddress->setCurrent       (current);
  mAddress->saveChanges      ();
  mJob->setCurrent           (current);
  mJob->saveChanges          ();
  mPrivate->setCurrent       (current);
  mPrivate->saveChanges      ();
  mSecurity->setCurrent      (current);
  mSecurity->saveChanges     ();
  
  mDB->commit();
  updateTree();
  lockEntry (current);

  mContactSave->setEnabled   (false);
  mContactDelete->setEnabled (true);

  if (mSelectedItem)
    mTree->setSelected (mSelectedItem, true);
  
  mStatusbar->message (tr("Ready."));
}

//=============================================================================
// SLOT:  Close the contacts application.
//=============================================================================
void CContacts::slotContactClose()
{
  mStatusbar->message (tr("Closing Contacts..."));

  debug ("Closing Contacts");
  
  mStatusbar->message (tr("Ready."));
}

//=============================================================================
// SLOT:  Print the current contact.
//=============================================================================
void CContacts::slotContactPrint()
{
  mStatusbar->message (tr("Printing..."));
	
  mStatusbar->message (tr("Ready."));
}

//=============================================================================
// SLOT:  Undo the last action.
//=============================================================================
void CContacts::slotEditUndo()
{
  mStatusbar->message (tr("Reverting last action..."));
	
  mStatusbar->message (tr("Ready."));
}

//=============================================================================
// SLOT:  Cut the selection and insert it into the clipboard.
//=============================================================================
void CContacts::slotEditCut()
{
  mStatusbar->message (tr("Cutting selection..."));
	
  mStatusbar->message (tr("Ready."));
}

//=============================================================================
// SLOT:  Copy the selection into the clipboard.
//=============================================================================
void CContacts::slotEditCopy()
{
  mStatusbar->message (tr("Copying selection to clipboard..."));

  mStatusbar->message (tr("Ready."));
}

//=============================================================================
// SLOT:  Insert the content of the clipboard at the current position.
//=============================================================================
void CContacts::slotEditPaste()
{
  mStatusbar->message (tr("Inserting clipboard contents..."));

  mStatusbar->message (tr("Ready."));
}

//=============================================================================
// SLOT:  Toggle the toolbar on/off.
//=============================================================================
void CContacts::slotViewToolBar (bool toggle)
{
  mStatusbar->message (tr("Toggle toolbar..."));

  if (toggle)
    mContactToolbar->show();
  else
    mContactToolbar->hide();

 mStatusbar->message (tr("Ready."));
}

//=============================================================================
// SLOT:  Toggle the statusbar on/off.
//=============================================================================
void CContacts::slotViewStatusBar (bool toggle)
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
void CContacts::slotHelpContents()
{
  mStatusbar->message (tr("Showing the user manual contents..."));

  mStatusbar->message (tr("Ready."));
}

//=============================================================================
// SLOT:  Show the What's This cursor.
//=============================================================================
void CContacts::slotHelpWhatsThis()
{
  mStatusbar->message (tr("Showing information about widgets..."));

  QWhatsThis::enterWhatsThisMode();
  
  mStatusbar->message (tr("Ready."));
}

//=============================================================================
// SLOT:  Open the about dialog of the application.
//=============================================================================
void CContacts::slotHelpAbout()
{
  QMessageBox mb (tr("About Contacts"),
                  tr("Contacts ") + VERSION + "\n\n" +
                  tr("The Contacts Managing Application\n\n") +
                  tr("(c) 2002 by B-connected Linux Solutions GmbH"),
                  QMessageBox::Information,
                  QMessageBox::Ok | QMessageBox::Default,
                  QMessageBox::NoButton, QMessageBox::NoButton, this);
  mb.setIconPixmap (QPixmap (contacts_icon));

  mb.exec();
}

//=============================================================================
// SLOT:  Change the status message.
//=============================================================================
void CContacts::slotStatusHelpMsg (const QString &text)
{
  mStatusbar->message (text, 2000);
}

//=============================================================================
// SLOT: Enable the Save action.
//=============================================================================
void CContacts::slotEnableSaving (bool changed)
{
  mContactSave->setEnabled (changed);
}

//=============================================================================
// SLOT: Enable the Print action.
//=============================================================================
void CContacts::slotEnablePrinting (bool changed)
{
  mContactPrint->setEnabled (changed);
}

//=============================================================================
// SLOT: Enable the Delete action.
//=============================================================================
void CContacts::slotEnableDeleting (bool changed)
{
  mContactDelete->setEnabled (changed);
}

//=============================================================================
// Return the name of the plugin.
//=============================================================================
QString CContacts::name()
{
  return tr("Contacts");
}

//=============================================================================
// Return the group of the plugin.
//=============================================================================
QString CContacts::group()
{
  return "&PIM";
}

//=============================================================================
// Return the icon of the plugin.
//=============================================================================
QPixmap CContacts::icon()
{
  return QPixmap (contacts_icon);
}

//=============================================================================
// Return the tooltip text of the plugin.
//=============================================================================
QString CContacts::toolTip()
{
  return tr("Contacts Management");
}

//=============================================================================
// Return the What's This text of the plugin.
//=============================================================================
QString CContacts::whatsThis()
{
  return tr("Application for managing contacts");
}

//=============================================================================
// Return the summary information of the plugin.
//=============================================================================
QString CContacts::summary()
{
  return tr("Summary information");
}

//=============================================================================
// Insert the offered objects of the plugin as children into the given
// QListViewItem so that other plugins can use them for linking.
//=============================================================================
void CContacts::offeredObjects (QListViewItem *item)
{
}


