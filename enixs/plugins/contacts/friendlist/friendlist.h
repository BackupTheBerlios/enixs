//#############################################################################
//#  friendlist.h
//#  ============
//#
//#  Creation:       Thu Jun 13 23:55:16 CEST 2002
//#  Author:         Ralf Schneider  (ralf.schneider@b-connected.de)
//#  Copyright:      (C) 2002 by B-connected Linux Solutions GmbH
//#
//#  ------------------------------------------------------------------------
//#
//#  This file contains the definition of the class CFriendList.
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

#ifndef FRIENDLIST_H
#define FRIENDLIST_H

#include <qdialog.h>
#include <qlayout.h>
#include <qlineedit.h>
#include <qlistview.h>
#include <qsqldatabase.h>


class CFriendList : public QDialog
{
Q_OBJECT

public:
 CFriendList (QWidget *parent = 0, const char *name=0, bool singleSelect=true,
              QSqlDatabase* db=0);
 ~CFriendList();
 
 QPtrList<QListViewItem>    selection();

private:
  QGridLayout*  mLayout;
  QListView*    mList;
};

#endif
