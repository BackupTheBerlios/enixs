//#############################################################################
//#  photo.h
//#  =======
//#
//#  Creation:       Mit Jul 24 22:33:40 CEST 2002
//#  Author:         Ralf Schneider  (ralf.schneider@b-connected.de)
//#  Copyright:      (C) 2002 by B-connected Linux Solutions GmbH
//#
//#  ------------------------------------------------------------------------
//#
//#  This file contains the definition of the class CPhoto.
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

#ifndef PHOTO_H
#define PHOTO_H

//=============================================================================
// Include files for QT.
//=============================================================================
#include <qlabel.h>
#include <qpopupmenu.h>

//=============================================================================
// Application specific includes.
//=============================================================================


class CPhoto : public QLabel
{
  Q_OBJECT

public: 
  CPhoto (QString text, QWidget *parent=0, const char *name=0);
  ~CPhoto();

  bool    hasChanged   ()          { return mContentChanged; }
  bool    isNull       ()          { return pixmap()->isNull(); }  
  void    setChanged   (bool flag) { mContentChanged = flag; }
  void    setReadOnly  (bool flag) { mReadOnly = flag; }
  void    setPixmap    (const QPixmap &p);
//  QByteArray data      ()          { return 
signals:
  void    contentChanged  	 ();

protected:
  void    mousePressEvent       (QMouseEvent *e);
  void    mouseDoubleClickEvent (QMouseEvent *e);

protected slots:
  void    slotLoad ();
  void    slotDelete();
 
private:
  QPopupMenu*  mMenu;
  bool	       mContentChanged;
  bool         mReadOnly;
};

#endif
