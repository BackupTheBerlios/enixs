//#############################################################################
//#  jobbox.cpp
//#  ==========
//#
//#  Creation:       Thu Jun 06 09:37:16 CEST 2002
//#  Author:         Ralf Schneider  (ralf.schneider@b-connected.de)
//#  Copyright:      (C) 2002 by B-connected Linux Solutions GmbH
//#
//#  ------------------------------------------------------------------------
//#
//#  This file contains the implementation of the class CJobBox.
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

//=============================================================================
// Include files for QT.
//=============================================================================
#include <qpixmap.h>
#include <qtooltip.h>

//=============================================================================
// Application specific includes.
//=============================================================================
#include "jobbox.h"
#include "util.h"
#include "../bitmaps/delete.xpm"

//=============================================================================
// Constructor of the class CJobBox.
//=============================================================================
CJobBox::CJobBox (QWidget *parent, const char *name)
	: QWidget (parent,name)
{
  QLabel  *label1, *label2, *label3, *label4;
  QPixmap *icon = new QPixmap (delete_icon);
  
  mContentChanged = false;
  
  //----------------------------------------------------------------------------
  //  Create the grid layout
  //----------------------------------------------------------------------------
  mLayout = new QGridLayout (this, 4, 3, 0, 10);
  mLayout->setColStretch    (0, 2);
  mLayout->setColStretch    (0, 1);
  mLayout->setColStretch    (0, 3);
  
//  setSizePolicy (QSizePolicy (QSizePolicy::Preferred, QSizePolicy::Fixed));
  
  //----------------------------------------------------------------------------
  //  Create combobox.
  //----------------------------------------------------------------------------
  mJob  = new QComboBox (true, this);
  mJob->setFixedWidth   (140);
  mLayout->addWidget    (mJob, 0, 0);

  //----------------------------------------------------------------------------
  //  Create the labels.
  //----------------------------------------------------------------------------
  label1 = new QLabel (tr("Company"), this);
  label2 = new QLabel (tr("Department"), this);
  label3 = new QLabel (tr("Manager"), this);
  label4 = new QLabel (tr("Assistant"), this);
  
  mLayout->addWidget (label1, 0, 1);
  mLayout->addWidget (label2, 1, 1);
  mLayout->addWidget (label3, 2, 1);
  mLayout->addWidget (label4, 3, 1);
  
  //----------------------------------------------------------------------------
  //  Create the lineedits.
  //----------------------------------------------------------------------------
  mCompany    = new QLineEdit (this);
  mDepartment = new QLineEdit (this);
  mManager    = new QLineEdit (this);
  mAssistant  = new QLineEdit (this);

  //----------------------------------------------------------------------------
  //  Create the pushbutton.
  //----------------------------------------------------------------------------
  mDelete = new QPushButton (this);
  mDelete->setPixmap        (*icon);
  mDelete->setFixedSize     (24,24);

  QToolTip::add (mDelete, tr("Delete this job"));
  connect       (mDelete, SIGNAL(clicked()), this, SLOT(slotDeleteLine()));

  //----------------------------------------------------------------------------
  //  Add the lineedits and the pushbutton to the layout.
  //----------------------------------------------------------------------------
  mLayout->addWidget (mCompany,    0, 2);
  mLayout->addWidget (mDepartment, 1, 2);
  mLayout->addWidget (mManager,    2, 2);
  mLayout->addWidget (mAssistant,  3, 2);
  mLayout->addWidget (mDelete,     0, 3);

  connectSlots();
}

//=============================================================================
// Destructor of the class CJobBox.
//=============================================================================
CJobBox::~CJobBox()
{
  FREE_PTR(mJob);
  FREE_PTR(mCompany);
  FREE_PTR(mDepartment);
  FREE_PTR(mManager);
  FREE_PTR(mAssistant);
  FREE_PTR(mDelete);
  FREE_PTR(mLayout);
}

//=============================================================================
// Set the tooltip text for the widget.
//=============================================================================
void CJobBox::setToolTip (QString tip)
{
  QToolTip::add (mJob,        tip);
  QToolTip::add (mCompany,    tip);
  QToolTip::add (mDepartment, tip);
  QToolTip::add (mManager,    tip);
  QToolTip::add (mAssistant,  tip);
}

//=============================================================================
// Clear the edit line and set the first combobox item.
//=============================================================================
void CJobBox::clear ()
{
  disconnectSlots();
  
  mJob->setCurrentItem (0);
  mCompany->clear();
  mDepartment->clear();
  mManager->clear();
  mAssistant->clear();

  connectSlots();
}

//=============================================================================
// Make the widget readonly.
//=============================================================================
void CJobBox::setReadOnly (bool readonly)
{
  mJob->setEnabled         (!readonly);
  mCompany->setReadOnly    (readonly);
  mDepartment->setReadOnly (readonly);
  mManager->setReadOnly    (readonly);
  mAssistant->setReadOnly  (readonly);
  mDelete->setEnabled      (!readonly);
}

//=============================================================================
// SLOT: Notice that the content has changed.
//=============================================================================
void CJobBox::slotContentChanged (const QString &text)
{ 
  mContentChanged = true;
  emit contentChanged ();
}

//=============================================================================
// SLOT: Notice that the content has changed.
//=============================================================================
void CJobBox::slotContentChanged ()
{
  mContentChanged = true;
  emit contentChanged ();
}

//=============================================================================
// SLOT: Delete this line.
//=============================================================================
void CJobBox::slotDeleteLine ()
{
  int ret = QMessageBox::information (this, tr ("Delete Job"),
                                  tr ("Do you really want to delete this job?"),
                                  QMessageBox::Yes | QMessageBox::Default, 
                                  QMessageBox::No);

  if (ret == QMessageBox::Yes)
    emit lineDeleted (mID);
}

//=============================================================================
// Connect the slots to the signals.
//=============================================================================
void CJobBox::connectSlots()
{
  connect (mJob,        SIGNAL (activated   	   (const QString &)),
           this,        SLOT   (slotContentChanged (const QString &)));
  connect (mCompany,    SIGNAL (textChanged        (const QString &)),
           this,        SLOT   (slotContentChanged (const QString &)));
  connect (mDepartment, SIGNAL (textChanged        (const QString &)),
           this,        SLOT   (slotContentChanged (const QString &)));
  connect (mManager,    SIGNAL (textChanged        (const QString &)),
           this,        SLOT   (slotContentChanged (const QString &)));
  connect (mAssistant,  SIGNAL (textChanged        (const QString &)),
           this,        SLOT   (slotContentChanged (const QString &)));
}

//=============================================================================
// Disconnect the slots from the signals.
//=============================================================================
void CJobBox::disconnectSlots()
{
  mJob->disconnect();
  mCompany->disconnect();
  mDepartment->disconnect();
  mManager->disconnect();
  mAssistant->disconnect();
}
