//#############################################################################
//#  general.cpp
//#  ===========
//#
//#  Creation:       Fri May 17 20:15:16 CEST 2002
//#  Author:         Ralf Schneider  (ralf.schneider@b-connected.de)
//#  Copyright:      (C) 2002 by B-connected Linux Solutions GmbH
//#
//#  ------------------------------------------------------------------------
//#
//#  This file contains the implementation of the class CGeneral.
//#  This class contains all the widgets for displaying the general details
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
#include <qtextcodec.h>
#include <qsqlcursor.h>

//=============================================================================
// Application specific includes.
//=============================================================================
#include "general.h"
#include "util.h"
#include "../validators/datevalidator.h"

//=============================================================================
// Constructor of the class CGeneral.
//=============================================================================
CGeneral::CGeneral (QWidget *parent, const char *name, QSqlDatabase *db,
                    CUserData *current)
	: QWidget (parent,name)
{
  mContentChanged = false;
  mNew            = false;
  mDB             = db;
  mCurrentUser    = current;

  //----------------------------------------------------------------------------
  //  Create the grid layout
  //----------------------------------------------------------------------------
  mGrid = new QGridLayout (this, 7, 3, 20, 12);
  mGrid->addRowSpacing    (1, 15);
  mGrid->addRowSpacing    (6, 5);
  mGrid->addRowSpacing    (9, 15);
  
  //----------------------------------------------------------------------------
  //  Photo
  //----------------------------------------------------------------------------
  mPhoto = new CPhoto       (tr ("Photo"), this);
  mPhoto->setAlignment      (AlignCenter);
  mPhoto->setFrameStyle     (QFrame::StyledPanel | QFrame::Sunken);
  mPhoto->setFixedSize      (110, 135);
  mGrid->addMultiCellWidget (mPhoto, 4, 8, 7, 7);

  
  //----------------------------------------------------------------------------
  //  Category
  //----------------------------------------------------------------------------
  mCategoryL = new QLabel (tr ("Category"), this);
  mGrid->addWidget        (mCategoryL, 0, 0);
  
  mCategory = new QComboBox (this, "category");
  mCategory->setMaximumWidth(150);
  mGrid->addWidget          (mCategory, 0, 2);

  loadCategories (mCategories, mCurrentUser->id());

  QAsciiDictIterator<QString> it(mCategories);
  for( ; it.current(); ++it )
    mCategory->insertItem (*(it.current()));
  
  //----------------------------------------------------------------------------
  //  Name
  //----------------------------------------------------------------------------
  mNameL = new QLabel (tr ("Name"), this);
  mGrid->addWidget    (mNameL, 2, 0);
  
  mName = new QLineEdit     (this, "name");
  mName->setMaxLength       (128);
  mGrid->addMultiCellWidget (mName, 2, 2, 2, 7);

  //----------------------------------------------------------------------------
  //  First Name
  //----------------------------------------------------------------------------
  mFirstL = new QLabel (tr ("First Name"), this);
  mGrid->addWidget     (mFirstL, 3, 0);
  
  mFirst = new QLineEdit    (this, "first name");
  mFirst->setMaxLength      (128);
  mGrid->addMultiCellWidget (mFirst, 3, 3, 2, 7);

  //----------------------------------------------------------------------------
  //  Name Suffix
  //----------------------------------------------------------------------------
  mSuffixL = new QLabel (tr ("Name Suffix"), this);
  mGrid->addWidget      (mSuffixL, 4, 0);
  
  mSuffix = new QLineEdit (this, "name suffix");
  mSuffix->setMaxLength   (16);
  mGrid->addWidget        (mSuffix, 4, 2);

  //----------------------------------------------------------------------------
  //  Title
  //----------------------------------------------------------------------------
  mTitleL = new QLabel (tr ("Title"), this);
  mGrid->addWidget     (mTitleL, 5, 0);
  
  mTitle = new QLineEdit (this, "title");
  mTitle->setMaxLength   (32);
  mGrid->addWidget       (mTitle, 5, 2);
  
  //----------------------------------------------------------------------------
  //  Birthday
  //----------------------------------------------------------------------------
  mBirthdayL = new QLabel (tr ("Birthday"), this);
  mGrid->addWidget        (mBirthdayL, 7, 0);
  
  mBirthday = new QLineEdit (this, "birthday");
  mBirthday->setMaxLength   (10);
  mBirthday->setMaximumWidth(90);
  mBirthday->setValidator   (new CDateValidator (mBirthday));
  mGrid->addWidget          (mBirthday, 7, 2);

  //----------------------------------------------------------------------------
  //  Gender
  //----------------------------------------------------------------------------
  mGenderL = new QLabel (tr ("Gender"), this);
  mGrid->addWidget      (mGenderL, 8, 0);
  
  mGender = new QComboBox (this, "gender");
  mGender->insertItem     (tr ("Male"));
  mGender->insertItem     (tr ("Female"));
  mGender->setMaximumWidth(90);
  mGrid->addWidget        (mGender, 8, 2);

  //----------------------------------------------------------------------------
  //  Company
  //----------------------------------------------------------------------------
  mCompanyL = new QLabel (tr ("Company"), this);
  mGrid->addWidget       (mCompanyL, 10, 0);
  
  mCompany = new QLineEdit  (this, "company");
  mCompany->setMaxLength    (128);
  mGrid->addMultiCellWidget (mCompany, 10, 10, 2, 7);

  //----------------------------------------------------------------------------
  //  Profession
  //----------------------------------------------------------------------------
  mProfessionL = new QLabel (tr ("Profession"), this);
  mGrid->addWidget          (mProfessionL, 11, 0);
  
  mProfession = new QLineEdit (this, "profession");
  mProfession->setMaxLength   (128);
  mGrid->addMultiCellWidget   (mProfession, 11, 11, 2, 7);

  //----------------------------------------------------------------------------
  //  Commment
  //----------------------------------------------------------------------------
  mCommentL = new QLabel (tr ("Comment"), this);
  mCommentL->setAlignment(AlignLeft | AlignTop);
  mGrid->addWidget       (mCommentL, 13, 0);
  
  mComment = new CTextEdit  (this, "comment");
  mComment->setMaxLength    (1024);
  mGrid->addMultiCellWidget (mComment, 13, 13, 2, 7);

  setReadonly (true);
  connectSlots();
}

//=============================================================================
// Destructor of the class CGeneral.
//=============================================================================
CGeneral::~CGeneral()
{
  FREE_PTR(mPhoto);
  FREE_PTR(mCategoryL);
  FREE_PTR(mCategory);
  FREE_PTR(mNameL);
  FREE_PTR(mName);
  FREE_PTR(mFirstL);
  FREE_PTR(mFirst);
  FREE_PTR(mSuffixL);
  FREE_PTR(mSuffix);
  FREE_PTR(mTitleL);
  FREE_PTR(mTitle);
  FREE_PTR(mBirthdayL);
  FREE_PTR(mBirthday);
  FREE_PTR(mGenderL);
  FREE_PTR(mGender);
  FREE_PTR(mCompanyL);
  FREE_PTR(mCompany);
  FREE_PTR(mProfessionL);
  FREE_PTR(mProfession);
  FREE_PTR(mCommentL);
  FREE_PTR(mComment);
}

//=============================================================================
// Load the detail information of the company.
//=============================================================================
void CGeneral::loadData (QString id, bool readonly)
{
  QString 			DBnull = "?", filename;
  QPixmap           photo;
  
  //----------------------------------------------------------------------------
  // Disconnect the slots during data load.
  //----------------------------------------------------------------------------
  clearControls();
  disconnectSlots();
  
  //----------------------------------------------------------------------------
  // Load the selected person.
  //----------------------------------------------------------------------------
  QSqlQuery query ("SELECT name, first_name, name_suffix, title, birthday, male, "
                   "company, profession, comment, category, photo_available, photo "
                   "FROM   contacts_persons "
                   "WHERE  person_id = " + id);

  if (!query.isActive())
  {
    SHOW_DB_ERROR(tr ("Error during database query"), query);
    return;
  }

  query.first();

  //----------------------------------------------------------------------------
  // Set the text of the single and multi line edit controls and the labels.
  //----------------------------------------------------------------------------
  if (query.value(0).toString() != DBnull)
    mName->setText (query.value(0).toString());

  if (query.value(1).toString() != DBnull)
    mFirst->setText (query.value(1).toString());
  
  if (query.value(2).toString() != DBnull)
    mSuffix->setText (query.value(2).toString());
  
  if (query.value(3).toString() != DBnull)
    mTitle->setText (query.value(3).toString());
  
  if (query.value(4).toString() != DBnull)
    mBirthday->setText (formatDate (query.value(4).toString()));
  
  if (query.value(5).toBool())
    mGender->setCurrentItem (0);
  else
    mGender->setCurrentItem (1);
  
  if (query.value(6).toString() != DBnull)
    mCompany->setText (query.value(6).toString());
  
  if (query.value(7).toString() != DBnull)
    mProfession->setText (query.value(7).toString());
  
  if (query.value(8).toString() != DBnull)
    mComment->setText (query.value(8).toString());
  
  if (query.value(9).toString() != DBnull)
    mCategory->setCurrentItem (query.value(9).toInt());

  //----------------------------------------------------------------------------
  // Load the photo of the selected person.
  //----------------------------------------------------------------------------
  if (query.value(10).toBool())
    mPhoto->setPixmap (QPixmap (query.value(11).toByteArray()));
  
  //----------------------------------------------------------------------------
  // Store the ID of the currently loaded person.
  //----------------------------------------------------------------------------
  mCurrent = id;

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
void CGeneral::deleteData (QString id)
{
  //----------------------------------------------------------------------------
  // Delete the person.
  //----------------------------------------------------------------------------
  QSqlQuery query ("DELETE FROM contacts_persons WHERE person_id = " + id);

  //----------------------------------------------------------------------------
  // Execute the SQL statement.
  //----------------------------------------------------------------------------
  if (!query.isActive())
  {
    SHOW_DB_ERROR(tr ("Error during deleting the data"), query);
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
QString CGeneral::saveChanges ()
{
  QString 	   sql, intro, firstValue, gender, birthday;
  QSqlRecord*  buffer;

  //----------------------------------------------------------------------------
  // If nothing was changed, nothing has to be done.
  //----------------------------------------------------------------------------
  if (!mContentChanged)
    return mCurrent;
  
  //----------------------------------------------------------------------------
  // Build the SQL statement.
  //----------------------------------------------------------------------------
  if (mNew)
    mCurrent = getNextID();

  intro      = mNew ? "INSERT contacts_persons (person_id, " : "UPDATE contacts_persons (";
  firstValue = mNew ? mCurrent + QString (", ") : QString ("");
  gender     = (mGender->currentItem() == 0) ? QString("TRUE") : QString("FALSE");
  birthday   = mBirthday->text().isEmpty() 
                     ? QString("NULL") : "'" + dateToDB (mBirthday->text()) + "'";

  sql = intro + "name, first_name, name_suffix, title, birthday, male, company, "
                "profession, comment, last_modified) "
		"VALUES (" + firstValue + "'" + textToDB (mName->text()) + "', "
                 "'" + textToDB (mFirst->text()) + "', "
                 "'" + textToDB (mSuffix->text()) + "', "
                 "'" + textToDB (mTitle->text()) + "', " +
                 birthday + ", " + gender + ", "
                 "'" + textToDB (mCompany->text()) + "', "
                 "'" + textToDB (mProfession->text()) + "', "
                 "'" + textToDB (mComment->text()) + "', TIMESTAMP) ";

  if (!mNew)
    sql += "WHERE person_id = " + mCurrent;
  
  //----------------------------------------------------------------------------
  // Execute the SQL statement.
  //----------------------------------------------------------------------------
  QSqlQuery query (sql);
  
  if (!query.isActive())
  {
    SHOW_DB_ERROR(tr ("Error during writing of data"), query);
    return "";
  }

  //----------------------------------------------------------------------------
  // Save the photo if it was changed.
  //----------------------------------------------------------------------------
  if (mPhoto->hasChanged())
  {
    debug ("Foto wird gespeichert");
    QSqlCursor cursor ("CONTACTS_PERSONS");
    
    if (mNew)
      buffer = cursor.primeInsert();
    else
      buffer = cursor.primeUpdate();

    buffer->setValue ("person_id", mCurrent);
    if (mPhoto->isNull())
    {
      debug ("Foto ist NULL");
      buffer->setValue ("photo_available", QVariant (false, 0));
      buffer->setValue ("photo",           QVariant ("NULL"));
    }
    else
    {
      debug ("Foto ist nicht NULL");
      buffer->setValue ("photo_available", QVariant (true, 0));
      buffer->setValue ("photo", QVariant (*mPhoto->pixmap()).toByteArray());
    }
    if (!cursor.update())
      debug (cursor.lastError().driverText());
  }
  
  mContentChanged = false;
  mNew            = false;

  return mCurrent;
}

//=============================================================================
// Determine the next available person ID for a person.
//=============================================================================
QString CGeneral::getNextID ()
{
  int   id;

  //----------------------------------------------------------------------------
  // Read the maximum person ID.
  //----------------------------------------------------------------------------
  QSqlQuery query ("SELECT max(person_id) FROM contacts_persons");
  
  if (!query.isActive())
  {
    SHOW_DB_ERROR(tr ("Error during database query"), query);
    return "";
  }

  query.first();

  //----------------------------------------------------------------------------
  // Increment the current max ID and return this value.
  //----------------------------------------------------------------------------
  id = query.value(0).toInt();
	
  return QString::number (++id);
}

//=============================================================================
// SLOT: Notice that the content has changed
//=============================================================================
void CGeneral::slotContentChanged (const QString &text)
{
	mContentChanged = true;
	emit contentChanged (true);
}

//=============================================================================
// SLOT: Notice that the content has changed
//=============================================================================
void CGeneral::slotContentChanged ()
{
	mContentChanged = true;
	emit contentChanged (true);
}

//=============================================================================
// Clear all controls.
//=============================================================================
void CGeneral::clearControls()
{
  disconnectSlots();

  mCategory->setCurrentItem(0);
  mName->clear();
  mFirst->clear();
  mSuffix->clear();
  mTitle->clear();
  mBirthday->clear();
  mGender->setCurrentItem(0);
  mCompany->clear();
  mProfession->clear();
  mComment->clear();
  mPhoto->setText(tr("Photo"));
  mName->setFocus();

  connectSlots();
}

//=============================================================================
// Make all controls readonly.
//=============================================================================
void CGeneral::setReadonly (bool readonly)
{
  mCategory->setEnabled    (!readonly);
  mName->setReadOnly       (readonly);
  mPhoto->setReadOnly      (readonly);
  mFirst->setReadOnly      (readonly);
  mSuffix->setReadOnly     (readonly);
  mTitle->setReadOnly      (readonly);
  mBirthday->setReadOnly   (readonly);
  mGender->setEnabled      (!readonly);
  mCompany->setReadOnly    (readonly);
  mProfession->setReadOnly (readonly);
  mComment->setReadOnly    (readonly);
}

//=============================================================================
// Connect the slots to the signals.
//=============================================================================
void CGeneral::connectSlots()
{
  connect (mCategory,   SIGNAL (activated   	   (const QString &)),
           this,     	SLOT   (slotContentChanged (const QString &)));
  connect (mPhoto,      SIGNAL (contentChanged     ()),
           this,        SLOT   (slotContentChanged ()));
  connect (mName,       SIGNAL (textChanged        (const QString &)),
           this,        SLOT   (slotContentChanged (const QString &)));
  connect (mFirst,      SIGNAL (textChanged 	   (const QString &)),
           this,        SLOT   (slotContentChanged (const QString &)));
  connect (mSuffix,     SIGNAL (textChanged 	   (const QString &)),
           this,     	SLOT   (slotContentChanged (const QString &)));
  connect (mTitle,      SIGNAL (textChanged 	   (const QString &)),
           this,     	SLOT   (slotContentChanged (const QString &)));
  connect (mBirthday,   SIGNAL (textChanged 	   (const QString &)),
           this,     	SLOT   (slotContentChanged (const QString &)));
  connect (mGender,     SIGNAL (activated   	   (const QString &)),
           this,     	SLOT   (slotContentChanged (const QString &)));
  connect (mCompany,	SIGNAL (textChanged 	   (const QString &)),
           this,     	SLOT   (slotContentChanged (const QString &)));
  connect (mProfession, SIGNAL (textChanged 	   (const QString &)),
           this,     	SLOT   (slotContentChanged (const QString &)));
  connect (mComment,    SIGNAL (textChanged 	   ()),
           this,     	SLOT   (slotContentChanged ()));
}

//=============================================================================
// Disconnect the slots from the signals.
//=============================================================================
void CGeneral::disconnectSlots()
{
  mCategory->disconnect   ();
  mName->disconnect       ();
  mFirst->disconnect      ();
  mSuffix->disconnect     ();
  mTitle->disconnect      ();
  mBirthday->disconnect   ();
  mGender->disconnect     ();
  mCompany->disconnect    ();
  mProfession->disconnect ();
  mComment->disconnect    ();
}
