//#############################################################################
//#  addressbox.cpp
//#  ==============
//#
//#  Creation:       Tue Jun 04 00:55:16 CEST 2002
//#  Author:         Ralf Schneider  (ralf.schneider@b-connected.de)
//#  Copyright:      (C) 2002 by B-connected Linux Solutions GmbH
//#
//#  ------------------------------------------------------------------------
//#
//#  This file contains the implementation of the class CAddressBox.
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

//=============================================================================
// Include files for QT.
//=============================================================================
#include <qpixmap.h>
#include <qtooltip.h>

//=============================================================================
// Application specific includes.
//=============================================================================
#include "addressbox.h"
#include "util.h"
#include "../bitmaps/delete.xpm"

#include "../bitmaps/office.xpm"
#include "../bitmaps/home.xpm"
#include "../bitmaps/others.xpm"

//=============================================================================
// Constructor of the class CAddressBox.
//=============================================================================
CAddressBox::CAddressBox (QWidget *parent, const char *name)
	: QWidget (parent,name)
{
  QLabel  *label1, *label2, *label3, *label4;
  QPixmap *icon = new QPixmap (delete_icon);
  
  //----------------------------------------------------------------------------
  //  Create the grid layout
  //----------------------------------------------------------------------------
  mLayout = new QGridLayout (this, 4, 4, 0, 10);
  mLayout->setColStretch    (0, 3);
  mLayout->setColStretch    (1, 3);
  mLayout->setColStretch    (2, 1);
  mLayout->setColStretch    (3, 5);
  mLayout->setColStretch    (4, 5);
//  setSizePolicy (QSizePolicy (QSizePolicy::Preferred, QSizePolicy::Fixed));
  
  //----------------------------------------------------------------------------
  //  Create combobox.
  //----------------------------------------------------------------------------
  mType  = new QComboBox (this);
  mType->insertItem      (QPixmap (office_icon), tr("Office"));
  mType->insertItem      (QPixmap (home_icon),   tr("Home"));
  mType->insertItem      (QPixmap (others_icon), tr("Others"));
  mLayout->addWidget     (mType, 0, 0);

  //----------------------------------------------------------------------------
  //  Create the labels.
  //----------------------------------------------------------------------------
  label1 = new QLabel (tr("Street"),  this);
  label2 = new QLabel (tr("City"),    this);
  label3 = new QLabel (tr("State"),   this);
  label4 = new QLabel (tr("Country"), this);

  mLayout->addWidget  (label1, 0, 1);
  mLayout->addWidget  (label2, 1, 1);
  mLayout->addWidget  (label3, 2, 1);
  mLayout->addWidget  (label4, 3, 1);

  //----------------------------------------------------------------------------
  //  Create the lineedits.
  //----------------------------------------------------------------------------
  mStreet  = new QLineEdit (this);
  mZip     = new QLineEdit (this);
  mCity    = new QLineEdit (this);
  mState   = new QLineEdit (this);
  mCountry = new QLineEdit (this);

  //----------------------------------------------------------------------------
  //  Create the pushbutton.
  //----------------------------------------------------------------------------
  mDelete = new QPushButton (this);
  mDelete->setPixmap        (*icon);
  mDelete->setFixedSize     (24,24);

  QToolTip::add (mDelete, tr("Delete this address"));
  connect       (mDelete, SIGNAL(clicked()), this, SLOT(slotDeleteLine()));

  //----------------------------------------------------------------------------
  //  Add the lineedits and the pushbutton to the layout.
  //----------------------------------------------------------------------------
  mLayout->addMultiCellWidget (mStreet,  0, 0, 2, 4);
  mLayout->addMultiCellWidget (mZip,     1, 1, 2, 2);
  mLayout->addMultiCellWidget (mCity,    1, 1, 3, 4);
  mLayout->addMultiCellWidget (mState,   2, 2, 2, 4);
  mLayout->addMultiCellWidget (mCountry, 3, 3, 2, 4);
  mLayout->addWidget          (mDelete,  0, 5);
  
  connectSlots();
}

//=============================================================================
// Destructor of the class CAddressBox.
//=============================================================================
CAddressBox::~CAddressBox()
{
  FREE_PTR(mType);
  FREE_PTR(mStreet);
  FREE_PTR(mZip);
  FREE_PTR(mCity);
  FREE_PTR(mState);
  FREE_PTR(mCountry);
  FREE_PTR(mDelete);
  FREE_PTR(mLayout);
}

//=============================================================================
// Set the tooltip text for the widget.
//=============================================================================
void CAddressBox::setToolTip (QString tip)
{
  QToolTip::add (mType,    tip);
  QToolTip::add (mStreet,  tip);
  QToolTip::add (mZip,     tip);
  QToolTip::add (mCity,    tip);
  QToolTip::add (mState,   tip);
  QToolTip::add (mCountry, tip);
}

//=============================================================================
// Clear the edit line and set the first combobox item.
//=============================================================================
void CAddressBox::clear ()
{
  disconnectSlots();
  
  mType->setCurrentItem (0);
  mStreet->clear();
  mZip->clear();
  mCity->clear();
  mState->clear();
  mCountry->clear();

  connectSlots();
}

//=============================================================================
// Make the widget readonly.
//=============================================================================
void CAddressBox::setReadOnly (bool readonly)
{
  mType->setEnabled     (!readonly);
  mStreet->setReadOnly  (readonly);
  mZip->setReadOnly     (readonly);
  mCity->setReadOnly    (readonly);
  mState->setReadOnly   (readonly);
  mCountry->setReadOnly (readonly);
  mDelete->setEnabled   (!readonly);
}

//=============================================================================
// SLOT: Notice that the content has changed.
//=============================================================================
void CAddressBox::slotContentChanged (const QString &text)
{ 
  mContentChanged = true;
  emit contentChanged ();
}

//=============================================================================
// SLOT: Notice that the content has changed.
//=============================================================================
void CAddressBox::slotContentChanged ()
{
  mContentChanged = true;
  emit contentChanged ();
}

//=============================================================================
// SLOT: Delete this line.
//=============================================================================
void CAddressBox::slotDeleteLine ()
{
  int ret = QMessageBox::information (this, tr ("Delete Address"),
                                  tr ("Do you really want to delete this address?"),
                                  QMessageBox::Yes | QMessageBox::Default, 
                                  QMessageBox::No);

  if (ret == QMessageBox::Yes)
    emit lineDeleted (mID);
}

//=============================================================================
// Connect the slots to the signals.
//=============================================================================
void CAddressBox::connectSlots()
{
  connect (mType,    SIGNAL (activated   	    (const QString &)),
           this,     SLOT   (slotContentChanged (const QString &)));
  connect (mStreet,  SIGNAL (textChanged        (const QString &)),
           this,     SLOT   (slotContentChanged (const QString &)));
  connect (mZip,     SIGNAL (textChanged        (const QString &)),
           this,     SLOT   (slotContentChanged (const QString &)));
  connect (mCity,    SIGNAL (textChanged        (const QString &)),
           this,     SLOT   (slotContentChanged (const QString &)));
  connect (mState,   SIGNAL (textChanged        (const QString &)),
           this,     SLOT   (slotContentChanged (const QString &)));
  connect (mCountry, SIGNAL (textChanged        (const QString &)),
           this,     SLOT   (slotContentChanged (const QString &)));
}

//=============================================================================
// Disconnect the slots from the signals.
//=============================================================================
void CAddressBox::disconnectSlots()
{
  mType->disconnect();
  mStreet->disconnect();
  mZip->disconnect();
  mCity->disconnect();
  mState->disconnect();
  mCountry->disconnect();
}
