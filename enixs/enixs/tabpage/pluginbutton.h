//#############################################################################
//#  pluginbutton.h
//#  ==============
//#
//#  Creation:       Mon Apr 22 23:55:16 CEST 2002
//#  Author:         Ralf Schneider  (ralf.schneider@b-connected.de)
//#  Copyright:      (C) 2002 by B-connected Linux Solutions GmbH
//#
//#  ------------------------------------------------------------------------
//#
//#  This file contains the definition of the class CPluginButton.
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

#ifndef PLUGINBUTTON_H
#define PLUGINBUTTON_H

//=============================================================================
// Include files for QT.
//=============================================================================
#include <qframe.h>
#include <qlabel.h>
#include <qpixmap.h>


class CPluginButton : public QFrame
{
  Q_OBJECT

public:
  CPluginButton (QString label, QPixmap icon, QWidget *parent=0, const char* name=0);
  ~CPluginButton();
		
signals:
  void clicked (QString);

protected:
  void mouseReleaseEvent (QMouseEvent *e);

private:
  QLabel*      mIcon;
  QLabel*      mText;
};

#endif
