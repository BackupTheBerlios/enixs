//#############################################################################
//#  communication.cpp
//#  =================
//#
//#  Creation:       Wed May 29 01:15:16 CEST 2002
//#  Author:         Ralf Schneider  (ralf.schneider@b-connected.de)
//#  Copyright:      (C) 2002 by B-connected Linux Solutions GmbH
//#
//#  ------------------------------------------------------------------------
//#
//#  This file contains the implementation of the class CCommunicaion.
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

//=============================================================================
// Include files for QT.
//=============================================================================
#include <qpixmap.h>
#include <qtooltip.h>

//=============================================================================
// Application specific includes.
//=============================================================================
#include "communication.h"
#include "util.h"

//=============================================================================
// Constructor of the class CCommunication.
//=============================================================================
CCommunication::CCommunication (QWidget *parent, const char *name, QSqlDatabase *db,
                                CUserData *current)
	: QWidget (parent,name)
{
  int   i;
  
  mContentChanged = false;
  mNew            = false;
  mDB             = db;
  mCurrentUser    = current;

  //----------------------------------------------------------------------------
  //  Create the layout
  //----------------------------------------------------------------------------
  mLayout = new QGridLayout (this, 10, 2, 10);

  //----------------------------------------------------------------------------
  //  Create four default rows consisting of combobox and lineedit.
  //----------------------------------------------------------------------------
  for (i = 0; i < 4; i++)
    slotAddLine();
  
  setReadonly (true);
  connectSlots();
}

//=============================================================================
// Destructor of the class CCommunication.
//=============================================================================
CCommunication::~CCommunication()
{
  FREE_PTR(mLayout);
}

//=============================================================================
// Load the detail communication information of the person.
//=============================================================================
void CCommunication::loadData (QString id, bool readonly)
{
  QString 			DBnull = "?", tip;
  unsigned int      row = 0;
  
  //----------------------------------------------------------------------------
  // Disconnect the slots during data load.
  //----------------------------------------------------------------------------
  removeAllLines();
  slotAddLine();
  disconnectSlots();

  //----------------------------------------------------------------------------
  // Load the communication types.
  //----------------------------------------------------------------------------
  QSqlQuery query ("SELECT comm_id, type, number, created, last_modified "
                   "FROM   contacts_communications "
                   "WHERE  person_id = " + id + " ORDER BY type");

  if (!query.isActive())
  {
    SHOW_DB_ERROR(tr ("Error during database query"), query);
    return;
  }

  while (query.next())
  {
    if (row >= mLine.count())
    {
      slotAddLine();
      disconnectSlots();
    }
    
    mIDs << query.value(0).toString();
    mLine.at(row)->setID      (query.value(0).toString());
    mLine.at(row)->setType    (query.value(1).toInt());
    mLine.at(row)->setValue   (query.value(2).toString());
    mLine.at(row)->setToolTip (tr ("Created:\t\t\t") + 
                               formatDateTime (query.value(3).toString()) +
                               tr ("\nLast Modified:\t\t") + 
                               formatDateTime (query.value(4).toString()));
    mLine.at(row)->setChanged (false);
    
    row++;
  } 
  
  //----------------------------------------------------------------------------
  // Store the ID of the currently loaded person.
  //----------------------------------------------------------------------------
  mCurrent  = id;
  mReadonly = readonly;

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
void CCommunication::deleteData (QString id)
{
  //----------------------------------------------------------------------------
  // Delete communication data of the person.
  //----------------------------------------------------------------------------
  QSqlQuery query ("DELETE FROM contacts_communications WHERE person_id = " + id);

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
QString CCommunication::saveChanges ()
{
  QString 	      intro, firstValue, where, newid;
  unsigned int    row;

  //----------------------------------------------------------------------------
  // If nothing was changed, nothing has to be done.
  //----------------------------------------------------------------------------
  if (!mContentChanged)
    return mCurrent;
  
  for (row = 0; row < mLine.count(); row++)
  {
    if (!mLine.at(row)->hasChanged())
      continue;
    
    //--------------------------------------------------------------------------
    // Build the SQL statement.
    //--------------------------------------------------------------------------
    if (row < mIDs.count())
    {
      intro      = "UPDATE contacts_communications (";
      firstValue = "";
      where      = "WHERE comm_id = " + mIDs[row];
    }
    else
    {
      newid      = getNextID();
      mIDs      << newid;
      
      intro      = "INSERT contacts_communications (comm_id, ";
      firstValue = newid + ", ";
      where      = "";
    }
    
    //--------------------------------------------------------------------------
    // Execute the SQL statement.
    //--------------------------------------------------------------------------
    QSqlQuery query (intro + "person_id, type, number, last_modified) VALUES (" + 
                     firstValue + mCurrent +  ", " + 
                     QString::number (mLine.at(row)->type()) + ", '" + 
                     mLine.at(row)->value() + "', TIMESTAMP) " + where);
    
    if (!query.isActive())
    {
      SHOW_DB_ERROR(tr ("Error during writing of data"), query);
      return "";
    }

  }

  mContentChanged = false;
  mNew            = false;

  return mCurrent;
}

//=============================================================================
// Determine the next available person ID for a person.
//=============================================================================
QString CCommunication::getNextID ()
{
  int   	  id;

  //----------------------------------------------------------------------------
  // Read the maximum comm ID.
  //----------------------------------------------------------------------------
  QSqlQuery query ("SELECT max(comm_id) FROM contacts_communications");
  
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
// SLOT: Notice that the content has changed.
//=============================================================================
void CCommunication::slotContentChanged ()
{
  mContentChanged = true;
  emit contentChanged (true);
}

//=============================================================================
// SLOT: Add another line to the grid.
//=============================================================================
void CCommunication::slotAddLine ()
{
  CCommLine   *line;
  int          row;

  disconnectSlots();
  
  //----------------------------------------------------------------------------
  // Delete the "more" button.
  //----------------------------------------------------------------------------
  FREE_PTR(mMore);
  
  //----------------------------------------------------------------------------
  // Create a new combobox and lineedit and add it to the grid layout.
  //----------------------------------------------------------------------------
  row = mLine.count();
  
  line = new CCommLine (this);
  mLayout->addWidget   (line, row, 0);
    
  mLine.append         (line);
  
  //----------------------------------------------------------------------------
  // Add the "more" button again.
  //----------------------------------------------------------------------------
  mMore = new QPushButton (tr("More"), this);
  mMore->setFixedWidth    (80);
  mLayout->addWidget      (mMore, row + 1, 0, AlignRight);

  connect (mMore, SIGNAL(clicked()), this, SLOT(slotAddLine()));

  line->show();
  mMore->show();

  connectSlots();
}

//=============================================================================
// SLOT: Delete the line with the given ID.
//=============================================================================
void CCommunication::slotDeleteLine (QString id)
{
  //----------------------------------------------------------------------------
  // If ID is an empty string, the line was not saved already, so nothing has
  // to bo done.
  //----------------------------------------------------------------------------
  if (id != "")
  {
    QSqlQuery query ("DELETE FROM contacts_communications WHERE comm_id = " + id);

    if (!query.isActive())
    {
      SHOW_DB_ERROR(tr ("Error during deleting the data"), query);
      return;
    }

    mDB->commit();
  }
  
  //----------------------------------------------------------------------------
  // Reload the data after deleting.
  //----------------------------------------------------------------------------
  loadData (mCurrent, mReadonly);
}

//=============================================================================
// Remove all lines.
//=============================================================================
void CCommunication::removeAllLines ()
{
  while (mLine.count() > 0)
  {
    delete mLine.at(0);
    mLine.remove   ((unsigned int)0);
  }
  
  mIDs.clear();
}

//=============================================================================
// Clear all controls.
//=============================================================================
void CCommunication::clearControls()
{
  unsigned int    row;
  
  disconnectSlots();

  for (row = 0; row < mLine.count(); row++)
    mLine.at(row)->clear();

  mIDs.clear();
  
  connectSlots();
}

//=============================================================================
// Make all controls readonly.
//=============================================================================
void CCommunication::setReadonly (bool readonly)
{
  unsigned int    row;
  
  for (row = 0; row < mLine.count(); row++)
    mLine.at(row)->setReadOnly (readonly);

  mReadonly = readonly;
}

//=============================================================================
// Connect the slots to the signals.
//=============================================================================
void CCommunication::connectSlots()
{
  unsigned int    row;
  
  for (row = 0; row < mLine.count(); row++)
  {
    connect (mLine.at(row), SIGNAL (contentChanged     ()), 
             this,          SLOT   (slotContentChanged ()));
    connect (mLine.at(row), SIGNAL (lineDeleted        (QString)), 
             this,          SLOT   (slotDeleteLine     (QString)));
  }
}

//=============================================================================
// Disconnect the slots from the signals.
//=============================================================================
void CCommunication::disconnectSlots()
{
  unsigned int    row;
  
  for (row = 0; row < mLine.count(); row++)
    mLine.at(row)->disconnect();
}
