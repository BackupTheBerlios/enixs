//#############################################################################
//#  photo.cpp
//#  =========
//#
//#  Creation:       Mit Jul 24 22:39:26 CEST 2002
//#  Author:         Ralf Schneider  (ralf.schneider@b-connected.de)
//#  Copyright:      (C) 2002 by B-connected Linux Solutions GmbH
//#
//#  ------------------------------------------------------------------------
//#
//#  This file contains the implementation of the class CPhoto.
//#  This class builds a new widget for displaying a picture and offers
//#  a context menu for loading and deleting the image data.
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
#include <qcursor.h>
#include <qfiledialog.h>
#include <qimage.h>

//=============================================================================
// Application specific includes.
//=============================================================================
#include "photo.h"
#include "util.h"

//=============================================================================
// Constructor of the class CPhoto.
//=============================================================================
CPhoto::CPhoto (QString text, QWidget *parent, const char *name)
	: QLabel (text, parent,name)
{
  mMenu = new QPopupMenu (this);
  mMenu->insertItem (tr ("&Load"),   this, SLOT (slotLoad()));
  mMenu->insertItem (tr ("&Delete"), this, SLOT (slotDelete()));

  mContentChanged = false;
}

//=============================================================================
// Destructor of the class CPhoto.
//=============================================================================
CPhoto::~CPhoto()
{
  FREE_PTR(mMenu);
}

//=============================================================================
// Show the context menu if the photo is clicked with the right mouse button.
//=============================================================================
void CPhoto::mousePressEvent (QMouseEvent* e)
{
  if (mReadOnly)
    return;
  
  if (e->button() == RightButton)
    mMenu->exec (QCursor::pos());
}

//=============================================================================
// Show the context menu if the photo is clicked with the right mouse button.
//=============================================================================
void CPhoto::mouseDoubleClickEvent (QMouseEvent* e)
{
  if (!mReadOnly)
    slotLoad();
}

//=============================================================================
// SLOT: Show the File Open dialog for loading an image.
//=============================================================================
void CPhoto::slotLoad()
{
  QString  filename;
  
  filename = QFileDialog::getOpenFileName 
    (QString::null, tr("Images") + " (*.png *.jpg *.xpm *.gif *.tif)",
     this, "open file dialog", tr("Load an Image"));
  
  if (!filename.isEmpty())
  {
    setPixmap (QPixmap (filename));
    mContentChanged = true;
    emit contentChanged();
  }
}

//=============================================================================
// SLOT: Delete the image.
//=============================================================================
void CPhoto::slotDelete()
{
  QLabel::setPixmap (0);
  setText (tr("Photo"));
  mContentChanged = true;
  emit contentChanged();
}

//=============================================================================
// Set the pixmap of the photo.
//=============================================================================
void CPhoto::setPixmap (const QPixmap &p)
{
  if (p.isNull())
    return;
  
  QPixmap image = p;
  image.convertFromImage (image.convertToImage().smoothScale (size()));
  QLabel::setPixmap (image);
}
