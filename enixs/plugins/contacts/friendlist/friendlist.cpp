//#############################################################################
//#  friendlist.cpp
//#  ==============
//#
//#  Creation:       Thu Jun 13 23:55:16 CEST 2002
//#  Author:         Ralf Schneider  (ralf.schneider@b-connected.de)
//#  Copyright:      (C) 2002 by B-connected Linux Solutions GmbH
//#
//#  ------------------------------------------------------------------------
//#
//#  This file contains the implementation of the class CFriendList.
//#  This class defines the dialog for asking the user a selection of one
//#  or more users or groups.
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
#include <qlabel.h>
#include <qapplication.h>
#include <qheader.h>

//=============================================================================
// Include files for KDE.
//=============================================================================
#include <qpushbutton.h>

//=============================================================================
// Application specific includes.
//=============================================================================
#include "friendlist.h"
#include "util.h"


//=============================================================================
// Constructor of the class CFriendList.
//=============================================================================
CFriendList::CFriendList (QWidget *parent, const char *name, bool singleSelect,
                          CConnection *mDB)
    : QDialog (parent, name, TRUE)
{
  QLabel          *text;
  QPushButton     *ok, *cancel;
  QString          sql;
  QStringList      record;
  QListViewItem   *item;

  setCaption (tr ("Friends"));

  mLayout = new QGridLayout (this, 3, 3, 10, 10);

  text = new QLabel (tr ("Please select one or more users/groups:"), this);

  mList = new QListView (this);
  mList->addColumn      		(tr ("User/Group"), 300);
  mList->addColumn      		(tr ("Type"), 130);
  mList->addColumn      		(tr ("ID"), 40);
  mList->setAllColumnsShowFocus (true);
  mList->setHScrollBarMode      (QListView::AlwaysOff);
  mList->setFixedWidth          (250);
  mList->header()->close();
  
  if (singleSelect)
    mList->setSelectionMode (QListView::Single);
  else
    mList->setSelectionMode (QListView::Multi);

  ok = new QPushButton (tr ("OK"), this);
  ok->setDefault (true);
  connect (ok, SIGNAL (clicked()), SLOT (accept()));

  cancel = new QPushButton (tr ("Cancel"), this);
  connect (cancel, SIGNAL (clicked()), SLOT (reject()));

  mLayout->addMultiCellWidget (text,   0, 0, 0, 2);
  mLayout->addMultiCellWidget (mList,  1, 1, 0, 2);
  mLayout->addWidget          (ok,     2, 0);
  mLayout->addWidget          (cancel, 2, 2);

  //----------------------------------------------------------------------------
  // Load the users/groups.
  //----------------------------------------------------------------------------
  sql = "SELECT name, user_id FROM enixs_users ORDER BY name";

  if (mDB->executeSQL (sql) == false)
  {
    SHOW_DB_ERROR(tr ("Error during database query"), sql)
    return;
  }

  while (mDB->readResult (record))
    item = new QListViewItem (mList, item, record[0], "user", record[1]);

}

//=============================================================================
// Constructor of the class CFriendList.
//=============================================================================
CFriendList::~CFriendList()
{
}

//=============================================================================
// Return a list of the currently selected items in the listview.
//=============================================================================
QPtrList<QListViewItem> CFriendList::selection ()
{
  QPtrList<QListViewItem>   result;
  QListViewItem            *item;
  
  for (item = mList->firstChild(); item; item = item->nextSibling())
    if (item->isSelected())
      result.append (item);
  
  return result;
}

