//#############################################################################
//#  commline.h
//#  ==========
//#
//#  Creation:       Mon Jun 03 14:35:16 CEST 2002
//#  Author:         Ralf Schneider  (ralf.schneider@b-connected.de)
//#  Copyright:      (C) 2002 by B-connected Linux Solutions GmbH
//#
//#  ------------------------------------------------------------------------
//#
//#  This file contains the definition of the class CCommLine.
//#  This class builds a new widget consisting of a combobox and an editline
//#  for displaying one communication item.
//#
//#  ------------------------------------------------------------------------
//#
//#  This program is free software; you can redistribute it and/or modify it
//#  under the terms of the GNU General Public License as published by the
//#  Free Software Foundation; either version 2 of the License, or (at your
//#  option) any later version.
//#
//#############################################################################

#ifndef COMMLINE_H
#define COMMLINE_H

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


class CCommLine : public QWidget
{
  Q_OBJECT

public: 
  CCommLine (QWidget *parent=0, const char *name=0);
  ~CCommLine();

  void    setID        (QString id)    { mID = id; }
  void    setType      (int type)      { mType->setCurrentItem (type); }
  void    setValue     (QString value) { mValue->setText (value); }
  void    setToolTip   (QString tip);
  void    clear        ();
  void    setReadOnly  (bool readonly);
  
  int     type         () { return mType->currentItem(); }
  QString typeText     () { return mType->currentText(); }
  QString value        () { return mValue->text(); }

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
  QHBoxLayout*  mLayout;

  QString       mID;
  QComboBox*    mType;
  QLineEdit*    mValue;
  QPushButton*  mDelete;
  
  bool			mContentChanged;
};

#endif
