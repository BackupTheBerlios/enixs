//#############################################################################
//#  enixsview.cpp
//#  =============
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

//=============================================================================
// Include files for QT.
//=============================================================================
#include <qprinter.h>
#include <qpainter.h>
#include <qdir.h>
#include <qstringlist.h>
#include <qwidgetfactory.h>
#include <qapplication.h>
#include <qpushbutton.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qregexp.h>

//=============================================================================
// Application specific includes.
//=============================================================================
#include "enixsview.h"
#include "tabpage/tabpage.h"
#include "plugin/widgetfactory.h"
#include <iostream.h>
#include <stdlib.h>

extern QString    mEnixsDir;

//=============================================================================
// Constructor of the EniXs view class.
//=============================================================================
CEnixsView::CEnixsView (QWidget *parent, const char* name, int wflags)
 : QTabWidget (parent, name, wflags)
{
  CTabPage*      page;
  QString        group;
  CPluginButton *button;
  QStringList    keys;
  CPluginDef*    plugin;
  unsigned int   i;
  
  resize (600, 450);

  //---------------------------------------------------------------------------
  // Load the plugins.
  //---------------------------------------------------------------------------
  mPlugins.setAutoDelete (true);
  loadPlugins();

  //---------------------------------------------------------------------------
  // Create the tab for the What's Up summary page.
  //---------------------------------------------------------------------------
  mWhatsUp = new CWhatsUp (this, "whats up", wflags);
  addTab (mWhatsUp, tr("What's &Up"));
  mPages.insert ("Whats Up", mWhatsUp);

  //---------------------------------------------------------------------------
  // For each plugin create a button that starts the plugins main window.
  //---------------------------------------------------------------------------
  QAsciiDictIterator<CPluginDef>  it(mPlugins);

  for ( ; it.current(); ++it)
    keys << it.current()->name();
  
  keys.sort();
  
  for (i = 0; i < keys.count(); i++)
  {
    plugin = mPlugins[keys[i]];
    group  = plugin->group().replace(QRegExp("&"), "");

    if (!mPagesList.contains (group))
    {
      page = new CTabPage (this, group, wflags);
      addTab (page, plugin->group());
      mPages.insert (group, page);
      mPagesList << group;
    }
    else
      page = mPages[group];

    button = page->addButton (plugin->name(), plugin->icon());
    connect (button, SIGNAL(clicked(QString)), this, SLOT(slotShowPlugin(QString)));

    QToolTip::add   (button, plugin->toolTip());
    QWhatsThis::add (button, plugin->whatsThis());
  }
}

//=============================================================================
// Destructor of the EniXs view class.
//=============================================================================
CEnixsView::~CEnixsView()
{
}


//=============================================================================
// Load the list of installed plugins.
//=============================================================================
void CEnixsView::loadPlugins ()
{
  CPluginDef*		pluginDef;
  QString			path;

  //---------------------------------------------------------------------------
  // Create which plugins are available.
  //---------------------------------------------------------------------------
  QDir libs (mEnixsDir + "/plugins", "*.so");

  mPlugins.resize (libs.count());

  //---------------------------------------------------------------------------
  // Load the plugin definition of each plugin.
  //---------------------------------------------------------------------------
  for (uint i = 0; i < libs.count(); i++)
  {
    path = libs.path() + QDir::separator() + libs[i];
    CWidgetFactory *wf = new CWidgetFactory (path);

    pluginDef = new CPluginDef ();
    pluginDef->setLib		(path);
    pluginDef->setName      (wf->name());
    pluginDef->setGroup     (wf->group());
    pluginDef->setIcon      (wf->icon());
    pluginDef->setToolTip   (wf->toolTip());
    pluginDef->setWhatsThis (wf->whatsThis());
    
    mPlugins.insert (wf->name(), pluginDef);

    delete wf;
  }
}

//=============================================================================
// SLOT: Show the given plugin.
//=============================================================================
void CEnixsView::slotShowPlugin (QString name)
{
  emit buttonClicked (name);
}

//=============================================================================
// Update the view.
//=============================================================================
void CEnixsView::update (CEnixsView *sender)
{
  if (sender != this)
    repaint();
}

//=============================================================================
// Print the view.
//=============================================================================
void CEnixsView::print (QPrinter *printer)
{
  if (printer->setup (this))
  {
    QPainter p;
    p.begin (printer);
    
    ///////////////////////////////
    // TODO: add your printing code here
    ///////////////////////////////
		
    p.end();
  }
}

//=============================================================================
// Close the view.
//=============================================================================
void CEnixsView::closeEvent (QCloseEvent*)
{
  // LEAVE THIS EMPTY: THE EVENT FILTER IN THE EniXsApp CLASS TAKES CARE FOR CLOSING
  // QWidget closeEvent must be prevented.
}

//=============================================================================
// Handle the press of the function keys.
//=============================================================================
void CEnixsView::keyPressEvent (QKeyEvent *e)
{
  CTabPage *page;
  QString   name;

  if (currentPageIndex() == 0)
    return;

  page = (CTabPage *)currentPage();

  switch (e->key())
  {
    case Qt::Key_F1:
      name = page->button(0);
      break;
    case Qt::Key_F2:
      name = page->button(1);
      break;
    case Qt::Key_F3:
      name = page->button(2);
      break;
    case Qt::Key_F4:
      name = page->button(3);
      break;
    case Qt::Key_F5:
      name = page->button(4);
      break;
    case Qt::Key_F6:
      name = page->button(5);
      break;
    case Qt::Key_F7:
      name = page->button(6);
      break;
    case Qt::Key_F8:
      name = page->button(7);
      break;
    case Qt::Key_F9:
      name = page->button(8);
      break;
    case Qt::Key_F10:
      name = page->button(9);
      break;
    case Qt::Key_F11:
      name = page->button(10);
      break;
    case Qt::Key_F12:
      name = page->button(11);
      break;
  }

  if (!name.isEmpty())
    emit buttonClicked (name);
}

