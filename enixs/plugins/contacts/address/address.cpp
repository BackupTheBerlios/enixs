//#############################################################################
//#  address.cpp
//#  ===========
//#
//#  Creation:       Sun Jun 02 23:45:16 CEST 2002
//#  Author:         Ralf Schneider  (ralf.schneider@b-connected.de)
//#  Copyright:      (C) 2002 by B-connected Linux Solutions GmbH
//#
//#  ------------------------------------------------------------------------
//#
//#  This file contains the implementation of the class CAddress.
//#  This class contains all the widgets for displaying the address
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
#include "address.h"
#include "util.h"

//=============================================================================
// Constructor of the class CAddress.
//=============================================================================
CAddress::CAddress (QWidget *parent, const char *name, QSqlDatabase *db,
                    CUserData *current)
	: QWidget (parent,name)
{
  mContentChanged = false;
  mNew            = false;
  mDB             = db;
  mCurrentUser    = current;
  mMore           = 0;
  
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
// Destructor of the class CAddress.
//=============================================================================
CAddress::~CAddress()
{
  FREE_PTR(mLayout);
}

//=============================================================================
// Load the detail address information of the person.
//=============================================================================
void CAddress::loadData (QString id, bool readonly)
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
  // Load the data.
  //----------------------------------------------------------------------------
  QSqlQuery query ("SELECT address_id, type, street, zip, city, state, country, "
                          "created, last_modified "
                   "FROM   contacts_addresses "
                   "WHERE  person_id = " + id + " ORDER BY type");

  if (!query.isActive())
  {
    SHOW_DB_ERROR(tr ("Error during database query"), query);
    return;
  }

  while (query.next())
  {
    if (row >= mAddress.count())
    {
      slotAddLine();
      disconnectSlots();
    }
    
    mIDs << query.value(0).toString();
    mAddress.at(row)->setID      (query.value(0).toString());
    mAddress.at(row)->setType    (query.value(1).toInt());
    mAddress.at(row)->setStreet  (query.value(2).toString());
    mAddress.at(row)->setZip     (query.value(3).toString());
    mAddress.at(row)->setCity    (query.value(4).toString());
    mAddress.at(row)->setState   (query.value(5).toString());
    mAddress.at(row)->setCountry (query.value(6).toString());
    mAddress.at(row)->setToolTip (tr ("Created:\t\t\t") + 
                                  formatDateTime (query.value(7).toString()) +
                                  tr ("\nLast Modified:\t\t") + 
                                  formatDateTime (query.value(8).toString()));
    mAddress.at(row)->setChanged (false);
    
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
void CAddress::deleteData (QString id)
{
  //----------------------------------------------------------------------------
  // Delete communication data of the person.
  //----------------------------------------------------------------------------
  QSqlQuery query ("DELETE FROM contacts_addresses WHERE person_id = " + id);

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
QString CAddress::saveChanges ()
{
  QString 	      intro, firstValue, where, newid;
  unsigned int    row;

  //----------------------------------------------------------------------------
  // If nothing was changed, nothing has to be done.
  //----------------------------------------------------------------------------
  if (!mContentChanged)
    return mCurrent;
  
  for (row = 0; row < mAddress.count(); row++)
  {
    if (!mAddress.at(row)->hasChanged())
      continue;
    
    //--------------------------------------------------------------------------
    // Build the SQL statement.
    //--------------------------------------------------------------------------
    if (row < mIDs.count())
    {
      intro      = "UPDATE contacts_addresses (";
      firstValue = "";
      where      = "WHERE address_id = " + mIDs[row];
    }
    else
    {
      newid      = getNextID();
      mIDs      << newid;
      
      intro      = "INSERT contacts_addresses (address_id, ";
      firstValue = newid + ", ";
      where      = "";
    }
    
    //--------------------------------------------------------------------------
    // Execute the SQL statement.
    //--------------------------------------------------------------------------
    QSqlQuery query (intro + "person_id, type, street, zip, city, state, country, "
                     "last_modified) VALUES (" + firstValue + mCurrent +  ", " + 
                     QString::number (mAddress.at(row)->type()) + ", '" + 
                     mAddress.at(row)->street() + "', '" +
                     mAddress.at(row)->zip() + "', '" + 
                     mAddress.at(row)->city() + "', '" + 
                     mAddress.at(row)->state() + "', '" + 
                     mAddress.at(row)->country() + "', TIMESTAMP) " + where);
    
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
// Determine the next available address ID for a person.
//=============================================================================
QString CAddress::getNextID ()
{
  int  id;

  //----------------------------------------------------------------------------
  // Read the maximum comm ID.
  //----------------------------------------------------------------------------
  QSqlQuery query ("SELECT max(address_id) FROM contacts_addresses");
  
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
void CAddress::slotContentChanged (const QString &text)
{ 
  mContentChanged = true;
  emit contentChanged (true);
}

//=============================================================================
// SLOT: Notice that the content has changed.
//=============================================================================
void CAddress::slotContentChanged ()
{
  mContentChanged = true;
  emit contentChanged (true);
}

//=============================================================================
// SLOT: Add another line to the grid.
//=============================================================================
void CAddress::slotAddLine ()
{
  CAddressBox *box;
  int          row;

  disconnectSlots();
  
  //----------------------------------------------------------------------------
  // Delete the "more" button.
  //----------------------------------------------------------------------------
  FREE_PTR(mMore);
  
  //----------------------------------------------------------------------------
  // Create a new combobox and the edit fields  and add them to the grid layout.
  //----------------------------------------------------------------------------
  row = mAddress.count();
  
  box = new CAddressBox (this);

  mLayout->addWidget (box, row, 0);
    
  mAddress.append (box);
  
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
void CAddress::slotDeleteLine (QString id)
{
  //----------------------------------------------------------------------------
  // If ID is an empty string, the line was not saved already, so nothing has
  // to bo done.
  //----------------------------------------------------------------------------
  if (id != "")
  {
    QSqlQuery query ("DELETE FROM contacts_addresses WHERE address_id = " + id);

    if (!query.isActive())
    {
      SHOW_DB_ERROR(tr ("Error during deleting the data"), query);
      return;
    }

    mDB->commit();
  }
  
  loadData (mCurrent, mReadonly);
}

//=============================================================================
// Remove all lines.
//=============================================================================
void CAddress::removeAllLines ()
{
  while (mAddress.count() > 0)
  {
    delete mAddress.at(0);
    mAddress.remove   ((unsigned int)0);
  }
  
  mIDs.clear();
}

//=============================================================================
// Clear all controls.
//=============================================================================
void CAddress::clearControls()
{
  unsigned int    row;
  
  disconnectSlots();

  for (row = 0; row < mAddress.count(); row++)
  {
    mAddress.at(row)->clear();
    mAddress.at(row)->setToolTip ("");
  }

  mIDs.clear();
  
  connectSlots();
}

//=============================================================================
// Make all controls readonly.
//=============================================================================
void CAddress::setReadonly (bool readonly)
{
  unsigned int    row;
  
  for (row = 0; row < mAddress.count(); row++)
    mAddress.at(row)->setReadOnly (readonly);

  mReadonly = readonly;
}

//=============================================================================
// Connect the slots to the signals.
//=============================================================================
void CAddress::connectSlots()
{
  unsigned int    row;
  
  for (row = 0; row < mAddress.count(); row++)
  {
    connect (mAddress.at(row), SIGNAL (contentChanged     ()),
             this,     	       SLOT   (slotContentChanged ()));
    connect (mAddress.at(row), SIGNAL (lineDeleted        (QString)), 
             this,             SLOT   (slotDeleteLine     (QString)));
  }
}

//=============================================================================
// Disconnect the slots from the signals.
//=============================================================================
void CAddress::disconnectSlots()
{
  unsigned int    row;
  
  for (row = 0; row < mAddress.count(); row++)
    mAddress.at(row)->disconnect();
}
