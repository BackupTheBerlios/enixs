//#############################################################################
//#  enixsview.h
//#  ===========
//#
//#  Creation:       Die Apr  2 16:25:16 CEST 2002
//#  Author:         Ralf Schneider  (ralf.schneider@b-connected.de)
//#  Copyright:      (C) 2002 by B-connected Linux Solutions GmbH
//#
//#  ------------------------------------------------------------------------
//#
//#  This file contains the definition of the class CEnixsView.
//#  This is the main class of the application.
//#
//#  ------------------------------------------------------------------------
//#
//#  This program is free software; you can redistribute it and/or modify it
//#  under the terms of the GNU General Public License as published by the
//#  Free Software Foundation; either version 2 of the License, or (at your
//#  option) any later version.
//#
//#############################################################################

#ifndef ENIXSVIEW_H
#define ENIXSVIEW_H

//=============================================================================
// Include files for QT.
//=============================================================================
#include <qtabwidget.h>
#include <qasciidict.h>
#include <qstringlist.h>

//=============================================================================
// Application specific includes.
//=============================================================================
#include "whatsup/whatsup.h"
#include "plugindef/plugindef.h"


typedef QStringList (*keysFunction) ();
typedef QString     (*groupFunction) ();


class CEnixsView : public QTabWidget
{
  Q_OBJECT

public:
  CEnixsView (QWidget* parent, const char *name, int wflags);
  ~CEnixsView();

  void        loadPlugins ();
  void        update	  (CEnixsView	*sender);
  void        print		  (QPrinter *printer);
  CPluginDef* pluginDef   (QString key) { return mPlugins[key]; }
		
protected:
  virtual void closeEvent	 (QCloseEvent *e);
  virtual void keyPressEvent (QKeyEvent *e);

protected slots:
  void slotShowPlugin     (QString name);

signals:
  void buttonClicked (QString);

private:
  CWhatsUp*					mWhatsUp;
  QAsciiDict<CPluginDef>	mPlugins;
  QAsciiDict<CTabPage>      mPages;
  QStringList               mPagesList;
};

#endif
