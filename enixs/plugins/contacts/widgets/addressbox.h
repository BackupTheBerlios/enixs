//#############################################################################
//#  addressbox.h
//#  ============
//#
//#  Creation:       Tue Jun 04 00:48:16 CEST 2002
//#  Author:         Ralf Schneider  (ralf.schneider@b-connected.de)
//#  Copyright:      (C) 2002 by B-connected Linux Solutions GmbH
//#
//#  ------------------------------------------------------------------------
//#
//#  This file contains the definition of the class CAddressBox.
//#  This class builds a new widget consisting of lineedit widgets for
//#  for displaying the different parts of an address item.
//#
//#  ------------------------------------------------------------------------
//#
//#  This program is free software; you can redistribute it and/or modify it
//#  under the terms of the GNU General Public License as published by the
//#  Free Software Foundation; either version 2 of the License, or (at your
//#  option) any later version.
//#
//#############################################################################

#ifndef ADDRESSBOX_H
#define ADDRESSBOX_H

//=============================================================================
// Include files for QT.
//=============================================================================
#include <qlabel.h>
#include <qlayout.h>
#include <qcombobox.h>
#include <qlineedit.h>
#include <qpushbutton.h>
#include <qstringlist.h>

//=============================================================================
// Application specific includes.
//=============================================================================


class CAddressBox : public QWidget
{
  Q_OBJECT

public: 
  CAddressBox (QWidget *parent=0, const char *name=0);
  ~CAddressBox();

  void    setID           (QString id)        { mID = id; }
  void    setType         (int type)          { mType->setCurrentItem (type); }
  void    setStreet       (QString street)    { mStreet->setText (street); }
  void    setZip          (QString zip)       { mZip->setText (zip); }
  void    setCity         (QString city)      { mCity->setText (city); }
  void    setState        (QString state)     { mState->setText (state); }
  void    setCountry      (QString country)   { mCountry->setText (country); }
  void    setToolTip      (QString tip);
  void    clear           ();
  void    setReadOnly     (bool readonly);
  
  int     type         () { return mType->currentItem(); }
  QString typeText     () { return mType->currentText(); }
  QString street       () { return mStreet->text(); }
  QString zip          () { return mZip->text(); }
  QString city         () { return mCity->text(); }
  QString state        () { return mState->text(); }
  QString country      () { return mCountry->text(); }

  bool    hasChanged   ()          { return mContentChanged; }
  void    setChanged   (bool flag) { mContentChanged = flag; }
  
signals:
  void    contentChanged  	 ();
  void    lineDeleted        (QString id);

protected:
  void    connectSlots       ();
  void    disconnectSlots    ();
  
protected slots:
  void    slotContentChanged (const QString &text);
  void    slotContentChanged ();
  void    slotDeleteLine     ();
  
private:
  QGridLayout*  mLayout;

  QString       mID;
  QComboBox*    mType;
  QLineEdit*    mStreet;
  QLineEdit*    mZip;
  QLineEdit*    mCity;
  QLineEdit*    mState;
  QLineEdit*    mCountry;
  QPushButton*  mDelete;
  
  bool			mContentChanged;
};

#endif
