//#############################################################################
//#  communication.h
//#  ===============
//#
//#  Creation:       Wed May 29 01:15:16 CEST 2002
//#  Author:         Ralf Schneider  (ralf.schneider@b-connected.de)
//#  Copyright:      (C) 2002 by B-connected Linux Solutions GmbH
//#
//#  ------------------------------------------------------------------------
//#
//#  This file contains the definition of the class CCommunicaion.
//#  This class contains all the widgets for displaying the communication
//#  details of a contact.
//#
//#  ------------------------------------------------------------------------
//#
//#  This program is free software; you can redistribute it and/or modify it
//#  under the terms of the GNU General Public License as published by the
//#  Free Software Foundation; either version 2 of the License, or (at your
//#  option) any later version.
//#
//#############################################################################

#ifndef COMMUNICATION_H
#define COMMUNICATION_H

//=============================================================================
// Include files for QT.
//=============================================================================
#include <qlabel.h>
#include <qlayout.h>
#include <qlistview.h>
#include <qpushbutton.h>

//=============================================================================
// Application specific includes.
//=============================================================================
#include "../widgets/commline.h"
#include "userdata.h"
#include <dbconnection.h>


class CCommunication : public QWidget
{
  Q_OBJECT

public: 
  CCommunication (QWidget *parent=0, const char *name=0, CConnection *db=0,
                  CUserData *current=0);
  ~CCommunication();

  void    loadData        (QString id, bool readonly);
  void    deleteData      (QString id);
  void    newData         () { mNew = true; clearControls(); slotContentChanged(); }
  bool    hasChanged      () { return mContentChanged; }
  void    setChanged      (bool flag) { mContentChanged = flag; }
  void    setCurrent      (QString current) { mCurrent = current; }
  QString saveChanges     ();
  void    clearControls   ();
  
signals:
  void    contentChanged  	 (bool);
  void    contentLoaded   	 (bool);
  void    treeDataChanged 	 ();
  void    saveModifications  ();

protected:
  QString getNextID          ();
  void    connectSlots       ();
  void    disconnectSlots    ();
  void    setReadonly        (bool readonly);
  void    removeAllLines     ();
  
protected slots:
  void    slotContentChanged ();
  void    slotAddLine        ();
  void    slotDeleteLine     (QString id);
  
private:
  QGridLayout*  mLayout;

  QPtrList<CCommLine>   mLine;
  QStringList           mIDs;
  QPushButton*          mMore;
  
  CConnection*  mDB;
  CUserData*    mCurrentUser;

  QString       mCurrent;
  bool			mContentChanged;
  bool			mReadonly;
  bool			mNew;
};

#endif
