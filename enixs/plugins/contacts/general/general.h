//#############################################################################
//#  general.h
//#  =========
//#
//#  Creation:       Fri May 17 20:15:16 CEST 2002
//#  Author:         Ralf Schneider  (ralf.schneider@b-connected.de)
//#  Copyright:      (C) 2002 by B-connected Linux Solutions GmbH
//#
//#  ------------------------------------------------------------------------
//#
//#  This file contains the definition of the class CGeneral.
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

#ifndef GENERAL_H
#define GENERAL_H

//=============================================================================
// Include files for QT.
//=============================================================================
#include <qlabel.h>
#include <qlayout.h>
#include <qlistview.h>
#include <qcombobox.h>
#include <qlineedit.h>
#include <qsqldatabase.h>
#include <qasciidict.h>

//=============================================================================
// Application specific includes.
//=============================================================================
#include "userdata.h"
#include "textedit.h"
#include "../widgets/photo.h"


class CGeneral : public QWidget
{
  Q_OBJECT

public: 
  CGeneral (QWidget *parent=0, const char *name=0, QSqlDatabase *db=0,
            CUserData *current=0);
  ~CGeneral();

  void 		loadData      (QString id, bool readonly);
  void 		deleteData    (QString id);
  void 		newData       () { mNew = true; clearControls(); slotContentChanged(); }
  bool 		hasChanged    () { return mContentChanged; }
  void 		setChanged    (bool flag) { mContentChanged = flag; }
  QString   saveChanges   ();
  void      clearControls ();

signals:
  void contentChanged  	  (bool);
  void contentLoaded   	  (bool);
  void treeDataChanged 	  ();
  void saveModifications  ();

protected:
  QString getNextID       ();
  void    connectSlots    ();
  void    disconnectSlots ();
  void    setReadonly     (bool readonly);
  
protected slots:
  void slotContentChanged (const QString &text);
  void slotContentChanged ();
  
private:
  QGridLayout*  mGrid;
  QLabel*		mCategoryL;
  QLabel*		mNameL;
  QLabel*		mFirstL;
  QLabel*		mSuffixL;
  QLabel*		mTitleL;
  QLabel*		mBirthdayL;
  QLabel*		mGenderL;
  QLabel*		mCompanyL;
  QLabel*		mProfessionL;
  QLabel*		mCommentL;

  CPhoto*   	mPhoto;
  QComboBox*	mCategory;
  QLineEdit*	mName;
  QLineEdit*	mFirst;
  QLineEdit*	mSuffix;
  QLineEdit*	mTitle;
  QLineEdit*	mBirthday;
  QComboBox*	mGender;
  QLineEdit*	mCompany;
  QLineEdit*	mProfession;
  CTextEdit*	mComment;

  QSqlDatabase* mDB;
  CUserData*    mCurrentUser;
  QString       mCurrent;
  bool			mContentChanged;
  bool			mReadonly;
  bool			mNew;

  QAsciiDict<QString> mCategories;
};

#endif
