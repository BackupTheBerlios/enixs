//#############################################################################
//#  tabpage.cpp
//#  ===========
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

//=============================================================================
// Include files for QT.
//=============================================================================
#include <qprinter.h>
#include <qpainter.h>
#include <qpushbutton.h>

//=============================================================================
// Application specific includes.
//=============================================================================
#include "tabpage.h"


//=============================================================================
// Constructor of the class CTabPage.
//=============================================================================
CTabPage::CTabPage (QWidget *parent, const char* name, int wflags)
 : QScrollView (parent, name, wflags)
{
  setHScrollBarMode  (AlwaysOff);
  enableClipper      (true);
  setFrameStyle      (NoFrame | Plain);
}

//=============================================================================
// Destructor of the class CTabPage.
//=============================================================================
CTabPage::~CTabPage()
{
}

//=============================================================================
// Add a widget to the page.
//=============================================================================
CPluginButton* CTabPage::addButton (QString label, QPixmap icon)
{
  CPluginButton*    button;

  button = new CPluginButton (label, icon, this, label);
  addChild (button);
  mWidgets.append (button);

  return button;
}

//=============================================================================
// Return the label of the button at the given index.
//=============================================================================
QString CTabPage::button (int index)
{
  if (index >= (int)mWidgets.count())
    return "";

  return mWidgets.at(index)->name();
}

//=============================================================================
// Handle the resize event.
//=============================================================================
void CTabPage::resizeEvent (QResizeEvent* e)
{
  uint   i;
  int    xpos = 20, ypos = 20;

  for (i = 0; i < mWidgets.count(); i++)
  {
    moveChild (mWidgets.at(i), xpos, ypos);

    xpos += 180;

    if (xpos + 180 > e->size().width())
    {
      xpos = 20;
      ypos += 140;
    }
  }

  resizeContents (e->size().width() - 20, ypos + 140);
  QScrollView::resizeEvent (e);
}

//=============================================================================
// Draw the contents of the scrollview.
//=============================================================================
void CTabPage::drawContents (QPainter * p, int clipx, int clipy, int clipw, int cliph)
{
  p->fillRect (clipx, clipy, clipw, cliph, QColor ("#dcdcdc"));
}

