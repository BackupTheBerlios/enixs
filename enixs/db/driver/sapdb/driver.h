//#############################################################################
//#  driver.h
//#  ========
//#
//#  Creation:       Mon Jul 08 17:39:16 CEST 2002
//#  Author:         Ralf Schneider  (ralf.schneider@b-connected.de)
//#  Copyright:      (C) 2002 by B-connected Linux Solutions GmbH
//#
//#  ------------------------------------------------------------------------
//#
//#  This file contains the definition of the class QSAPDBDriver.
//#  This implements the driver functionality for accessing a SAPDB database
//#  over the ODBC driver.
//#
//#  ------------------------------------------------------------------------
//#
//#  This program is free software; you can redistribute it and/or modify it
//#  under the terms of the GNU General Public License as published by the
//#  Free Software Foundation; either version 2 of the License, or (at your
//#  option) any later version.
//#
//#############################################################################

#ifndef DRIVER_H
#define DRIVER_H

//=============================================================================
// Include files for QT.
//=============================================================================
#include <qmap.h>
#include <qstring.h>
#include <qsqldriver.h>
#include <qsqlfield.h>
#include <qsqlresult.h>
#include <qsqlindex.h>

#if defined (Q_OS_WIN32)
#include <qt_windows.h>
#ifdef UNICODE
#undef UNICODE
#endif
#endif

//=============================================================================
// Database specific includes.
//=============================================================================
#include <lcbasetypes.h>
#include <sql.h>
#include <sqlext.h>

//=============================================================================
// Application specific includes.
//=============================================================================
#include "util.h"

//class QSqlRecordInfo;


class QSAPDBDriver : public QSqlDriver
{
public:
  QSAPDBDriver (QObject * parent=0, const char * name=0);
  ~QSAPDBDriver();
  
  bool	hasFeature (DriverFeature f) const;
  bool	open       (const QString & db, const QString & user = QString::null,
                    const QString & password = QString::null,
                    const QString & host = QString::null, int port = -1);
  void	close      ();
  
  QSqlQuery		    createQuery  () const;
  QStringList		tables       (const QString& user) const;
  QSqlRecord		record       (const QString& tablename) const;
  QSqlRecord		record       (const QSqlQuery& query) const;
  QSqlRecordInfo	recordInfo   (const QString& tablename) const;
  QSqlRecordInfo	recordInfo   (const QSqlQuery& query) const;
  QSqlIndex		    primaryIndex (const QString& tablename) const;
  SQLHANDLE		    environment  ();
  SQLHANDLE		    connection   ();
  QString		    formatValue  (const QSqlField* field, bool trimStrings) const;

protected:
  bool	beginTransaction();
  bool	commitTransaction();
  bool	rollbackTransaction();
    
private:
  bool  endTrans();
  
  QSAPDBHandles* mDB;
};

#endif
