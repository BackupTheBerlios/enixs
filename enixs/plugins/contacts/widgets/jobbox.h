//#############################################################################
//#  jobbox.h
//#  ========
//#
//#  Creation:       Thu Jun 06 09:18:16 CEST 2002
//#  Author:         Ralf Schneider  (ralf.schneider@b-connected.de)
//#  Copyright:      (C) 2002 by B-connected Linux Solutions GmbH
//#
//#  ------------------------------------------------------------------------
//#
//#  This file contains the definition of the class CJobBox.
//#  This class builds a new widget consisting of lineedit widgets for
//#  for displaying the different parts of an job item.
//#
//#  ------------------------------------------------------------------------
//#
//#  This program is free software; you can redistribute it and/or modify it
//#  under the terms of the GNU General Public License as published by the
//#  Free Software Foundation; either version 2 of the License, or (at your
//#  option) any later version.
//#
//#############################################################################

#ifndef JOBBOX_H
#define JOBBOX_H

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


class CJobBox : public QWidget
{
  Q_OBJECT

public: 
  CJobBox (QWidget *parent=0, const char *name=0);
  ~CJobBox();

  void    setID         (QString id)        { mID = id; }
  void    setJob        (QString job)       { mJob->setCurrentText (job); }
  void    setJobTypes   (QStringList types) { mJob->insertStringList (types); }
  void    setCompany    (QString company)   { mCompany->setText (company); }
  void    setDepartment (QString department){ mDepartment->setText (department); }
  void    setManager    (QString manager)   { mManager->setText (manager); }
  void    setAssistant  (QString assistant) { mAssistant->setText (assistant); }
  void    setToolTip    (QString tip);
  void    clear         ();
  void    setReadOnly   (bool readonly);
  
  QString job          () { return mJob->currentText(); }
  QString company      () { return mCompany->text(); }
  QString department   () { return mDepartment->text(); }
  QString manager      () { return mManager->text(); }
  QString assistant    () { return mAssistant->text(); }

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
  QComboBox*    mJob;
  QLineEdit*    mCompany;
  QLineEdit*    mDepartment;
  QLineEdit*    mManager;
  QLineEdit*    mAssistant;
  QPushButton*  mDelete;
  
  bool			mContentChanged;
};

#endif
