//#############################################################################
//#  util.h
//#  ======
//#
//#  Creation:       Mon Jul 08 17:39:16 CEST 2002
//#  Author:         Ralf Schneider  (ralf.schneider@b-connected.de)
//#  Copyright:      (C) 2002 by B-connected Linux Solutions GmbH
//#
//#  ------------------------------------------------------------------------
//#
//#  This file contains the definition of the class QSAPDBHandles as well as
//#  some utility functions.
//#
//#  ------------------------------------------------------------------------
//#
//#  This program is free software; you can redistribute it and/or modify it
//#  under the terms of the GNU General Public License as published by the
//#  Free Software Foundation; either version 2 of the License, or (at your
//#  option) any later version.
//#
//#############################################################################

#ifndef UTIL_H
#define UTIL_H

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

#define LONG_SEGMENT_SIZE 8192

//=============================================================================
// Handles class.
//=============================================================================
class QSAPDBHandles
{
public:
  QSAPDBHandles()
    : hEnv(0), hDbc(0), hStmt(0)
  {}

  SQLHANDLE hEnv;
  SQLHANDLE hDbc;
  SQLHANDLE hStmt;

  bool checkDriver() const;
};

//=============================================================================
// Function Prototypes.
//=============================================================================
QString        qWarnSAPDBHandle (int handleType, SQLHANDLE handle);
QString        qSAPDBWarn       (const QSAPDBHandles* db);
void           qSqlWarning      (const QString& message, const QSAPDBHandles* db);
QSqlError      qMakeError       (const QString& err, int type,
                                 const QSAPDBHandles* db);
QVariant::Type qDecodeSAPDBType (const SQLSMALLINT sqltype);
QSqlFieldInfo  qMakeFieldInfo   (const QSAPDBHandles* db, int i);
QString        qGetStringData   (SQLHANDLE hStmt, int index, SQLINTEGER& length, 
                                 bool& isNull);
QByteArray     qGetBinaryData   (SQLHANDLE hStmt, int index, bool& isNull);
int            qGetIntData      (SQLHANDLE hStmt, int column, bool& isNull);
QSqlFieldInfo  qMakeFieldInfo   (const QSAPDBHandles* db, const QString& tablename, 
                                 const QString& fieldname);
QSqlField      qMakeField       (const QSAPDBHandles* db, const QString& tablename, 
                                 const QString& fieldname);
QSqlField      qMakeField       (const QSAPDBHandles* db, int i);

#endif
