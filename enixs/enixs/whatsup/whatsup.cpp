//#############################################################################
//#  whatsup.cpp
//#  ===========
//#
//#  Creation:       Die Apr  2 16:25:16 CEST 2002
//#  Author:         Ralf Schneider  (ralf.schneider@b-connected.de)
//#  Copyright:      (C) 2002 by B-connected Linux Solutions GmbH
//#
//#  ------------------------------------------------------------------------
//#
//#  This file contains the definition of the class CWhatsUp.
//#  This class shows a summary of the different modules.
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

//=============================================================================
// Application specific includes.
//=============================================================================
#include "whatsup.h"


//=============================================================================
// Constructor of the class CWhatsUp.
//=============================================================================
CWhatsUp::CWhatsUp (QWidget *parent, const char* name, int wflags)
 : CTabPage (parent, name, wflags)
{
}

//=============================================================================
// Destructor of the class CWhatsUp.
//=============================================================================
CWhatsUp::~CWhatsUp()
{
}

//=============================================================================
// Draw the contents of the scrollview.
//=============================================================================
void CWhatsUp::drawContents (QPainter * p, int clipx, int clipy, int clipw, int cliph)
{
  p->fillRect (clipx, clipy, clipw, cliph, QColor ("#dcdcdc"));
}
