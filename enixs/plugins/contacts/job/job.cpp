//#############################################################################
//#  job.cpp
//#  =======
//#
//#  Creation:       Wed Jun 05 00:20:16 CEST 2002
//#  Author:         Ralf Schneider  (ralf.schneider@b-connected.de)
//#  Copyright:      (C) 2002 by B-connected Linux Solutions GmbH
//#
//#  ------------------------------------------------------------------------
//#
//#  This file contains the implementation of the class CJob.
//#  This class contains all the widgets for displaying the job
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
#include "job.h"
#include "util.h"

//=============================================================================
// Constructor of the class CJob.
//=============================================================================
CJob::CJob (QWidget *parent, const char *name, CConnection *db, CUserData *current)
	: QWidget (parent,name)
{
  mContentChanged = false;
  mNew            = false;
  mDB             = db;
  mCurrentUser    = current;

  //----------------------------------------------------------------------------
  //  Set the address types.
  //----------------------------------------------------------------------------
  loadJobTypes();
  
  //----------------------------------------------------------------------------
  //  Create the grid layout
  //----------------------------------------------------------------------------
  mLayout = new QGridLayout (this, 10, 2, 10, 20);

  //----------------------------------------------------------------------------
  //  Create two default address sets.
  //----------------------------------------------------------------------------
  slotAddLine();
  
  setReadonly (true);
  connectSlots();
}

//=============================================================================
// Destructor of the class CJob.
//=============================================================================
CJob::~CJob()
{
  FREE_PTR(mLayout);
}

//=============================================================================
// Load the detail job information of the person.
//=============================================================================
void CJob::loadData (QString id, bool readonly)
{
  QString 			sql, DBnull = "?", tip;
  QStringList		record;
  unsigned int      row = 0;
  
  //----------------------------------------------------------------------------
  // Disconnect the slots during data load.
  //----------------------------------------------------------------------------
  removeAllLines();
  slotAddLine();
  disconnectSlots();

  //----------------------------------------------------------------------------
  // Load the data.
  //----------------------------------------------------------------------------
  sql = "SELECT job_id, company, activity, department, manager, assistant, "
               "created, last_modified "
        "FROM   contacts_jobs "
		"WHERE  person_id = " + id + " ORDER BY company";

  if (!mDB->executeSQL (sql))
  {
    SHOW_DB_ERROR(tr ("Error during database query"), sql);
    return;
  }

  while (mDB->readResult (record))
  {
    if (row >= mJob.count())
    {
      slotAddLine();
      disconnectSlots();
    }
    
    mIDs << record[0];
    mJob.at(row)->setID         (record[0]);
    mJob.at(row)->setCompany    (record[1]);
    mJob.at(row)->setJob        (record[2]);
    mJob.at(row)->setDepartment (record[3]);
    mJob.at(row)->setManager    (record[4]);
    mJob.at(row)->setAssistant  (record[5]);
    mJob.at(row)->setToolTip    (tr ("Created:\t\t\t") + 
                                 formatDateTime (record[6]) +
                                 tr ("\nLast Modified:\t\t") + 
                                 formatDateTime (record[7]));
    mJob.at(row)->setChanged    (false);
    
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
void CJob::deleteData (QString id)
{
  QString 	  sql, err;
  QStringList record;
  
  //----------------------------------------------------------------------------
  // Delete the job data of the person.
  //----------------------------------------------------------------------------
  sql = "DELETE FROM contacts_jobs WHERE person_id = " + id;

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
QString CJob::saveChanges ()
{
  QString 	      sql, intro, firstValue, where, newid;
  unsigned int    row;

  //----------------------------------------------------------------------------
  // If nothing was changed, nothing has to be done.
  //----------------------------------------------------------------------------
  if (!mContentChanged)
    return mCurrent;
  
  for (row = 0; row < mJob.count(); row++)
  {
    if (!mJob.at(row)->hasChanged())
      continue;
    
    //--------------------------------------------------------------------------
    // Build the SQL statement.
    //--------------------------------------------------------------------------
    if (row < mIDs.count())
    {
      intro      = "UPDATE contacts_jobs (";
      firstValue = "";
      where      = "WHERE job_id = " + mIDs[row];
    }
    else
    {
      newid      = getNextID();
      mIDs      << newid;
      
      intro      = "INSERT contacts_jobs (job_id, ";
      firstValue = newid + ", ";
      where      = "";
    }
    
    sql = intro + "person_id, company, activity, department, manager, assistant, "
          "last_modified) VALUES (" + firstValue + mCurrent +  ", '" + 
          mJob.at(row)->company() + "', '" + 
          mJob.at(row)->job() + "', '" +
          mJob.at(row)->department() + "', '" + 
          mJob.at(row)->manager() + "', '" + 
          mJob.at(row)->assistant() + "', TIMESTAMP) " + where;
    debug (sql);
    
    //--------------------------------------------------------------------------
    // Execute the SQL statement.
    //--------------------------------------------------------------------------
    if (!mDB->executeSQL (sql))
    {
      SHOW_DB_ERROR(tr ("Error during writing of data"), sql);
      return "";
    }

  }

  mContentChanged = false;
  mNew            = false;

  return mCurrent;
}

//=============================================================================
// Load all job types allready in the database.
//=============================================================================
void CJob::loadJobTypes()
{
  QString     sql;
  QStringList record;

  sql = "SELECT DISTINCT activity FROM contacts_jobs";
  
  if (!mDB->executeSQL (sql))
  {
    SHOW_DB_ERROR(tr ("Error during database query"), sql);
    return;
  }

  while (mDB->readResult (record))
    mJobTypes << record[0];
}

//=============================================================================
// Determine the next available job ID for a person.
//=============================================================================
QString CJob::getNextID ()
{
  int   	  id;
  QString     sql;
  QStringList record;

  //----------------------------------------------------------------------------
  // Read the maximum job ID.
  //----------------------------------------------------------------------------
  sql = "SELECT max(job_id) FROM contacts_jobs";
  
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
// SLOT: Notice that the content has changed.
//=============================================================================
void CJob::slotContentChanged (const QString &text)
{ 
  mContentChanged = true;
  emit contentChanged (true);
}

//=============================================================================
// SLOT: Notice that the content has changed.
//=============================================================================
void CJob::slotContentChanged ()
{
  mContentChanged = true;
  emit contentChanged (true);
}

//=============================================================================
// SLOT: Add another line to the grid.
//=============================================================================
void CJob::slotAddLine ()
{
  CJobBox *box;
  int      row;

  disconnectSlots();
  
  //----------------------------------------------------------------------------
  // Delete the "more" button.
  //----------------------------------------------------------------------------
  FREE_PTR(mMore);
  
  //----------------------------------------------------------------------------
  // Create a new combobox and the edit fields and add them to the grid layout.
  //----------------------------------------------------------------------------
  row = mJob.count();
  
  box = new CJobBox (this);
  box->setJobTypes  (mJobTypes);

  mLayout->addWidget   (box, row, 0);
    
  mJob.append (box);
  
  //----------------------------------------------------------------------------
  // Add the "more" button again.
  //----------------------------------------------------------------------------
  mMore = new QPushButton (tr("More"), this);
  mMore->setMaximumWidth  (60);
  mLayout->addWidget      (mMore, row + 1, 0, AlignRight);

  connect (mMore, SIGNAL(clicked()), this, SLOT(slotAddLine()));

  box->show();
  mMore->show();

  connectSlots();
}

//=============================================================================
// SLOT: Delete the line with the given ID.
//=============================================================================
void CJob::slotDeleteLine (QString id)
{
  QString   sql;
  
  //----------------------------------------------------------------------------
  // If ID is an empty string, the line was not saved already, so nothing has
  // to bo done.
  //----------------------------------------------------------------------------
  if (id != "")
  {
    sql = "DELETE FROM contacts_jobs WHERE job_id = " + id;

    if (!mDB->executeSQL (sql))
    {
      SHOW_DB_ERROR(tr ("Error during deleting the data"), sql);
      return;
    }

    mDB->commit();
  }
  
  loadData (mCurrent, mReadonly);
}

//=============================================================================
// Remove all lines.
//=============================================================================
void CJob::removeAllLines ()
{
  while (mJob.count() > 0)
  {
    delete mJob.at(0);
    mJob.remove   ((unsigned int)0);
  }
  
  mIDs.clear();
}

//=============================================================================
// Clear all controls.
//=============================================================================
void CJob::clearControls()
{
  unsigned int    row;
  
  disconnectSlots();

  for (row = 0; row < mJob.count(); row++)
  {
    mJob.at(row)->clear();
    mJob.at(row)->setToolTip (tr ("Created:\nLast Modified:"));
  }
  
  mIDs.clear();
  
  connectSlots();
}

//=============================================================================
// Make all controls readonly.
//=============================================================================
void CJob::setReadonly (bool readonly)
{
  unsigned int    row;
  
  for (row = 0; row < mJob.count(); row++)
    mJob.at(row)->setReadOnly (readonly);

  mReadonly = readonly;
}

//=============================================================================
// Connect the slots to the signals.
//=============================================================================
void CJob::connectSlots()
{
  unsigned int    row;
  
  for (row = 0; row < mJob.count(); row++)
  {
    connect (mJob.at(row), SIGNAL (contentChanged     ()),
             this,     	   SLOT   (slotContentChanged ()));
    connect (mJob.at(row), SIGNAL (lineDeleted        (QString)), 
             this,         SLOT   (slotDeleteLine     (QString)));
  }
}

//=============================================================================
// Disconnect the slots from the signals.
//=============================================================================
void CJob::disconnectSlots()
{
  unsigned int    row;
  
  for (row = 0; row < mJob.count(); row++)
    mJob.at(row)->disconnect();
}
