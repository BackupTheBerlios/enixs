//#############################################################################
//#  pluginbutton.cpp
//#  ================
//#
//#  Creation:       Mon Apr 22 23:55:16 CEST 2002
//#  Author:         Ralf Schneider  (ralf.schneider@b-connected.de)
//#  Copyright:      (C) 2002 by B-connected Linux Solutions GmbH
//#
//#  ------------------------------------------------------------------------
//#
//#  This file contains the implementation of the class CPluginButton.
//#  This class shows a plugin button that consists of a label and a big
//#  icon.
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

//=============================================================================
// Application specific includes.
//=============================================================================
#include "pluginbutton.h"


//=============================================================================
// Constructor of the class CPluginButton.
//=============================================================================
CPluginButton::CPluginButton (QString label, QPixmap icon, QWidget *parent, const char* name)
 : QFrame (parent, name)
{
  setFrameStyle (StyledPanel | Raised);
  setFixedSize  (160, 120);
  setBackgroundColor (white);

  QFont f = font();
  f.setBold      (true);
  f.setPointSize (f.pointSize() + 2);

  mIcon = new QLabel (this, "icon");
  mIcon->setPixmap   (icon);
  mIcon->setGeometry (30, 10, 100, 60);

  mText = new QLabel (label, this, "text");
  mText->setGeometry (30, 75, 100, 35);
  mText->setAlignment(AlignCenter);
  mText->setFont     (f);
  mText->setBackgroundColor (white);
}

//=============================================================================
// Destructor of the class CPluginButton.
//=============================================================================
CPluginButton::~CPluginButton()
{
}

//=============================================================================
// Handle the mouse release event.
//=============================================================================
void CPluginButton::mouseReleaseEvent (QMouseEvent* e)
{
  emit clicked (mText->text());
}
