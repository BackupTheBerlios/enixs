//#############################################################################
//#  private.cpp
//#  ===========
//#
//#  Creation:       Sat Jun 08 23:20:16 CEST 2002
//#  Author:         Ralf Schneider  (ralf.schneider@b-connected.de)
//#  Copyright:      (C) 2002 by B-connected Linux Solutions GmbH
//#
//#  ------------------------------------------------------------------------
//#
//#  This file contains the implementation of the class CPrivate.
//#  This class contains all the widgets for displaying the private details
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

//=============================================================================
// Application specific includes.
//=============================================================================
#include "private.h"
#include "util.h"
#include "../validators/datevalidator.h"

//=============================================================================
// Constructor of the class CPrivate.
//=============================================================================
CPrivate::CPrivate (QWidget *parent, const char *name, QSqlDatabase *db,
                    CUserData *current)
	: QWidget (parent,name)
{
  QLabel    *label1, *label2, *label3, *label4;
  
  mContentChanged = false;
  mNew            = false;
  mDB             = db;
  mCurrentUser    = current;

  //----------------------------------------------------------------------------
  //  Create the grid layout
  //----------------------------------------------------------------------------
  mGrid = new QGridLayout (this, 4, 3, 20, 12);
//mGrid->setRowStretch    (2, 10);
  mGrid->setRowStretch    (3, 10);
  
//  mGrid->addRowSpacing    (4, 5);
//  mGrid->addRowSpacing    (7, 15);

  //----------------------------------------------------------------------------
  //  Create the labels.
  //----------------------------------------------------------------------------
  label1 = new QLabel (tr ("Nickname"), this);
  label2 = new QLabel (tr ("Spouse"),   this);
  label3 = new QLabel (tr ("Children"), this);
  label4 = new QLabel (tr ("Hobbies"),  this);

  mGrid->addWidget (label1, 0, 0, AlignTop);
  mGrid->addWidget (label2, 1, 0, AlignTop);
  mGrid->addWidget (label3, 2, 0, AlignTop);
  mGrid->addWidget (label4, 3, 0, AlignTop);

  //----------------------------------------------------------------------------
  //  Create the lineedits.
  //----------------------------------------------------------------------------
  mNickname = new QLineEdit (this, "nickname");
  mSpouse   = new QLineEdit (this, "spouse");
  mChildren = new CTextEdit (this, "children");
  mHobbies  = new CTextEdit (this, "hobbies");

  mNickname->setMaxLength (128);
  mSpouse->setMaxLength   (128);
  mChildren->setMaxLength (255);
  mHobbies->setMaxLength  (255);

  mGrid->addWidget (mNickname, 0, 1, AlignTop);
  mGrid->addWidget (mSpouse,   1, 1, AlignTop);
  mGrid->addWidget (mChildren, 2, 1, AlignTop);
  mGrid->addWidget (mHobbies,  3, 1, AlignTop);

  setReadonly (true);
  connectSlots();
}

//=============================================================================
// Destructor of the class CPrivate.
//=============================================================================
CPrivate::~CPrivate()
{
  FREE_PTR(mSpouse);
  FREE_PTR(mChildren);
  FREE_PTR(mNickname);
  FREE_PTR(mHobbies);
}

//=============================================================================
// Load the private information of the contact.
//=============================================================================
void CPrivate::loadData (QString id, bool readonly)
{
  QString 	DBnull = "?", filename;
  
  //----------------------------------------------------------------------------
  // Disconnect the slots during data load.
  //----------------------------------------------------------------------------
  clearControls();
  disconnectSlots();
  
  //----------------------------------------------------------------------------
  // Load the selected person.
  //----------------------------------------------------------------------------
  QSqlQuery query ("SELECT spouse, children, nickname, hobbies "
                   "FROM   contacts_persons "
                   "WHERE  person_id = " + id);

  if (!query.isActive())
  {
    SHOW_DB_ERROR(tr ("Error during database query"), query);
    return;
  }

  query.first();

  //----------------------------------------------------------------------------
  // Set the text of the line edit controls.
  //----------------------------------------------------------------------------
  if (query.value(0).toString() != DBnull)
    mSpouse->setText (query.value(0).toString());

  if (query.value(1).toString() != DBnull)
    mChildren->setText (query.value(1).toString());
  
  if (query.value(2).toString() != DBnull)
    mNickname->setText (query.value(2).toString());
  
  if (query.value(3).toString() != DBnull)
    mHobbies->setText (query.value(3).toString());
  
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
void CPrivate::deleteData (QString id)
{
  // Nothing has to be done here, because everthing is deleted by CGeneral
}

//=============================================================================
// Write the current values into the database.
//=============================================================================
QString CPrivate::saveChanges ()
{
  QString 	intro, firstValue, gender, birthday;

  //----------------------------------------------------------------------------
  // If nothing was changed, nothing has to be done.
  //----------------------------------------------------------------------------
  if (!mContentChanged)
    return mCurrent;
  
  //----------------------------------------------------------------------------
  // Execute the SQL statement.
  //----------------------------------------------------------------------------
  QSqlQuery query ("UPDATE contacts_persons (spouse, children, nickname, hobbies, "
                   "last_modified) VALUES ('"  + textToDB (mSpouse->text()) + "', "
                   "'" + textToDB (mChildren->text()) + "', "
                   "'" + textToDB (mNickname->text()) + "', "
                   "'" + textToDB (mHobbies->text()) + "', TIMESTAMP) "
                   "WHERE person_id = " + mCurrent);
  
  if (!query.isActive())
  {
    SHOW_DB_ERROR(tr ("Error during writing of data"), query);
    return "";
  }

  mContentChanged = false;

  return mCurrent;
}

//=============================================================================
// SLOT: Notice that the content has changed
//=============================================================================
void CPrivate::slotContentChanged (const QString &text)
{
	mContentChanged = true;
	emit contentChanged (true);
}

//=============================================================================
// SLOT: Notice that the content has changed
//=============================================================================
void CPrivate::slotContentChanged ()
{
	mContentChanged = true;
	emit contentChanged (true);
}

//=============================================================================
// Clear all controls.
//=============================================================================
void CPrivate::clearControls()
{
  disconnectSlots();

  mSpouse->clear();
  mChildren->clear();
  mNickname->clear();
  mHobbies->clear();

  connectSlots();
}

//=============================================================================
// Make all controls readonly.
//=============================================================================
void CPrivate::setReadonly (bool readonly)
{
  mSpouse->setReadOnly   (readonly);
  mChildren->setReadOnly (readonly);
  mNickname->setReadOnly (readonly);
  mHobbies->setReadOnly  (readonly);
}

//=============================================================================
// Connect the slots to the signals.
//=============================================================================
void CPrivate::connectSlots()
{
  connect (mNickname, SIGNAL (textChanged 	     (const QString &)),
           this,      SLOT   (slotContentChanged (const QString &)));
  connect (mSpouse,   SIGNAL (textChanged        (const QString &)),
           this,      SLOT   (slotContentChanged (const QString &)));
  connect (mChildren, SIGNAL (textChanged 	     ()),
           this,      SLOT   (slotContentChanged ()));
  connect (mHobbies,  SIGNAL (textChanged 	     ()),
           this,      SLOT   (slotContentChanged ()));
}

//=============================================================================
// Disconnect the slots from the signals.
//=============================================================================
void CPrivate::disconnectSlots()
{
  mSpouse->disconnect  ();
  mChildren->disconnect();
  mNickname->disconnect();
  mHobbies->disconnect ();
}
