//#############################################################################
//#  plugin.h
//#  ========
//#
//#  Creation:       Mon Jul 08 17:39:16 CEST 2002
//#  Author:         Ralf Schneider  (ralf.schneider@b-connected.de)
//#  Copyright:      (C) 2002 by B-connected Linux Solutions GmbH
//#
//#  ------------------------------------------------------------------------
//#
//#  This file contains the definition of the class QSAPDBDriverPlugin.
//#  This is the plugin class that implements the interface for being used
//#  as a database driver from within the Qt toolkit.
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
#include <qsqldriverplugin.h>


class QSAPDBDriverPlugin : public QSqlDriverPlugin
{
public:
  QSAPDBDriverPlugin();

  QSqlDriver*   create (const QString & key);
  QStringList   keys   () const;
};

Q_EXPORT_PLUGIN(QSAPDBDriverPlugin)
