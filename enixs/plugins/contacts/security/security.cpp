//#############################################################################
//#  security.cpp
//#  ============
//#
//#  Creation:       Mon Jun 10 00:22:16 CEST 2002
//#  Author:         Ralf Schneider  (ralf.schneider@b-connected.de)
//#  Copyright:      (C) 2002 by B-connected Linux Solutions GmbH
//#
//#  ------------------------------------------------------------------------
//#
//#  This file contains the implementation of the class CSecurity.
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

//=============================================================================
// Include files for QT.
//=============================================================================
#include <qpixmap.h>
#include <qheader.h>

//=============================================================================
// Application specific includes.
//=============================================================================
#include "security.h"
#include "util.h"
#include "../friendlist/friendlist.h"

//=============================================================================
// Constructor of the class CSecurity.
//=============================================================================
CSecurity::CSecurity (QWidget *parent, const char *name, CConnection *db,
                      CUserData *current)
	: QWidget (parent,name)
{
  QLabel    *label1, *label2, *label3, *label4, *label5, *label6, *label7, *label8;
  
  mContentChanged = false;
  mNew            = false;
  mDB             = db;
  mCurrentUser    = current;

  //----------------------------------------------------------------------------
  //  Create the grid layout
  //----------------------------------------------------------------------------
  mGrid = new QGridLayout (this, 4, 3, 20, 12);
  mGrid->setColStretch    (4, 10);
  mGrid->addColSpacing    (0, 70);
  mGrid->addColSpacing    (1, 50);
  mGrid->addColSpacing    (2, 50);
  mGrid->addColSpacing    (3, 50);
  mGrid->addRowSpacing    (5, 60);

  //----------------------------------------------------------------------------
  //  Create the labels.
  //----------------------------------------------------------------------------
  label1 = new QLabel (tr ("Read"),   this);
  label2 = new QLabel (tr ("Write"),  this);
  label3 = new QLabel (tr ("Delete"), this);
  label4 = new QLabel (tr ("Link"),   this);
  label5 = new QLabel (tr ("Owner"),  this);
  label6 = new QLabel (tr ("Friends"),this);
  label7 = new QLabel (tr ("All"),    this);
  label8 = new QLabel (tr ("Friends"),this);
  
  mGrid->addWidget (label1, 1, 0);
  mGrid->addWidget (label2, 2, 0);
  mGrid->addWidget (label3, 3, 0);
  mGrid->addWidget (label4, 4, 0);
  mGrid->addWidget (label5, 0, 1, AlignHCenter);
  mGrid->addWidget (label6, 0, 2, AlignHCenter);
  mGrid->addWidget (label7, 0, 3, AlignHCenter);
  mGrid->addWidget (label8, 6, 0, AlignTop);

  //----------------------------------------------------------------------------
  //  Create the checkboxes.
  //----------------------------------------------------------------------------
  mOwnerRead    = new QCheckBox (this, "owner read");
  mOwnerWrite   = new QCheckBox (this, "owner write");
  mOwnerDelete  = new QCheckBox (this, "owner delete");
  mOwnerLink    = new QCheckBox (this, "owner link");
  mFriendRead   = new QCheckBox (this, "friend read");
  mFriendWrite  = new QCheckBox (this, "friend write");
  mFriendDelete = new QCheckBox (this, "friend delete");
  mFriendLink   = new QCheckBox (this, "friend link");
  mAllRead      = new QCheckBox (this, "all read");
  mAllWrite     = new QCheckBox (this, "all write");
  mAllDelete    = new QCheckBox (this, "all delete");
  mAllLink      = new QCheckBox (this, "all link");

  mGrid->addWidget (mOwnerRead,    1, 1, AlignHCenter);
  mGrid->addWidget (mOwnerWrite,   2, 1, AlignHCenter);
  mGrid->addWidget (mOwnerDelete,  3, 1, AlignHCenter);
  mGrid->addWidget (mOwnerLink,    4, 1, AlignHCenter);
  mGrid->addWidget (mFriendRead,   1, 2, AlignHCenter);
  mGrid->addWidget (mFriendWrite,  2, 2, AlignHCenter);
  mGrid->addWidget (mFriendDelete, 3, 2, AlignHCenter);
  mGrid->addWidget (mFriendLink,   4, 2, AlignHCenter);
  mGrid->addWidget (mAllRead,      1, 3, AlignHCenter);
  mGrid->addWidget (mAllWrite,     2, 3, AlignHCenter);
  mGrid->addWidget (mAllDelete,    3, 3, AlignHCenter);
  mGrid->addWidget (mAllLink,      4, 3, AlignHCenter);

  mPrivate = new QPushButton (tr ("Private"), this);
  mGrid->addWidget (mPrivate, 2, 4);
  connect (mPrivate, SIGNAL(clicked()), this, SLOT(slotSetPrivate()));

  //----------------------------------------------------------------------------
  //  Create the friends listbox.
  //----------------------------------------------------------------------------
  mFriends = new QListView         (this, "friends");
  mFriends->setFixedWidth          (250);
  mFriends->addColumn      		   (tr ("Name"), 300);
  mFriends->addColumn      		   (tr ("Type"), 90);
  mFriends->addColumn      		   (tr ("ID"), 90);
  mFriends->setAllColumnsShowFocus (true);
  mFriends->setHScrollBarMode      (QListView::AlwaysOff);
  mFriends->setSelectionMode 	   (QListView::Multi);
  mFriends->setSorting       	   (0);
  mFriends->header()->close();

  mGrid->addMultiCellWidget (mFriends, 6, 9, 1, 3);

  mAdd    = new QPushButton (tr ("Add"),    this);
  mRemove = new QPushButton (tr ("Remove"), this);

  connect (mAdd,    SIGNAL(clicked()), this, SLOT(slotAddFriend()));
  connect (mRemove, SIGNAL(clicked()), this, SLOT(slotRemoveFriend()));

  mGrid->addWidget (mAdd,    7, 4);
  mGrid->addWidget (mRemove, 8, 4);
  
  setReadonly (true);
  connectSlots();
}

//=============================================================================
// Destructor of the class CSecurity.
//=============================================================================
CSecurity::~CSecurity()
{
  FREE_PTR(mOwnerRead);
  FREE_PTR(mOwnerWrite);
  FREE_PTR(mOwnerDelete);
  FREE_PTR(mOwnerLink);
  FREE_PTR(mFriendRead);
  FREE_PTR(mFriendWrite);
  FREE_PTR(mFriendDelete);
  FREE_PTR(mFriendLink);
  FREE_PTR(mAllRead);
  FREE_PTR(mAllWrite);
  FREE_PTR(mAllDelete);
  FREE_PTR(mAllLink);
}

//=============================================================================
// Load the security information of the contact.
//=============================================================================
void CSecurity::loadData (QString id, bool readonly)
{
  QString 		sql, DBnull = "?", filename;
  QStringList	record, friends;
  unsigned int  i;
  
  //----------------------------------------------------------------------------
  // Disconnect the slots during data load.
  //----------------------------------------------------------------------------
  clearControls();
  disconnectSlots();

  //----------------------------------------------------------------------------
  // Load the user rights.
  //----------------------------------------------------------------------------
  sql = "SELECT owner_read, owner_write, owner_delete, owner_link, "
               "friend_read, friend_write, friend_delete, friend_link, "
               "all_read, all_write, all_delete, all_link, last_modified "
		"FROM   contacts_rights "
		"WHERE  person_id = " + id;

  if (!mDB->executeSQL (sql))
  {
    SHOW_DB_ERROR(tr ("Error during database query"), sql);
    return;
  }

  mDB->readResult (record);

  //----------------------------------------------------------------------------
  // Set the text of the checkbox controls.
  //----------------------------------------------------------------------------
  if (record.count() > 0)
  {
    mOwnerRead->setChecked    ((record[0]  == "TRUE") ? true : false);
    mOwnerWrite->setChecked   ((record[1]  == "TRUE") ? true : false);
    mOwnerDelete->setChecked  ((record[2]  == "TRUE") ? true : false);
    mOwnerLink->setChecked    ((record[3]  == "TRUE") ? true : false);
    mFriendRead->setChecked   ((record[4]  == "TRUE") ? true : false);
    mFriendWrite->setChecked  ((record[5]  == "TRUE") ? true : false);
    mFriendDelete->setChecked ((record[6]  == "TRUE") ? true : false);
    mFriendLink->setChecked   ((record[7]  == "TRUE") ? true : false);
    mAllRead->setChecked      ((record[8]  == "TRUE") ? true : false);
    mAllWrite->setChecked     ((record[9]  == "TRUE") ? true : false);
    mAllDelete->setChecked    ((record[10] == "TRUE") ? true : false);
    mAllLink->setChecked      ((record[11] == "TRUE") ? true : false);
  }

  //----------------------------------------------------------------------------
  // Load the list of friends.
  //----------------------------------------------------------------------------
  sql = "SELECT id, type FROM contacts_friends WHERE person_id = " + id;

  if (!mDB->executeSQL (sql))
  {
    SHOW_DB_ERROR(tr ("Error during database query"), sql);
    return;
  }

  while (mDB->readResult (record))
    friends << record[0] << record[1];

  //----------------------------------------------------------------------------
  // Load the names of the friends.
  //----------------------------------------------------------------------------
  for (i = 0; i < friends.count(); i += 2)
  {
    if (friends[i + 1] == "0")
      sql = "SELECT name FROM enixs_users WHERE user_id = " + friends[i];
    else
      sql = "SELECT name FROM enixs_groups WHERE group_id = " + friends[i];

    if (!mDB->executeSQL (sql))
    {
      SHOW_DB_ERROR(tr ("Error during database query"), sql);
      return;
    }

    mDB->readResult (record);

    new QListViewItem (mFriends, record[0], friends[i + 1], friends[i]);
  }
  
  //----------------------------------------------------------------------------
  // Store the ID of the currently loaded person.
  //----------------------------------------------------------------------------
  mCurrent  = id;
  mReadonly = readonly;
  
  //----------------------------------------------------------------------------
  // Connect the slots after data load is complete.
  //----------------------------------------------------------------------------
  connectSlots();
  setReadonly (readonly);
  
  emit contentLoaded  (true);
  emit contentChanged (false);
}

//=============================================================================
// Delete the person.
//=============================================================================
void CSecurity::deleteData (QString id)
{
  QString 	  sql;
  
  //----------------------------------------------------------------------------
  // Delete the rights of the contact
  //----------------------------------------------------------------------------
  sql = "DELETE FROM contacts_rights WHERE person_id = " + id;

  if (!mDB->executeSQL (sql))
  {
    SHOW_DB_ERROR(tr ("Error during deleting the data"), sql);
    return;
  }

  //----------------------------------------------------------------------------
  // Delete the friends of the contact
  //----------------------------------------------------------------------------
  sql = "DELETE FROM contacts_friends WHERE person_id = " + id;

  if (!mDB->executeSQL (sql))
  {
    SHOW_DB_ERROR(tr ("Error during deleting the data"), sql);
    return;
  }

  //----------------------------------------------------------------------------
  // Commit the current transaction.
  //----------------------------------------------------------------------------
  mDB->commit();

  mContentChanged = false;
}

//=============================================================================
// Write the current values into the database.
//=============================================================================
QString CSecurity::saveChanges ()
{
  QString 	     sql, intro, firstValue, gender, birthday;
  QString 	     ownerRead, ownerWrite, ownerDelete, ownerLink;
  QString 	     friendRead, friendWrite, friendDelete, friendLink;
  QString 	     allRead, allWrite, allDelete, allLink;
  QListViewItem* item;

  //----------------------------------------------------------------------------
  // If nothing was changed, nothing has to be done.
  //----------------------------------------------------------------------------
  if (!mContentChanged)
    return mCurrent;
  
  //----------------------------------------------------------------------------
  // Save the rights of the contact.
  //----------------------------------------------------------------------------
  intro        = mNew ? "INSERT contacts_rights (person_id, " : "UPDATE contacts_rights (";
  firstValue   = mNew ? mCurrent + QString (", ") : QString ("");
  ownerRead    = mOwnerRead->isChecked()    ? QString("TRUE") : QString("FALSE");
  ownerWrite   = mOwnerWrite->isChecked()   ? QString("TRUE") : QString("FALSE");
  ownerDelete  = mOwnerDelete->isChecked()  ? QString("TRUE") : QString("FALSE");
  ownerLink    = mOwnerLink->isChecked()    ? QString("TRUE") : QString("FALSE");
  friendRead   = mFriendRead->isChecked()   ? QString("TRUE") : QString("FALSE");
  friendWrite  = mFriendWrite->isChecked()  ? QString("TRUE") : QString("FALSE");
  friendDelete = mFriendDelete->isChecked() ? QString("TRUE") : QString("FALSE");
  friendLink   = mFriendLink->isChecked()   ? QString("TRUE") : QString("FALSE");
  allRead      = mAllRead->isChecked()      ? QString("TRUE") : QString("FALSE");
  allWrite     = mAllWrite->isChecked()     ? QString("TRUE") : QString("FALSE");
  allDelete    = mAllDelete->isChecked()    ? QString("TRUE") : QString("FALSE");
  allLink      = mAllLink->isChecked()      ? QString("TRUE") : QString("FALSE");
  
  sql = intro + "owner_read, owner_write, owner_delete, owner_link, "
                "friend_read, friend_write, friend_delete, friend_link, "
                "all_read, all_write, all_delete, all_link, last_modified) "
		"VALUES (" + firstValue + ownerRead + ", " + ownerWrite + ", "
                 + ownerDelete + ", " + ownerLink + ", " + friendRead + ", "
                 + friendWrite + ", " + friendDelete + ", " + friendLink + ", "
                 + allRead + ", " + allWrite + ", " + allDelete + ", " + allLink +
                 ", TIMESTAMP) ";

  if (!mNew)
    sql += "WHERE person_id = " + mCurrent;
  
  //----------------------------------------------------------------------------
  // Execute the SQL statement.
  //----------------------------------------------------------------------------
  if (!mDB->executeSQL (sql))
  {
    SHOW_DB_ERROR(tr ("Error during writing of data"), sql);
    return "";
  }
  
  //----------------------------------------------------------------------------
  // Add the newly added friends of the contact.
  //----------------------------------------------------------------------------
  for (item = mAddedList.first(); item; item = mAddedList.next())
  {
    sql = "INSERT contacts_friends (person_id, type, id) "
          "VALUES (" + mCurrent + ", " + (item->text(1) == "user" ? "0" : "1") + 
          ", " + item->text(2) + ")";
    
    if (!mDB->executeSQL (sql))
    {
      SHOW_DB_ERROR(tr ("Error during writing of data"), sql);
      return "";
    }
  }

  mAddedList.clear();
  
  //----------------------------------------------------------------------------
  // Delete the removed friends of the contact.
  //----------------------------------------------------------------------------
  for (item = mRemovedList.first(); item; item = mRemovedList.next())
  {
    sql = "DELETE FROM contacts_friends WHERE person_id = " + mCurrent + 
          " AND id = " + item->text(2);
  
    if (!mDB->executeSQL (sql))
    {
      SHOW_DB_ERROR(tr ("Error during writing of data"), sql);
      return "";
    }
  }
  
  mRemovedList.clear();
  
  mContentChanged = false;

  return mCurrent;
}

//=============================================================================
// SLOT: Notice that the content has changed
//=============================================================================
void CSecurity::slotContentChanged (const QString &text)
{
  mContentChanged = true;
  emit contentChanged (true);
}

//=============================================================================
// SLOT: Notice that the content has changed
//=============================================================================
void CSecurity::slotContentChanged ()
{
  mContentChanged = true;
  emit contentChanged (true);
}

//=============================================================================
// SLOT: Add a friend to the list.
//=============================================================================
void CSecurity::slotAddFriend ()
{
  CFriendList   dlg (this, "friend list", false, mDB);
  
  if (!dlg.exec())
    return;

  if (insertSelection (mFriends, mAddedList, dlg.selection()))
    slotContentChanged();  
}

//=============================================================================
// SLOT: Remove a friend from the list.
//=============================================================================
void CSecurity::slotRemoveFriend ()
{
  //----------------------------------------------------------------------------
  // If nothing is selected --> return.
  //----------------------------------------------------------------------------
  if (selectedFriends().isEmpty())
  {
    QMessageBox::warning (this, tr ("No Selection"),
              tr ("Please select an item of the list first."),
              QMessageBox::Ok | QMessageBox::Default,
              QMessageBox::NoButton, QMessageBox::NoButton);
    return;
  }

  //----------------------------------------------------------------------------
  // Ask the user if he realy wants to remove the selected items.
  //----------------------------------------------------------------------------
  if (QMessageBox::warning (this, tr ("Confirmation"),
              tr ("Do you really want to remove the selected items?"),
              QMessageBox::Yes | QMessageBox::Default,
              QMessageBox::No, QMessageBox::NoButton) == QMessageBox::No)
    return;

  //----------------------------------------------------------------------------
  // Remove the selected item from the listview and add them to
  // the removedList.
  //----------------------------------------------------------------------------
  removeSelection 	(mFriends, mRemovedList, selectedFriends());
  slotContentChanged();
}

//=============================================================================
// SLOT: Make the contact private.
//=============================================================================
void CSecurity::slotSetPrivate()
{
  mFriendRead->setChecked   (false);
  mFriendWrite->setChecked  (false);
  mFriendDelete->setChecked (false);
  mFriendLink->setChecked   (false);
  mAllRead->setChecked      (false);
  mAllWrite->setChecked     (false);
  mAllDelete->setChecked    (false);
  mAllLink->setChecked      (false);
}

//=============================================================================
// Return a list of the currently selected items of the friends list.
//=============================================================================
QPtrList<QListViewItem> CSecurity::selectedFriends()
{
  QPtrList<QListViewItem>   result;
  QListViewItem            *item;
  
  for (item = mFriends->firstChild(); item; item = item->nextSibling())
    if (item->isSelected())
      result.append (item);
  
  return result;
}

//=============================================================================
// Clear all controls.
//=============================================================================
void CSecurity::clearControls()
{
  disconnectSlots();

  mOwnerRead->setChecked    (true);
  mOwnerWrite->setChecked   (true);
  mOwnerDelete->setChecked  (true);
  mOwnerLink->setChecked    (true);
  mFriendRead->setChecked   (true);
  mFriendWrite->setChecked  (true);
  mFriendDelete->setChecked (true);
  mFriendLink->setChecked   (true);
  mAllRead->setChecked      (true);
  mAllWrite->setChecked     (false);
  mAllDelete->setChecked    (false);
  mAllLink->setChecked      (true);

  mFriends->clear();
  
  connectSlots();
}

//=============================================================================
// Make all controls readonly.
//=============================================================================
void CSecurity::setReadonly (bool readonly)
{
  mOwnerRead->setEnabled    (!readonly);
  mOwnerWrite->setEnabled   (!readonly);
  mOwnerDelete->setEnabled  (!readonly);
  mOwnerLink->setEnabled    (!readonly);
  mFriendRead->setEnabled   (!readonly);
  mFriendWrite->setEnabled  (!readonly);
  mFriendDelete->setEnabled (!readonly);
  mFriendLink->setEnabled   (!readonly);
  mAllRead->setEnabled      (!readonly);
  mAllWrite->setEnabled     (!readonly);
  mAllDelete->setEnabled    (!readonly);
  mAllLink->setEnabled      (!readonly);
  mFriends->setEnabled      (!readonly);
  mAdd->setEnabled          (!readonly);
  mRemove->setEnabled       (!readonly);
}

//=============================================================================
// Connect the slots to the signals.
//=============================================================================
void CSecurity::connectSlots()
{
  connect (mOwnerRead,    SIGNAL (toggled     	     (bool)),
           this,          SLOT   (slotContentChanged ()));
  connect (mOwnerWrite,   SIGNAL (toggled            (bool)),
           this,          SLOT   (slotContentChanged ()));
  connect (mOwnerDelete,  SIGNAL (toggled            (bool)),
           this,          SLOT   (slotContentChanged ()));
  connect (mOwnerLink,    SIGNAL (toggled            (bool)),
           this,          SLOT   (slotContentChanged ()));
  connect (mFriendRead,   SIGNAL (toggled     	     (bool)),
           this,          SLOT   (slotContentChanged ()));
  connect (mFriendWrite,  SIGNAL (toggled            (bool)),
           this,          SLOT   (slotContentChanged ()));
  connect (mFriendDelete, SIGNAL (toggled            (bool)),
           this,          SLOT   (slotContentChanged ()));
  connect (mFriendLink,   SIGNAL (toggled            (bool)),
           this,          SLOT   (slotContentChanged ()));
  connect (mAllRead,      SIGNAL (toggled     	     (bool)),
           this,          SLOT   (slotContentChanged ()));
  connect (mAllWrite,     SIGNAL (toggled            (bool)),
           this,          SLOT   (slotContentChanged ()));
  connect (mAllDelete,    SIGNAL (toggled            (bool)),
           this,          SLOT   (slotContentChanged ()));
  connect (mAllLink,      SIGNAL (toggled            (bool)),
           this,          SLOT   (slotContentChanged ()));
}

//=============================================================================
// Disconnect the slots from the signals.
//=============================================================================
void CSecurity::disconnectSlots()
{
  mOwnerRead->disconnect   ();
  mOwnerWrite->disconnect  ();
  mOwnerDelete->disconnect ();
  mOwnerLink->disconnect   ();
  mFriendRead->disconnect  ();
  mFriendWrite->disconnect ();
  mFriendDelete->disconnect();
  mFriendLink->disconnect  ();
  mAllRead->disconnect     ();
  mAllWrite->disconnect    ();
  mAllDelete->disconnect   ();
  mAllLink->disconnect     ();
}
