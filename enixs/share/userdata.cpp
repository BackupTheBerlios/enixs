//#############################################################################
//#  userdata.cpp
//#  ============
//#
//#  Creation:       Tue May 14 2002
//#  Author:         Ralf Schneider  (ralf.schneider@b-connected.de)
//#  Copyright:      (C) 2002 by B-connected Linux Solutions GmbH
//#
//#  ------------------------------------------------------------------------
//#
//#  This file contains the implementation of the class CUserData which
//#  holds some information about the user currently logged in.
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
#include <qmessagebox.h>

//=============================================================================
// Application specific includes.
//=============================================================================
#include "userdata.h"
#include "util.h"
#include <dbconnection.h>

extern CConnection *mDB;

//=============================================================================
// Constructor of the class CUserData.
//=============================================================================
CUserData::CUserData()
{
  mID      = "";
  mName    = "";
  mRole    = "";
  mComment = "";
  mGroups.clear();
}

//=============================================================================
// Destructor of the class CUserData.
//=============================================================================
CUserData::~CUserData()
{
}

//=============================================================================
// Retrieve the user information for a given user.
//=============================================================================
bool CUserData::getUserData (QString user)
{
  QString 			sql;
  QStringList  		record;

  //----------------------------------------------------------------------------
  // Load the user information.
  //----------------------------------------------------------------------------
  sql = "SELECT A.user_id, B.name, A.comment "
		"FROM   enixs_users A, enixs_roles B "
        "WHERE  A.role_id = B.role_id AND A.name = '" + user + "'";
  
  if (!mDB->executeSQL (sql))
  {
    SHOW_DB_ERROR(tr ("Fehler bei der Datenbankanfrage"), sql);
    return false;
  }

  while (mDB->readResult (record))
  {
    mName    = user;
    mID      = record[0];
    mRole    = record[1];
    mComment = record[2];
  }

  if (mID.isEmpty())
    return false;

  //----------------------------------------------------------------------------
  // Load the group information.
  //----------------------------------------------------------------------------
  sql = "SELECT group_id FROM enixs_users_in_groups WHERE user_id = " + mID;

  if (mDB->executeSQL (sql) == false)
  {
    SHOW_DB_ERROR(tr ("Fehler bei der Datenbankanfrage"), sql);
    return false;
  }

  while (mDB->readResult (record))
    mGroups.append (record[0]);

  return true;
}
