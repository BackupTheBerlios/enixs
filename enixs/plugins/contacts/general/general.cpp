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

//=============================================================================
// Application specific includes.
//=============================================================================
#include "general.h"
#include "util.h"
#include "../validators/datevalidator.h"

//=============================================================================
// Constructor of the class CGeneral.
//=============================================================================
CGeneral::CGeneral (QWidget *parent, const char *name, CConnection *db,
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
  mPhoto = new QLabel       (tr ("Photo"), this);
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

  loadCategories();
  
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
  QString 			sql, DBnull = "?", filename;
  QStringList		record;
  QPixmap           photo;
  
  //----------------------------------------------------------------------------
  // Disconnect the slots during data load.
  //----------------------------------------------------------------------------
  clearControls();
  disconnectSlots();
  
  //----------------------------------------------------------------------------
  // Load the selected person.
  //----------------------------------------------------------------------------
  sql = "SELECT name, first_name, name_suffix, title, birthday, male, "
        "       company, profession, comment, photo_available "
		"FROM   contacts_persons "
		"WHERE  person_id = " + id;

  if (!mDB->executeSQL (sql))
  {
    SHOW_DB_ERROR(tr ("Error during database query"), sql);
    return;
  }

  mDB->readResult (record);

  //----------------------------------------------------------------------------
  // Set the text of the single and multi line edit controls and the labels.
  //----------------------------------------------------------------------------
  if (record[0] != DBnull)
    mName->setText (record[0]);

  if (record[1] != DBnull)
    mFirst->setText (record[1]);
  
  if (record[2] != DBnull)
    mSuffix->setText (record[2]);
  
  if (record[3] != DBnull)
    mTitle->setText (record[3]);
  
  if (record[4] != DBnull)
    mBirthday->setText (formatDate (record[4]));
  
  if (record[5] == "TRUE")
    mGender->setCurrentItem (0);
  else
    mGender->setCurrentItem (1);
  
  if (record[6] != DBnull)
    mCompany->setText (record[6]);
  
  if (record[7] != DBnull)
    mProfession->setText (record[7]);
  
  if (record[8] != DBnull)
    mComment->setText (record[8]);

  //----------------------------------------------------------------------------
  // Load the photo of the selected person.
  //----------------------------------------------------------------------------
  if (record[9] == "TRUE")
  {
    filename = createTempFilename("png");

    sql = "SELECT photo FROM contacts_persons WHERE person_id = " + id;

    if (mDB->readFile (sql, filename) == false)
    {
      SHOW_DB_ERROR(tr ("Error during database query"), sql);
      return;
    }

    photo.load (filename);
    photo.resize (110, 135);
    mPhoto->setPixmap (photo);
  }
  
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
  QString 	  sql, err;
  QStringList record;
  
  //----------------------------------------------------------------------------
  // Delete the person.
  //----------------------------------------------------------------------------
  sql = "DELETE FROM contacts_persons WHERE person_id = " + id;

  //----------------------------------------------------------------------------
  // Execute the SQL statement.
  //----------------------------------------------------------------------------
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
QString CGeneral::saveChanges ()
{
  QString 	sql, intro, firstValue, gender, birthday;

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
  birthday   = mBirthday->text().isEmpty() ? QString("NULL") : "'" + dateToDB (mBirthday->text()) + "'";

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
  if (!mDB->executeSQL (sql))
  {
    SHOW_DB_ERROR(tr ("Error during writing of data"), sql);
    return "";
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
  int   	  id;
  QString     sql;
  QStringList record;

  //----------------------------------------------------------------------------
  // Read the maximum person ID.
  //----------------------------------------------------------------------------
  sql = "SELECT max(person_id) FROM contacts_persons";
  
  if (!mDB->executeSQL (sql))
  {
    SHOW_DB_ERROR(tr ("Error during database query"), sql);
    return "";
  }

  mDB->readResult (record);

  //----------------------------------------------------------------------------
  // Increment the current max ID and return this value.
  //----------------------------------------------------------------------------
  id = record[0].toInt();
	
  return QString::number (++id);
}

//=============================================================================
// Load the available categories.
//=============================================================================
void CGeneral::loadCategories ()
{
  QString     sql;
  QStringList record;

  //----------------------------------------------------------------------------
  // Read the maximum person ID.
  //----------------------------------------------------------------------------
  sql = "SELECT name FROM enixs_categories WHERE language = 'de' ORDER BY name";
  debug(sql);
  
  if (!mDB->executeSQL (sql))
  {
    SHOW_DB_ERROR(tr ("Error during database query"), sql);
    return;
  }

  while (mDB->readResult (record))
    mCategory->insertItem (record[0]);
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
