//#############################################################################
//#  util.cpp
//#  ========
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

//=============================================================================
// Include files for QT.
//=============================================================================
#include <qsqlrecord.h>
#include <qfile.h>

#if defined (Q_OS_WIN32)
#include <qt_windows.h>
#endif
#include <qapplication.h>
#include <qdatetime.h>

//=============================================================================
// Application specific includes.
//=============================================================================
#include "util.h"


//=============================================================================
// Check the features of the dirver.
//=============================================================================
bool QSAPDBHandles::checkDriver() const
{
#ifdef SAPDB_CHECK_DRIVER
  SQLRETURN     ret;
  SQLUSMALLINT  supported;
  int           i;

  //----------------------------------------------------------------------------
  //  These functions are required.
  //----------------------------------------------------------------------------
  static const SQLUSMALLINT reqFunc[] = { 
    SQL_API_SQLDESCRIBECOL, SQL_API_SQLGETDATA, SQL_API_SQLCOLUMNS, 
    SQL_API_SQLGETSTMTATTR, SQL_API_SQLGETDIAGREC, SQL_API_SQLEXECDIRECT,
    SQL_API_SQLGETINFO, SQL_API_SQLTABLES, 0}; // TODO: SQL_API_SQLPRIMARYKEYS

  //----------------------------------------------------------------------------
  //  These functions are optional.
  //----------------------------------------------------------------------------
  static const SQLUSMALLINT optFunc[] = {
    SQL_API_SQLNUMRESULTCOLS, SQL_API_SQLROWCOUNT, 0};

  //----------------------------------------------------------------------------
  //  Check for the required functions.
  //----------------------------------------------------------------------------
  for (i = 0; reqFunc[i] != 0; i++) 
  {
    if (SQLGetFunctions (hDbc, reqFunc[i], &supported) != SQL_SUCCESS) 
    {
      qSqlWarning ("QSAPDBDriver::checkDriver: Cannot get list of supported "
                   "functions", this);
      return false;
    }

    if (supported == SQL_false) 
      return false;
  }

  //----------------------------------------------------------------------------
  //  Check for the optional functions. If one is missing, only a warning is
  //  generated.
  //----------------------------------------------------------------------------
  for (i = 0; optFunc[i] != 0; i++) 
  {
    if (SQLGetFunctions (hDbc, optFunc[i], &supported) != SQL_SUCCESS) 
    {
      qSqlWarning ("QSAPDBDriver::checkDriver: Cannot get list of supported "
                   "functions", this);
      return false;
    }

    if (supported == SQL_false) 
    {
#ifdef QT_CHECK_RANGE
      qWarning ("QSAPDBDriver::checkDriver: Warning - Driver doesn't support "
                "some non-critical functions");
#endif
      return true;
    }
  }

#endif //SAPDB_CHECK_DRIVER

  return true;
}

//=============================================================================
// Generate a warning message for the given handle.
//=============================================================================
void qWarnSAPDBHandle (int handleType, SQLHANDLE handle, QString* text, int* code)
{
  SQLSMALLINT tmp;
  SQLRETURN   ret = SQL_ERROR;
  SQLCHAR     state[SQL_SQLSTATE_SIZE+1];
  SQLCHAR     description[SQL_MAX_MESSAGE_LENGTH];

  ret = SQLGetDiagRec (handleType, handle, 1, state, (SQLINTEGER *)code,description,
                       SQL_MAX_MESSAGE_LENGTH-1, &tmp);

  if ((ret == SQL_SUCCESS) || (ret == SQL_SUCCESS_WITH_INFO))
    *text = QString((const char*)description);
  else
    *text = QString::null;
}

//=============================================================================
// Generate a warning message for the given handles object.
//=============================================================================
void qSAPDBWarn (const QSAPDBHandles* db, QString* text, int* code)
{
  QString env, dbc, stmt;
  
  qWarnSAPDBHandle (SQL_HANDLE_ENV,  db->hEnv, &env, code);
  qWarnSAPDBHandle (SQL_HANDLE_DBC,  db->hDbc, &dbc, code);
  qWarnSAPDBHandle (SQL_HANDLE_STMT, db->hStmt, &stmt, code);

  *text = env + " " + dbc + " " + stmt;
}

//=============================================================================
// Generate a warning message for the given handle object.
//=============================================================================
void qSqlWarning (const QString& message, const QSAPDBHandles* db)
{
#ifdef QT_CHECK_RANGE
  QString   text;
  int       code;
  
  qSAPDBWarn (db, &text, &code);
  qWarning (message + "\tError:" + text, code);
#endif
}

//=============================================================================
// Generate an SQL error object for the given handle object.
//=============================================================================
QSqlError qMakeError (const QString& err, int type, const QSAPDBHandles* db)
{
  QString   text;
  int       code;
  
  qSAPDBWarn (db, &text, &code);
  return QSqlError ("QSAPDB7: " + err, text, type, code);
}

//=============================================================================
// Convert the given ODBC datatype to a Qt datatyep.
//=============================================================================
QVariant::Type qDecodeSAPDBType (const SQLSMALLINT sqltype)
{
  QVariant::Type type = QVariant::Invalid;
  
  switch (sqltype) 
  {
    case SQL_DECIMAL:
    case SQL_NUMERIC:
    case SQL_REAL:
    case SQL_FLOAT:
    case SQL_DOUBLE:
      type = QVariant::Double;
      break;
    case SQL_SMALLINT:
    case SQL_INTEGER:
    case SQL_BIT:
    case SQL_TINYINT:
    case SQL_BIGINT:
      type = QVariant::Int;
      break;
    case SQL_BINARY:
    case SQL_VARBINARY:
    case SQL_LONGVARBINARY:
      type = QVariant::ByteArray;
      break;
    case SQL_DATE:
    case SQL_TYPE_DATE:
      type = QVariant::Date;
      break;
    case SQL_TIME:
    case SQL_TYPE_TIME:
      type = QVariant::Time;
      break;
    case SQL_TIMESTAMP:
    case SQL_TYPE_TIMESTAMP:
      type = QVariant::DateTime;
      break;
    default:
    case SQL_CHAR:
    case SQL_VARCHAR:
    case SQL_LONGVARCHAR:
      type = QVariant::String;
      break;
  }
  return type;
}

//=============================================================================
// Generate a QSqlFieldInfo object for a given field of a result.
//=============================================================================
QSqlFieldInfo qMakeFieldInfo (const QSAPDBHandles* db, int i)
{
  SQLSMALLINT   columnLength;
  SQLSMALLINT   columnType;
  SQLUINTEGER   columnSize;
  SQLSMALLINT   columnScale;
  SQLSMALLINT   nullable;
  SQLCHAR       column[255];
  int           required = -1;

  //----------------------------------------------------------------------------
  //  Read the column description.
  //----------------------------------------------------------------------------
  if (SQLDescribeCol (db->hStmt, i + 1, column, sizeof(column), &columnLength,
                      &columnType, &columnSize, &columnScale, &nullable) 
      != SQL_SUCCESS) 
  {
    qSqlWarning (QString("qMakeField: Unable to describe column %1").arg(i), db);
    return QSqlFieldInfo();
  }

  //----------------------------------------------------------------------------
  //  Nullable can be SQL_NO_NULLS, SQL_NULLABLE or SQL_NULLABLE_UNKNOWN.
  //----------------------------------------------------------------------------
  if (nullable == SQL_NO_NULLS) 
    required = 1;
  else if (nullable == SQL_NULLABLE) 
    required = 0;
  
  //----------------------------------------------------------------------------
  //  Convert the ODBC datatype to a Qt datatype.
  //----------------------------------------------------------------------------
  QVariant::Type type = qDecodeSAPDBType (columnType);
  
  //----------------------------------------------------------------------------
  //  Return the generated field info object.
  //----------------------------------------------------------------------------
  return QSqlFieldInfo (QString((const char*)column), type, required, 
                        (int)columnSize == 0 ? -1 : (int)columnSize,
                        (int)columnScale == 0 ? -1 : (int)columnScale,
                        QVariant(), (int)columnType);
}

//=============================================================================
// Read string data from the database at a given index.
//=============================================================================
QString qGetStringData (SQLHANDLE hStmt, int index, SQLINTEGER& length,bool& isNull)
{
  QString       fieldValue;
  SQLSMALLINT   columnLength;
  SQLSMALLINT   columnType;
  SQLUINTEGER   columnSize;
  SQLSMALLINT   columnScale;
  SQLSMALLINT   nullable;
  SQLRETURN     ret = SQL_ERROR;
  SQLCHAR       column[255];
  SQLCHAR*      buffer;
  
  //----------------------------------------------------------------------------
  //  Read the column description.
  //----------------------------------------------------------------------------
  if (SQLDescribeCol (hStmt, index + 1, column, sizeof(column), &columnLength,
                      &columnType, &columnSize, &columnScale, &nullable) 
      != SQL_SUCCESS)
    qWarning (QString("qGetStringData: Unable to describe column %1").arg(index));

  // SQLDescribeCol may return 0 if size cannot be determined
  if (!columnSize)
    columnSize = 255;

  if (columnSize > 1048576) // limit fields larger than 1 MB
    columnSize = 1048576;

  buffer = new SQLTCHAR[columnSize + 1];

  //----------------------------------------------------------------------------
  //  Read the string data.
  //----------------------------------------------------------------------------
  while (true) 
  {
    ret = SQLGetData (hStmt, index + 1, SQL_C_CHAR, (SQLPOINTER)buffer, 
                      columnSize + 1, &length);

    if ((ret == SQL_SUCCESS) || (ret == SQL_SUCCESS_WITH_INFO))
    {
      if (length == SQL_NO_TOTAL)
        fieldValue += QString ((const char *)buffer);
      else if (length == SQL_NULL_DATA) 
      {
        fieldValue = QString::null;
        isNull     = true;
        break;
      } 
      else 
      {
        if (ret == SQL_SUCCESS) 
        {
          fieldValue += QString ((const char *)buffer);
          break;
        } 
        else 
        {
          if ((int)fieldValue.length() >= length ) // ### HACK - remove asap
            break;
          fieldValue += QString ((const char *)buffer);
        }
      }
    } 
    else 
    {
      fieldValue += QString::null;
      break;
    }
  }

  delete[] buffer;

  return fieldValue;
}

//=============================================================================
// Read binary data from the database at a given index.
//=============================================================================
QByteArray qGetBinaryData (SQLHANDLE hStmt, int index, bool& isNull)
{
  QByteArray    fieldValue;
  SQLINTEGER    length, numBytes, oldSize;
  char          buffer[LONG_SEGMENT_SIZE];

  //----------------------------------------------------------------------------
  //  Read the binary data.
  //----------------------------------------------------------------------------
  while (SQLGetData (hStmt, index + 1, SQL_C_BINARY, buffer, 
                     LONG_SEGMENT_SIZE, &length)
         != SQL_NO_DATA)
  {
    if (length == SQL_NULL_DATA)
    {
      isNull = true;
      return fieldValue;
    }

    numBytes = ((length > LONG_SEGMENT_SIZE) || (length == SQL_NO_TOTAL)) ? 
               LONG_SEGMENT_SIZE : length;
    
    oldSize = fieldValue.size();
    fieldValue.resize (fieldValue.size() + numBytes);
    memcpy (fieldValue.data() + oldSize, buffer, numBytes);
  }
  
  return fieldValue;
}

//=============================================================================
// Read integer data from the database at a given index.
//=============================================================================
int qGetIntData (SQLHANDLE hStmt, int column, bool& isNull)
{
  SQLINTEGER   buffer;
  SQLINTEGER   length = 0;
  SQLRETURN    ret;
  
  isNull = false;

  ret = SQLGetData (hStmt, column+1, SQL_C_SLONG, (SQLPOINTER)&buffer, 0, &length);

  if ((ret == SQL_SUCCESS) || (ret == SQL_SUCCESS_WITH_INFO))
  {
    if (length == SQL_NULL_DATA)
      isNull = true;
  }
  
  return (int)buffer;
}

//=============================================================================
// Generate a QSqlFieldInfo object.
//=============================================================================
QSqlFieldInfo qMakeFieldInfo (const QSAPDBHandles* db, const QString& tablename, 
                              const QString& fieldname)
{
  QSqlFieldInfo info;
  SQLHANDLE     hStmt;
  bool          isNull;
  int           type;
  int           required;
  int           size;
  int           prec;

  //----------------------------------------------------------------------------
  //  Allocate a statement handle.
  //----------------------------------------------------------------------------
  if (SQLAllocHandle (SQL_HANDLE_STMT, db->hDbc, &hStmt) != SQL_SUCCESS) 
  {
    qSqlWarning ("qMakeField: Unable to alloc handle", db);
    return info;
  }

  //----------------------------------------------------------------------------
  //  Set the cursor type to ForwardOnly.
  //----------------------------------------------------------------------------
  SQLSetStmtAttr (hStmt, SQL_ATTR_CURSOR_TYPE, (SQLPOINTER)SQL_CURSOR_FORWARD_ONLY,
                  SQL_IS_UINTEGER);

  //----------------------------------------------------------------------------
  //  Get the columns.
  //----------------------------------------------------------------------------
  if (SQLColumns (hStmt, NULL, 0, NULL, 0, (SQLCHAR*)(const char*)tablename,
                  tablename.length(), (SQLCHAR*)(const char*)fieldname,
                  fieldname.length()) 
      != SQL_SUCCESS) 
  {
    qSqlWarning ("qMakeField: Unable to execute column list", db);
    return info;
  }

  //----------------------------------------------------------------------------
  //  Fetch the result.
  //----------------------------------------------------------------------------
  if (SQLFetchScroll (hStmt, SQL_FETCH_NEXT, 0) == SQL_SUCCESS) 
  {
    type     = qGetIntData (hStmt,  4, isNull ); // column type
    required = qGetIntData (hStmt, 10, isNull ); // nullable-flag

    //--------------------------------------------------------------------------
    //  Required can be SQL_NO_NULLS, SQL_NULLABLE or SQL_NULLABLE_UNKNOWN
    //--------------------------------------------------------------------------
    if (required == SQL_NO_NULLS)
      required = 1;
    else if (required == SQL_NULLABLE )
      required = 0;
    else
      required = -1;

    size = qGetIntData (hStmt, 6, isNull ); // column size
    prec = qGetIntData (hStmt, 8, isNull ); // precision
    info = QSqlFieldInfo (fieldname, qDecodeSAPDBType (type), required, size, 
                          prec, QVariant(), type);
  }

  if (SQLFreeHandle (SQL_HANDLE_STMT, hStmt) != SQL_SUCCESS)
    qSqlWarning ("QSAPDBDriver: Unable to free statement handle", db);

  return info;
}

//=============================================================================
// Generate a QSqlField object.
//=============================================================================
QSqlField qMakeField (const QSAPDBHandles* db, const QString& tablename, 
                      const QString& fieldname)
{
  QSqlFieldInfo info = qMakeFieldInfo (db, tablename, fieldname);
  return QSqlField (info.name(), info.type());
}

//=============================================================================
// Generate a QSqlField object.
//=============================================================================
QSqlField qMakeField (const QSAPDBHandles* db, int i)
{
  QSqlFieldInfo info = qMakeFieldInfo (db, i);
  return QSqlField( info.name(), info.type() );
}
