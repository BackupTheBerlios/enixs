//#############################################################################
//#  commline.cpp
//#  ============
//#
//#  Creation:       Mon Jun 03 01:15:16 CEST 2002
//#  Author:         Ralf Schneider  (ralf.schneider@b-connected.de)
//#  Copyright:      (C) 2002 by B-connected Linux Solutions GmbH
//#
//#  ------------------------------------------------------------------------
//#
//#  This file contains the implementation of the class CCommLine.
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

//=============================================================================
// Include files for QT.
//=============================================================================
#include <qpixmap.h>
#include <qtooltip.h>

//=============================================================================
// Application specific includes.
//=============================================================================
#include "commline.h"
#include "util.h"

#include "../bitmaps/phone.xpm"
#include "../bitmaps/fax.xpm"
#include "../bitmaps/handy.xpm"
#include "../bitmaps/car.xpm"
#include "../bitmaps/mail.xpm"
#include "../bitmaps/homepage.xpm"
#include "../bitmaps/pager.xpm"
#include "../bitmaps/headquarter.xpm"
#include "../bitmaps/assistant.xpm"
#include "../bitmaps/others.xpm"
#include "../bitmaps/delete.xpm"

//=============================================================================
// Constructor of the class CCommLine.
//=============================================================================
CCommLine::CCommLine (QWidget *parent, const char *name)
	: QWidget (parent,name)
{
  QPixmap *icon = new QPixmap (delete_icon);
  
  //----------------------------------------------------------------------------
  //  Create the grid layout
  //----------------------------------------------------------------------------
  mLayout = new QHBoxLayout (this, 0, 10);
  setSizePolicy (QSizePolicy (QSizePolicy::Preferred, QSizePolicy::Fixed));
  
  //----------------------------------------------------------------------------
  //  Create combobox, the lineedit and the pushbutton.
  //----------------------------------------------------------------------------
  mType = new QComboBox (this);
  mType->insertItem     (QPixmap (phone_icon),       tr("Phone Office"));
  mType->insertItem     (QPixmap (phone_icon),       tr("Phone Private"));
  mType->insertItem     (QPixmap (fax_icon),         tr("Fax Office"));
  mType->insertItem     (QPixmap (fax_icon),         tr("Fax Private"));
  mType->insertItem     (QPixmap (handy_icon),       tr("Mobile Phone Office"));
  mType->insertItem     (QPixmap (handy_icon),       tr("Mobile Phone Private"));
  mType->insertItem     (QPixmap (car_icon),         tr("Car Phone Office"));
  mType->insertItem     (QPixmap (car_icon),         tr("Car Phone Private"));
  mType->insertItem     (QPixmap (mail_icon),        tr("E-Mail Office"));
  mType->insertItem     (QPixmap (mail_icon),        tr("E-Mail Private"));
  mType->insertItem     (QPixmap (homepage_icon),    tr("Homepage Office"));
  mType->insertItem     (QPixmap (homepage_icon),    tr("Homepage Private"));
  mType->insertItem     (QPixmap (pager_icon),       tr("Pager"));
  mType->insertItem     (QPixmap (fax_icon),         tr("Telex"));
  mType->insertItem     (QPixmap (headquarter_icon), tr("Headquarter"));
  mType->insertItem     (QPixmap (assistant_icon),   tr("Assistant/Secretary"));
  mType->insertItem     (QPixmap (others_icon),      tr("Others"));
  
  //----------------------------------------------------------------------------
  //  Create combobox, the lineedit and the pushbutton.
  //----------------------------------------------------------------------------
  mValue  = new QLineEdit   (this);

  mDelete = new QPushButton (this);
  mDelete->setPixmap        (*icon);
  mDelete->setFixedSize     (24,24);

  QToolTip::add (mDelete, tr("Delete this line"));
  connect       (mDelete, SIGNAL(clicked()), this, SLOT(slotDeleteLine()));
  
  //----------------------------------------------------------------------------
  //  Add the combobox and the lineedit to the layout.
  //----------------------------------------------------------------------------
  mLayout->addWidget (mType);
  mLayout->addWidget (mValue);
  mLayout->addWidget (mDelete);

  connectSlots();
}

//=============================================================================
// Destructor of the class CCommLine.
//=============================================================================
CCommLine::~CCommLine()
{
  FREE_PTR(mType);
  FREE_PTR(mValue);
  FREE_PTR(mDelete);
  FREE_PTR(mLayout);
}

//=============================================================================
// Set the tooltip text for the widget.
//=============================================================================
void CCommLine::setToolTip (QString tip)
{
  QToolTip::add (mType,  tip);
  QToolTip::add (mValue, tip);
}

//=============================================================================
// Clear the edit line and set the first combobox item.
//=============================================================================
void CCommLine::clear ()
{
  disconnectSlots();
  
  mType->setCurrentItem (0);
  mValue->clear();

  connectSlots();
}

//=============================================================================
// Make the widget readonly.
//=============================================================================
void CCommLine::setReadOnly (bool readonly)
{
  mType->setEnabled   (!readonly);
  mValue->setReadOnly (readonly);
  mDelete->setEnabled (!readonly);
}

//=============================================================================
// SLOT: Notice that the content has changed.
//=============================================================================
void CCommLine::slotContentChanged (const QString &text)
{ 
  mContentChanged = true;
  emit contentChanged ();
}

//=============================================================================
// SLOT: Notice that the content has changed.
//=============================================================================
void CCommLine::slotContentChanged ()
{
  mContentChanged = true;
  emit contentChanged ();
}

//=============================================================================
// SLOT: Delete this line.
//=============================================================================
void CCommLine::slotDeleteLine ()
{
  int ret = QMessageBox::information (this, tr ("Delete Communication"),
                                  tr ("Do you really want to delete this line?"),
                                  QMessageBox::Yes | QMessageBox::Default, 
                                  QMessageBox::No);

  if (ret == QMessageBox::Yes)
    emit lineDeleted (mID);
}

//=============================================================================
// Connect the slots to the signals.
//=============================================================================
void CCommLine::connectSlots()
{
  connect (mType,  SIGNAL (activated   	      (const QString &)),
           this,   SLOT   (slotContentChanged (const QString &)));
  connect (mValue, SIGNAL (textChanged        (const QString &)),
           this,   SLOT   (slotContentChanged (const QString &)));
}

//=============================================================================
// Disconnect the slots from the signals.
//=============================================================================
void CCommLine::disconnectSlots()
{
  mType->disconnect();
  mValue->disconnect();
}
