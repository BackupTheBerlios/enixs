//#############################################################################
//#  driver.cpp
//#  ==========
//#
//#  Creation:       Mon Jul 08 17:39:16 CEST 2002
//#  Author:         Ralf Schneider  (ralf.schneider@b-connected.de)
//#  Copyright:      (C) 2002 by B-connected Linux Solutions GmbH
//#
//#  ------------------------------------------------------------------------
//#
//#  This file contains the implementation of the class QSAPDBDriver.
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

//=============================================================================
// Include files for QT.
//=============================================================================
#include <qsqlrecord.h>

#if defined (Q_OS_WIN32)
#include <qt_windows.h>
#endif
#include <qapplication.h>
#include <qdatetime.h>

//=============================================================================
// Application specific includes.
//=============================================================================
#include "driver.h"
#include "result.h"
#include "util.h"


#define SAPDB_CHECK_DRIVER


QSAPDBDriver::QSAPDBDriver (QObject* parent, const char* name)
    : QSqlDriver (parent, name ? name : "QSAPDB7")
{
  mDB = new QSAPDBHandles();
}

QSAPDBDriver::~QSAPDBDriver()
{
  cleanup();
  delete mDB;
}

bool QSAPDBDriver::hasFeature (DriverFeature f) const
{
  SQLUSMALLINT txn;
  SQLSMALLINT  t;
  SQLRETURN    ret;
  
  switch (f) 
  {
    case Transactions:
      if (!mDB->hDbc)
        return false;
      
      ret = SQLGetInfo (mDB->hDbc, (SQLUSMALLINT)SQL_TXN_CAPABLE,
                        &txn, sizeof(txn), &t);

      if (ret != SQL_SUCCESS || txn == SQL_TC_NONE)
        return false;
      else
        return TRUE;

    case QuerySize:
      return TRUE;
      
    case BLOB:
      return false;

    default:
      return false;
  }
}

bool QSAPDBDriver::open (const QString & db, const QString & user,
                         const QString & password, const QString & host, int port)
{
  SQLRETURN  ret;
  QString    connectString;
  SQLCHAR    outString[1024];
  SWORD      outLen;
  
  if (isOpen())
    close();

  ret = SQLAllocHandle (SQL_HANDLE_ENV, SQL_NULL_HANDLE, &mDB->hEnv);
  if ((ret != SQL_SUCCESS) && (ret != SQL_SUCCESS_WITH_INFO)) 
  {
#ifdef QT_CHECK_RANGE
    qSqlWarning( "QSAPDBDriver::open: Unable to allocate environment", mDB);
#endif
    setOpenError (true);
    return false;
  }

  SQLSetEnvAttr (mDB->hEnv, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, 
                 SQL_IS_UINTEGER);

  ret = SQLAllocHandle (SQL_HANDLE_DBC, mDB->hEnv, &mDB->hDbc);

  if (ret != SQL_SUCCESS) 
  {
#ifdef QT_CHECK_RANGE
    qSqlWarning( "QSAPDBDriver::open: Unable to allocate connection", mDB);
#endif
    setOpenError (true);
    return false;
  }

  connectString = "DRIVER=SAP DB 7.3;SERVERDB=" + db + 
                  ";SERVERNODE=" + host + ";UID=" + user + ";PWD=" + password;

  ret = SQLDriverConnect (mDB->hDbc, NULL, (SQLCHAR*)(const char*)connectString, 
                          SQL_NTS, outString, sizeof(outString), &outLen, 
                          SQL_DRIVER_NOPROMPT);

  if ((ret != SQL_SUCCESS) && (ret != SQL_SUCCESS_WITH_INFO))
  {
    setLastError (qMakeError ("Unable to connect", QSqlError::Connection, mDB));
    setOpenError (true);
    return false;
  }

  if (!mDB->checkDriver()) 
  {
#ifdef QT_CHECK_RANGE
    qWarning ("QSAPDBDriver::open: Warning - Driver doesn't support all needed "
              "functionality. Please look at the Qt SQL Module Driver "
              "documentation for more information.");
#endif
    setLastError (qMakeError ("Unable to connect - Driver doesn't support all "
                              "needed functionality", QSqlError::Connection, mDB));
    setOpenError (true);
    return false;
  }

  setOpen (true);
  return true;
}

void QSAPDBDriver::close()
{
  cleanup      ();
  setOpen      (false);
  setOpenError (false);
}

void QSAPDBDriver::cleanup()
{
  SQLRETURN   ret;

  if ((isOpen() || isOpenError()) && (mDB != 0)) 
  {
    if (mDB->hDbc) 
    {
      // Open statements/descriptors handles are automatically cleaned up by 
      // SQLDisconnect
      ret = SQLDisconnect (mDB->hDbc);
#ifdef QT_CHECK_RANGE
      if (ret != SQL_SUCCESS)
        qSqlWarning ("QSAPDBDriver::disconnect: Unable to disconnect datasource",
                     mDB);
#endif
      ret = SQLFreeHandle (SQL_HANDLE_DBC, mDB->hDbc);
#ifdef QT_CHECK_RANGE
      if (ret != SQL_SUCCESS)
        qSqlWarning ("QSAPDBDriver::cleanup: Unable to free connection handle",mDB);
#endif
      mDB->hDbc = 0;
    }

    if (mDB->hEnv) 
    {
      ret = SQLFreeHandle (SQL_HANDLE_ENV, mDB->hEnv);
#ifdef QT_CHECK_RANGE
      if (ret != SQL_SUCCESS)
        qSqlWarning ("QSAPDBDriver::cleanup: Unable to free environment handle",
                     mDB);
#endif
      mDB->hEnv = 0;
    }
  }
}

QSqlQuery QSAPDBDriver::createQuery() const
{
  return QSqlQuery (new QSAPDBResult (this, mDB));
}

bool QSAPDBDriver::beginTransaction()
{
  SQLUINTEGER  ac = SQL_AUTOCOMMIT_OFF;
  SQLRETURN    ret;

  if (!isOpen()) 
  {
#ifdef QT_CHECK_RANGE
    qWarning ("QSAPDBDriver::beginTransaction: Database not open");
#endif
    return false;
  }

  ret = SQLSetConnectAttr (mDB->hDbc, SQL_ATTR_AUTOCOMMIT, (SQLPOINTER)ac, 
                           sizeof(ac));

  if (ret != SQL_SUCCESS) 
  {
    setLastError (qMakeError ("Unable to disable autocommit", 
                              QSqlError::Transaction, mDB));
    return false;
  }
  return true;
}

bool QSAPDBDriver::commitTransaction()
{
  SQLRETURN    ret;

  if (!isOpen()) 
  {
#ifdef QT_CHECK_RANGE
    qWarning ("QSAPDBDriver::commitTransaction: Database not open");
#endif
    return false;
  }

  ret = SQLEndTran (SQL_HANDLE_ENV, mDB->hEnv, SQL_COMMIT);

  if (ret != SQL_SUCCESS) 
  {
    setLastError (qMakeError ("Unable to commit transaction", 
                              QSqlError::Transaction, mDB));
    return false;
  }

  return endTrans();
}

bool QSAPDBDriver::rollbackTransaction()
{
  SQLRETURN    ret;

  if (!isOpen()) 
  {
#ifdef QT_CHECK_RANGE
    qWarning ("QSAPDBDriver::rollbackTransaction: Database not open");
#endif
    return false;
  }

  ret = SQLEndTran (SQL_HANDLE_ENV, mDB->hEnv, SQL_ROLLBACK);

  if (ret != SQL_SUCCESS) 
  {
    setLastError (qMakeError ("Unable to rollback transaction", 
                              QSqlError::Transaction, mDB));
    return false;
  }

  return endTrans();
}

bool QSAPDBDriver::endTrans()
{
  SQLUINTEGER ac = SQL_AUTOCOMMIT_ON;
  SQLRETURN   ret;

  ret = SQLSetConnectAttr (mDB->hDbc, SQL_ATTR_AUTOCOMMIT,
                           (SQLPOINTER)ac, sizeof(ac));

  if (ret != SQL_SUCCESS) 
  {
    setLastError (qMakeError ("Unable to enable autocommit", 
                              QSqlError::Transaction, mDB));
    return false;
  }

  return true;
}

QStringList QSAPDBDriver::tables (const QString & user) const
{
  SQLHANDLE    hStmt;
  SQLRETURN    ret;
  QStringList  tableList;
  SQLINTEGER   length = 0;
  bool         isNull;
  QString      name;

  if (!isOpen())
    return tableList;

  ret = SQLAllocHandle (SQL_HANDLE_STMT, mDB->hDbc, &hStmt);

  if (ret != SQL_SUCCESS) 
  {
#ifdef QT_CHECK_RANGE
    qSqlWarning( "QSAPDBDriver::tables: Unable to allocate handle", mDB);
#endif
    return tableList;
  }

  ret = SQLSetStmtAttr (hStmt, SQL_ATTR_CURSOR_TYPE,
                        (SQLPOINTER)SQL_CURSOR_FORWARD_ONLY, SQL_IS_UINTEGER);

  // Prevent SQLTables to display all the system tables
  ret = SQLTables (hStmt, NULL, 0, NULL, 0, NULL, 0, (SQLCHAR *)"TABLE", 5);

#ifdef QT_CHECK_RANGE
  if (ret != SQL_SUCCESS)
    qSqlWarning ("QSAPDBDriver::tables Unable to execute table list", mDB);
#endif
  ret = SQLFetchScroll (hStmt, SQL_FETCH_NEXT, 0);
  
  while (ret == SQL_SUCCESS) 
  {
    name = qGetStringData (hStmt, 2, length, isNull);
    tableList.append (name);

    ret = SQLFetchScroll (hStmt, SQL_FETCH_NEXT, 0);
  }

  ret = SQLFreeHandle (SQL_HANDLE_STMT, hStmt);
  if (ret != SQL_SUCCESS)
    qSqlWarning ("QSAPDBDriver: Unable to free statement handle" + 
                 QString::number(ret), mDB);

  return tableList;
}

QSqlIndex QSAPDBDriver::primaryIndex (const QString & tablename) const
{
  typedef QMap<QString,QString> FieldMap;
  FieldMap     map;
  QSqlIndex    index(tablename);
  SQLHANDLE    hStmt;
  SQLRETURN    ret;
  SQLINTEGER   length = 0;
  bool         isNull;
  QString      name;
  
  if (!isOpen())
    return index;

  ret = SQLAllocHandle (SQL_HANDLE_STMT, mDB->hDbc, &hStmt);
  if (ret != SQL_SUCCESS) 
  {
#ifdef QT_CHECK_RANGE
    qSqlWarning ("QSAPDBDriver::primaryIndex: Unable to list primary key", mDB);
#endif
    return index;
  }

  SQLSetStmtAttr (hStmt, SQL_ATTR_CURSOR_TYPE, (SQLPOINTER)SQL_CURSOR_FORWARD_ONLY, 
                  SQL_IS_UINTEGER);

  ret = SQLPrimaryKeys (hStmt, NULL, 0, NULL, 0, (SQLCHAR*)(const char*)tablename, 
                        tablename.length());

#ifdef QT_CHECK_RANGE
  if (ret != SQL_SUCCESS)
    qSqlWarning ("QSAPDBDriver::primaryIndex: Unable to execute primary key list", 
                 mDB);
#endif
  ret = SQLFetchScroll (hStmt, SQL_FETCH_NEXT, 0);
  // Store all fields in a StringList because some drivers can't detail fields 
  // in this FETCH loop
  while (ret == SQL_SUCCESS) 
  {
    QString name = qGetStringData (hStmt, 3, length, isNull);
    map[name]    = qGetStringData (hStmt, 5, length, isNull);

    ret = SQLFetchScroll (hStmt, SQL_FETCH_NEXT, 0);
  }

  FieldMap::Iterator i;
  for (i = map.begin();  i != map.end(); i++) 
  {
    QSqlField field = qMakeField (mDB, tablename, i.key());
    index.append (field);
    index.setName(i.data());
  }

  ret = SQLFreeHandle (SQL_HANDLE_STMT, hStmt);
  if (ret != SQL_SUCCESS)
    qSqlWarning ("QSAPDBDriver: Unable to free statement handle" + 
                 QString::number(ret), mDB);
  return index;
}

QSqlRecord QSAPDBDriver::record (const QString & tablename) const
{
  QSqlRecord  record;
  SQLHANDLE   hStmt;
  SQLRETURN   ret;
  bool        isNull;
  SQLINTEGER  length = 0;
  QString     name;
  int         type;
  
  if (!isOpen())
    return record;

  ret = SQLAllocHandle (SQL_HANDLE_STMT, mDB->hDbc, &hStmt);
  if (ret != SQL_SUCCESS) 
  {
#ifdef QT_CHECK_RANGE
    qSqlWarning ("QSAPDBDriver::record: Unable to allocate handle", mDB);
#endif
    return record;
  }
  
  SQLSetStmtAttr (hStmt, SQL_ATTR_CURSOR_TYPE, (SQLPOINTER)SQL_CURSOR_FORWARD_ONLY, 
                  SQL_IS_UINTEGER);
  
  ret = SQLColumns (hStmt, NULL, 0, NULL, 0, (SQLCHAR*)(const char*)tablename,
                    tablename.length(), NULL, 0);

#ifdef QT_CHECK_RANGE
  if (ret != SQL_SUCCESS)
    qSqlWarning ("QSAPDBDriver::record: Unable to execute column list", mDB);
#endif
  ret = SQLFetchScroll (hStmt, SQL_FETCH_NEXT, 0);

  while (ret == SQL_SUCCESS) 
  {
    name = qGetStringData (hStmt, 3, length, isNull);
    type = qGetIntData    (hStmt, 4, isNull);

    record.append (QSqlField (name, qDecodeSAPDBType (type)));

    ret = SQLFetchScroll (hStmt, SQL_FETCH_NEXT, 0);
  }

  ret = SQLFreeHandle (SQL_HANDLE_STMT, hStmt);
  if (ret != SQL_SUCCESS)
    qSqlWarning ("QSAPDBDriver: Unable to free statement handle" + 
                 QString::number(ret), mDB);

  return record;
}

QSqlRecord QSAPDBDriver::record (const QSqlQuery & query) const
{
  QSqlRecord    record;
  QSqlField     field;
  QSAPDBResult* result;
  SQLRETURN     ret;
  SQLSMALLINT   count;

  if (!isOpen())
    return record;

  if (query.isActive() && query.driver() == this) 
  {
    result = (QSAPDBResult*)query.result();

    ret = SQLNumResultCols (result->mDB->hStmt, &count);
#ifdef QT_CHECK_RANGE
    if (ret != SQL_SUCCESS)
      qSqlWarning ("QSAPDBDriver::record: Unable to count result columns", mDB);
#endif
    if (count > 0 && ret == SQL_SUCCESS) 
    {
      for (int i = 0; i < count; i++) 
      {
        QSqlField field = qMakeField (result->mDB, i);
        record.append (field);
      }
    }
  }

  return record;
}

QSqlRecordInfo QSAPDBDriver::recordInfo (const QString& tablename) const
{
  QSqlRecordInfo info;
  QStringList    fList;
  SQLHANDLE      hStmt;
  SQLRETURN      ret;
  bool           isNull;
  SQLINTEGER     length = 0;
  QString        name;
  int            type;
  int            required;
  int            size;
  int            prec;
  QString        defaultValue;

  if (!isOpen())
    return info;

  ret = SQLAllocHandle (SQL_HANDLE_STMT, mDB->hDbc, &hStmt);
  if (ret != SQL_SUCCESS) 
  {
#ifdef QT_CHECK_RANGE
    qSqlWarning ("QSAPDBDriver::record: Unable to allocate handle", mDB);
#endif
    return info;
  }

  SQLSetStmtAttr (hStmt, SQL_ATTR_CURSOR_TYPE, (SQLPOINTER)SQL_CURSOR_FORWARD_ONLY, 
                  SQL_IS_UINTEGER);

  ret = SQLColumns (hStmt, NULL, 0, NULL, 0, (SQLCHAR*)(const char*)tablename,
                    tablename.length(), NULL, 0);
#ifdef QT_CHECK_RANGE
  if (ret != SQL_SUCCESS)
    qSqlWarning ("QSAPDBDriver::record: Unable to execute column list", mDB);
#endif

  ret = SQLFetchScroll (hStmt, SQL_FETCH_NEXT, 0);

  while (ret == SQL_SUCCESS) 
  {
    name     = qGetStringData (hStmt, 3,  length, isNull);
    type     = qGetIntData    (hStmt, 4,  isNull);
    required = qGetIntData    (hStmt, 10, isNull);
    // required can be SQL_NO_NULLS, SQL_NULLABLE or SQL_NULLABLE_UNKNOWN

    if (required == SQL_NO_NULLS) 
      required = 1;
    else if (required == SQL_NULLABLE) 
      required = 0;
    else 
      required = -1;

    size = qGetIntData (hStmt, 6, isNull);
    prec = qGetIntData (hStmt, 8, isNull);

    defaultValue = qGetStringData (hStmt, 12, length, isNull);
    info.append (QSqlFieldInfo (name, qDecodeSAPDBType(type), required, size, prec,
                                defaultValue, type));

    ret = SQLFetchScroll (hStmt, SQL_FETCH_NEXT, 0);
  }

  ret = SQLFreeHandle (SQL_HANDLE_STMT, hStmt);
  if (ret != SQL_SUCCESS)
    qSqlWarning ("QSAPDBDriver: Unable to free statement handle" + 
                 QString::number(ret), mDB);

  return info;
}

QSqlRecordInfo QSAPDBDriver::recordInfo (const QSqlQuery & query) const
{
  QSqlRecordInfo info;
  QSAPDBResult*  result;
  SQLRETURN      ret;
  SQLSMALLINT    count;
  QSqlFieldInfo  field;
  
  if (!isOpen())
    return info;

  if (query.isActive() && query.driver() == this) 
  {
    result = (QSAPDBResult*)query.result();
    ret    = SQLNumResultCols (result->mDB->hStmt, &count);
#ifdef QT_CHECK_RANGE
    if (ret != SQL_SUCCESS)
      qSqlWarning ("QSAPDBDriver::record: Unable to count result columns", mDB);
#endif
    if (count > 0 && ret == SQL_SUCCESS) 
    {
      for (int i = 0; i < count; i++) 
      {
        field = qMakeFieldInfo (result->mDB, i);
        info.append (field);
      }
    }
  }
  
  return info;
}

SQLHANDLE QSAPDBDriver::environment()
{
  return mDB->hEnv;
}

SQLHANDLE QSAPDBDriver::connection()
{
  return mDB->hDbc;
}

QString QSAPDBDriver::formatValue (const QSqlField* field, bool trimStrings) const
{
  QString result;
  QDate   date;
  QTime   time;
  
  if (field->isNull())
    result = nullText();
  else if (field->type() == QVariant::DateTime) 
  {
    // Use an escape sequence for the datetime fields
    if (field->value().toDateTime().isValid())
    {
      date = field->value().toDateTime().date();
      time = field->value().toDateTime().time();
      // Dateformat has to be "yyyy-MM-dd hh:mm:ss", 
      // with leading zeroes if month or day < 10
      result = "{ ts '" + QString::number (date.year()) + "-"
               + QString::number (date.month()).rightJustify (2, '0', true) + "-"
               + QString::number (date.day()).rightJustify (2, '0', true) + " " 
               + time.toString() + "' }";
    } 
    else
      result = nullText();
  } 
  else 
    result = QSqlDriver::formatValue (field, trimStrings);

  return result;
}
