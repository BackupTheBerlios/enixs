//#############################################################################
//#  tabpage.h
//#  =========
//#
//#  Creation:       Mon Apr 22 23:55:16 CEST 2002
//#  Author:         Ralf Schneider  (ralf.schneider@b-connected.de)
//#  Copyright:      (C) 2002 by B-connected Linux Solutions GmbH
//#
//#  ------------------------------------------------------------------------
//#
//#  This file contains the definition of the class CTabPage.
//#  This is a generic class for displaying a new page of plugin buttons.
//#
//#  ------------------------------------------------------------------------
//#
//#  This program is free software; you can redistribute it and/or modify it
//#  under the terms of the GNU General Public License as published by the
//#  Free Software Foundation; either version 2 of the License, or (at your
//#  option) any later version.
//#
//#############################################################################

#ifndef TABPAGE_H
#define TABPAGE_H

//=============================================================================
// Include files for QT.
//=============================================================================
#include <qframe.h>
#include <qptrlist.h>
#include <qpixmap.h>
#include <qscrollview.h>

//=============================================================================
// Application specific includes.
//=============================================================================
#include "pluginbutton.h"


class CTabPage : public QScrollView
{
  Q_OBJECT

public:
  CTabPage (QWidget* parent, const char *name, int wflags);
  ~CTabPage();

  CPluginButton* addButton (QString label, QPixmap icon);
  QString        button    (int index);
		
protected:
  void resizeEvent       (QResizeEvent *e);
  void drawContents      (QPainter * p, int clipx, int clipy, int clipw, int cliph);

private:
  QPtrList<QWidget>      mWidgets;
};

#endif
