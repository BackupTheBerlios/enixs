//#############################################################################
//#  result.h
//#  ========
//#
//#  Creation:       Mon Jul 08 17:39:16 CEST 2002
//#  Author:         Ralf Schneider  (ralf.schneider@b-connected.de)
//#  Copyright:      (C) 2002 by B-connected Linux Solutions GmbH
//#
//#  ------------------------------------------------------------------------
//#
//#  This file contains the definition of the class QSAPDBResult.
//#  This implements a result set when retrieving data over the ODBC driver.
//#
//#  ------------------------------------------------------------------------
//#
//#  This program is free software; you can redistribute it and/or modify it
//#  under the terms of the GNU General Public License as published by the
//#  Free Software Foundation; either version 2 of the License, or (at your
//#  option) any later version.
//#
//#############################################################################

#ifndef RESULT_H
#define RESULT_H

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


class QSAPDBResult : public QSqlResult
{
  friend class QSAPDBDriver;
  
public:
  QSAPDBResult (const QSAPDBDriver *driver, QSAPDBHandles* db);
  ~QSAPDBResult();

  SQLHANDLE   statement();

protected:
  bool	      fetchFirst     ();
  bool	      fetchLast      ();
  bool	      fetch          (int i);
  bool	      reset          (const QString& query);
  QVariant	  data           (int field);
  bool	      isNull         (int field);
  int         size           ();
  int         numRowsAffected();
  
private:
  QSAPDBHandles*	          mDB;
  
  typedef QMap<int,QVariant>  FieldCache;
  typedef QMap<int,bool>      NullCache;
  FieldCache                  mFieldCache;
  NullCache                   mNullCache;
};

#endif
