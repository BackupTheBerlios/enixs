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


//=============================================================================
// Constructor of the driver class.
//=============================================================================
QSAPDBDriver::QSAPDBDriver (QObject* parent, const char* name)
    : QSqlDriver (parent, name ? name : "QSAPDB7")
{
  mDB = new QSAPDBHandles();
}

//=============================================================================
// Destructor of the driver class.
//=============================================================================
QSAPDBDriver::~QSAPDBDriver()
{
  close();
  delete mDB;
}

//=============================================================================
// Check if the driver supports a requested feature.
//=============================================================================
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
        return true;

    case QuerySize:
      return true;
      
    case BLOB:
      return true;

    default:
      return false;
  }
}

//=============================================================================
// Open a new connection to the database. The isolation level is set to 
// ReadUncommitted.
//=============================================================================
bool QSAPDBDriver::open (const QString & db, const QString & user,
                         const QString & password, const QString & host, int port)
{
  SQLRETURN    ret;
  QString      connectString;
  SQLCHAR      outString[1024];
  SWORD        outLen;
  SQLUINTEGER  isolevel = SQL_TXN_READ_UNCOMMITTED;
  
  //----------------------------------------------------------------------------
  //  If the database connection is already open --> close it.
  //----------------------------------------------------------------------------
  if (isOpen())
    close();

  //----------------------------------------------------------------------------
  //  Allocate an environment handle.
  //----------------------------------------------------------------------------
  ret = SQLAllocHandle (SQL_HANDLE_ENV, SQL_NULL_HANDLE, &mDB->hEnv);
  if ((ret != SQL_SUCCESS) && (ret != SQL_SUCCESS_WITH_INFO)) 
  {
    qSqlWarning  ("QSAPDBDriver::open: Unable to allocate environment", mDB);
    setOpenError (true);
    return false;
  }

  //----------------------------------------------------------------------------
  //  Enable ODBC Version 3.
  //----------------------------------------------------------------------------
  SQLSetEnvAttr (mDB->hEnv, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, 
                 SQL_IS_UINTEGER);

  //----------------------------------------------------------------------------
  //  Allocate a connection handle.
  //----------------------------------------------------------------------------
  if (SQLAllocHandle (SQL_HANDLE_DBC, mDB->hEnv, &mDB->hDbc) != SQL_SUCCESS) 
  {
    qSqlWarning  ("QSAPDBDriver::open: Unable to allocate connection", mDB);
    setOpenError (true);
    return false;
  }

  //----------------------------------------------------------------------------
  //  Set the isolation level to ReadUncommited.
  //----------------------------------------------------------------------------
  if (SQLSetConnectAttr (mDB->hDbc, SQL_ATTR_TXN_ISOLATION, (SQLPOINTER)isolevel, 
                         sizeof(isolevel)) 
      != SQL_SUCCESS) 
  {
    setLastError (qMakeError ("Unable to set isolation level", 
                              QSqlError::Transaction, mDB));
    return false;
  }

  //----------------------------------------------------------------------------
  //  Connect to the database.
  //----------------------------------------------------------------------------
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

  //----------------------------------------------------------------------------
  //  Check the features of the driver.
  //----------------------------------------------------------------------------
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

//=============================================================================
// Close the database connection.
//=============================================================================
void QSAPDBDriver::close()
{
  if ((isOpen() || isOpenError()) && (mDB != 0)) 
  {
    if (mDB->hDbc) 
    {
      //------------------------------------------------------------------------
      //  Disconnect from the database. Open statement handles are released 
      //  automatically.
      //------------------------------------------------------------------------
      if (SQLDisconnect (mDB->hDbc) != SQL_SUCCESS)
        qSqlWarning ("QSAPDBDriver::close: Unable to disconnect", mDB);

      //------------------------------------------------------------------------
      //  Release the connection handle.
      //------------------------------------------------------------------------
      if (SQLFreeHandle (SQL_HANDLE_DBC, mDB->hDbc) != SQL_SUCCESS)
        qSqlWarning ("QSAPDBDriver::close: Unable to free connection handle", mDB);

      mDB->hDbc = 0;
    }

    if (mDB->hEnv) 
    {
      //------------------------------------------------------------------------
      //  Release the environment handle.
      //------------------------------------------------------------------------
      if (SQLFreeHandle (SQL_HANDLE_ENV, mDB->hEnv) != SQL_SUCCESS)
        qSqlWarning("QSAPDBDriver::close: Unable to free environment handle", mDB);

      mDB->hEnv = 0;
    }
  }

  setOpen      (false);
  setOpenError (false);
}

//=============================================================================
// Create a new query object.
//=============================================================================
QSqlQuery QSAPDBDriver::createQuery() const
{
  return QSqlQuery (new QSAPDBResult (this, mDB));
}

//=============================================================================
// Start a new transaction. This implicitly disables the autocommit mode.
//=============================================================================
bool QSAPDBDriver::beginTransaction()
{
  SQLUINTEGER  autocommit = SQL_AUTOCOMMIT_OFF;

  //----------------------------------------------------------------------------
  //  Check if the database connection is open.
  //----------------------------------------------------------------------------
  if (!isOpen()) 
  {
#ifdef QT_CHECK_RANGE
    qWarning ("QSAPDBDriver::beginTransaction: Database not open");
#endif
    return false;
  }

  //----------------------------------------------------------------------------
  //  Disable autocommit mode.
  //----------------------------------------------------------------------------
  if (SQLSetConnectAttr (mDB->hDbc, SQL_ATTR_AUTOCOMMIT, (SQLPOINTER)autocommit, 
                         sizeof(autocommit)) 
      != SQL_SUCCESS) 
  {
    setLastError (qMakeError ("Unable to disable autocommit", 
                              QSqlError::Transaction, mDB));
    return false;
  }
  return true;
}

//=============================================================================
// Commit the current transaction.
//=============================================================================
bool QSAPDBDriver::commitTransaction()
{
  //----------------------------------------------------------------------------
  //  Check if the database connection is open.
  //----------------------------------------------------------------------------
  if (!isOpen()) 
  {
#ifdef QT_CHECK_RANGE
    qWarning ("QSAPDBDriver::commitTransaction: Database not open");
#endif
    return false;
  }

  //----------------------------------------------------------------------------
  //  Commit the current transaction.
  //----------------------------------------------------------------------------
  if (SQLEndTran (SQL_HANDLE_ENV, mDB->hEnv, SQL_COMMIT) != SQL_SUCCESS) 
  {
    setLastError (qMakeError ("Unable to commit transaction", 
                              QSqlError::Transaction, mDB));
    return false;
  }

  return true;
}

//=============================================================================
// Rollback the current transaction.
//=============================================================================
bool QSAPDBDriver::rollbackTransaction()
{
  //----------------------------------------------------------------------------
  //  Check if the database connection is open.
  //----------------------------------------------------------------------------
  if (!isOpen()) 
  {
#ifdef QT_CHECK_RANGE
    qWarning ("QSAPDBDriver::rollbackTransaction: Database not open");
#endif
    return false;
  }

  //----------------------------------------------------------------------------
  //  Rollback the current transaction.
  //----------------------------------------------------------------------------
  if (SQLEndTran (SQL_HANDLE_ENV, mDB->hEnv, SQL_ROLLBACK) != SQL_SUCCESS) 
  {
    setLastError (qMakeError ("Unable to rollback transaction", 
                              QSqlError::Transaction, mDB));
    return false;
  }

  return true;
}

//=============================================================================
// Enable the autocommit mode.
//=============================================================================
bool QSAPDBDriver::endTrans()
{
  SQLUINTEGER autocommit = SQL_AUTOCOMMIT_ON;

  if (SQLSetConnectAttr (mDB->hDbc, SQL_ATTR_AUTOCOMMIT, (SQLPOINTER)autocommit, 
                         sizeof(autocommit))
      != SQL_SUCCESS) 
  {
    setLastError (qMakeError ("Unable to enable autocommit", 
                              QSqlError::Transaction, mDB));
    return false;
  }

  return true;
}

//=============================================================================
// Retrieve all the database tables.
//=============================================================================
QStringList QSAPDBDriver::tables (const QString & user) const
{
  SQLHANDLE    hStmt;
  QStringList  tableList;
  SQLINTEGER   length = 0;
  bool         isNull;
  QString      name;

  //----------------------------------------------------------------------------
  //  Check if the database connection is open.
  //----------------------------------------------------------------------------
  if (!isOpen())
    return tableList;

  //----------------------------------------------------------------------------
  //  Allocate a statement handle.
  //----------------------------------------------------------------------------
  if (SQLAllocHandle (SQL_HANDLE_STMT, mDB->hDbc, &hStmt) != SQL_SUCCESS) 
  {
    qSqlWarning( "QSAPDBDriver::tables: Unable to allocate handle", mDB);
    return tableList;
  }

  //----------------------------------------------------------------------------
  //  Set the cursor type to ForwardOnly.
  //----------------------------------------------------------------------------
  SQLSetStmtAttr (hStmt, SQL_ATTR_CURSOR_TYPE, (SQLPOINTER)SQL_CURSOR_FORWARD_ONLY,
                  SQL_IS_UINTEGER);

  //----------------------------------------------------------------------------
  //  Get the tables.
  //----------------------------------------------------------------------------
  if (SQLTables (hStmt, NULL, 0, NULL, 0, NULL, 0, (SQLCHAR *)"TABLE", 5) 
      != SQL_SUCCESS)
    qSqlWarning ("QSAPDBDriver::tables Unable to execute table list", mDB);

  //----------------------------------------------------------------------------
  //  Fetch the result.
  //----------------------------------------------------------------------------
  while (SQLFetchScroll (hStmt, SQL_FETCH_NEXT, 0) == SQL_SUCCESS) 
  {
    name = qGetStringData (hStmt, 2, length, isNull);
    tableList.append (name);
  }

  //----------------------------------------------------------------------------
  //  Release the statement handle.
  //----------------------------------------------------------------------------
  if (SQLFreeHandle (SQL_HANDLE_STMT, hStmt) != SQL_SUCCESS)
    qSqlWarning ("QSAPDBDriver: Unable to free statement handle", mDB);

  return tableList;
}

//=============================================================================
// Retrieve all primary key columns of a given table.
//=============================================================================
QSqlIndex QSAPDBDriver::primaryIndex (const QString & tablename) const
{
  typedef QMap<QString,QString> FieldMap;
  FieldMap     map;
  QSqlIndex    index(tablename);
  SQLHANDLE    hStmt;
  SQLINTEGER   length = 0;
  bool         isNull;
  QString      name;
  
  //----------------------------------------------------------------------------
  //  Check if the database connection is open.
  //----------------------------------------------------------------------------
  if (!isOpen())
    return index;

  //----------------------------------------------------------------------------
  //  Allocate a statement handle.
  //----------------------------------------------------------------------------
  if (SQLAllocHandle (SQL_HANDLE_STMT, mDB->hDbc, &hStmt) != SQL_SUCCESS) 
  {
    qSqlWarning ("QSAPDBDriver::primaryIndex: Unable to list primary key", mDB);
    return index;
  }

  //----------------------------------------------------------------------------
  //  Set the cursor type to ForwardOnly.
  //----------------------------------------------------------------------------
  SQLSetStmtAttr (hStmt, SQL_ATTR_CURSOR_TYPE, (SQLPOINTER)SQL_CURSOR_FORWARD_ONLY, 
                  SQL_IS_UINTEGER);

  //----------------------------------------------------------------------------
  //  Get the primary key columns.
  //----------------------------------------------------------------------------
  if (SQLPrimaryKeys (hStmt, NULL, 0, NULL, 0, (SQLCHAR*)(const char*)tablename, 
                      tablename.length()) 
      != SQL_SUCCESS)
    qSqlWarning ("QSAPDBDriver::primaryIndex: Unable to get primary key list", mDB);

  //----------------------------------------------------------------------------
  //  Fetch the result.
  //----------------------------------------------------------------------------
  while (SQLFetchScroll (hStmt, SQL_FETCH_NEXT, 0) == SQL_SUCCESS) 
  {
    QString name = qGetStringData (hStmt, 3, length, isNull);
    map[name]    = qGetStringData (hStmt, 5, length, isNull);
  }

  FieldMap::Iterator i;
  for (i = map.begin();  i != map.end(); i++) 
  {
    QSqlField field = qMakeField (mDB, tablename, i.key());
    index.append (field);
    index.setName(i.data());
  }

  //----------------------------------------------------------------------------
  //  Release the statement handle.
  //----------------------------------------------------------------------------
  if (SQLFreeHandle (SQL_HANDLE_STMT, hStmt) != SQL_SUCCESS)
    qSqlWarning ("QSAPDBDriver: Unable to free statement handle", mDB);

  return index;
}

//=============================================================================
// Retrieve all column names of a given table.
//=============================================================================
QSqlRecord QSAPDBDriver::record (const QString & tablename) const
{
  QSqlRecord  record;
  SQLHANDLE   hStmt;
  bool        isNull;
  SQLINTEGER  length = 0;
  QString     name;
  int         type;
  
  //----------------------------------------------------------------------------
  //  Check if the database connection is open.
  //----------------------------------------------------------------------------
  if (!isOpen())
    return record;

  //----------------------------------------------------------------------------
  //  Allocate a statement handle.
  //----------------------------------------------------------------------------
  if (SQLAllocHandle (SQL_HANDLE_STMT, mDB->hDbc, &hStmt) != SQL_SUCCESS) 
  {
    qSqlWarning ("QSAPDBDriver::record: Unable to allocate handle", mDB);
    return record;
  }
  
  //----------------------------------------------------------------------------
  //  Set the cursor type to ForwardOnly.
  //----------------------------------------------------------------------------
  SQLSetStmtAttr (hStmt, SQL_ATTR_CURSOR_TYPE, (SQLPOINTER)SQL_CURSOR_FORWARD_ONLY, 
                  SQL_IS_UINTEGER);
  
  //----------------------------------------------------------------------------
  //  Get the column names of the table.
  //----------------------------------------------------------------------------
  if (SQLColumns (hStmt, NULL, 0, NULL, 0, (SQLCHAR*)(const char*)tablename,
                  tablename.length(), NULL, 0) 
      != SQL_SUCCESS)
    qSqlWarning ("QSAPDBDriver::record: Unable to execute column list", mDB);

  //----------------------------------------------------------------------------
  //  Fetch the result.
  //----------------------------------------------------------------------------
  while (SQLFetchScroll (hStmt, SQL_FETCH_NEXT, 0) == SQL_SUCCESS) 
  {
    name = qGetStringData (hStmt, 3, length, isNull);
    type = qGetIntData    (hStmt, 4, isNull);

    record.append (QSqlField (name, qDecodeSAPDBType (type)));
  }

  //----------------------------------------------------------------------------
  //  Release the statement handle.
  //----------------------------------------------------------------------------
  if (SQLFreeHandle (SQL_HANDLE_STMT, hStmt) != SQL_SUCCESS)
    qSqlWarning ("QSAPDBDriver: Unable to free statement handle", mDB);

  return record;
}

//=============================================================================
// Retrieve all column names of a given query.
//=============================================================================
QSqlRecord QSAPDBDriver::record (const QSqlQuery & query) const
{
  QSqlRecord    record;
  QSqlField     field;
  QSAPDBResult* result;
  SQLSMALLINT   count;

  //----------------------------------------------------------------------------
  //  Check if the database connection is open.
  //----------------------------------------------------------------------------
  if (!isOpen())
    return record;

  if (query.isActive() && query.driver() == this) 
  {
    result = (QSAPDBResult*)query.result();

    //--------------------------------------------------------------------------
    //  Get the number of columns in the result of the query.
    //--------------------------------------------------------------------------
    if (SQLNumResultCols (result->mDB->hStmt, &count) != SQL_SUCCESS)
    {
      qSqlWarning ("QSAPDBDriver::record: Unable to count result columns", mDB);
      return record;
    }

    //--------------------------------------------------------------------------
    //  If there is a result set --> read it.
    //--------------------------------------------------------------------------
    if (count > 0) 
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

//=============================================================================
// Retrieve some meta data of the columns of a given table.
//=============================================================================
QSqlRecordInfo QSAPDBDriver::recordInfo (const QString& tablename) const
{
  QSqlRecordInfo info;
  QStringList    fList;
  SQLHANDLE      hStmt;
  bool           isNull;
  SQLINTEGER     length = 0;
  QString        name;
  int            type;
  int            required;
  int            size;
  int            prec;
  QString        defaultValue;

  //----------------------------------------------------------------------------
  //  Check if the database connection is open.
  //----------------------------------------------------------------------------
  if (!isOpen())
    return info;

  //----------------------------------------------------------------------------
  //  Allocate a statement handle.
  //----------------------------------------------------------------------------
  if (SQLAllocHandle (SQL_HANDLE_STMT, mDB->hDbc, &hStmt) != SQL_SUCCESS) 
  {
    qSqlWarning ("QSAPDBDriver::record: Unable to allocate handle", mDB);
    return info;
  }

  //----------------------------------------------------------------------------
  //  Set the cursor type to ForwardOnly.
  //----------------------------------------------------------------------------
  SQLSetStmtAttr (hStmt, SQL_ATTR_CURSOR_TYPE, (SQLPOINTER)SQL_CURSOR_FORWARD_ONLY, 
                  SQL_IS_UINTEGER);

  //----------------------------------------------------------------------------
  //  Get the column names of the table.
  //----------------------------------------------------------------------------
  if (SQLColumns (hStmt, NULL, 0, NULL, 0, (SQLCHAR*)(const char*)tablename,
                  tablename.length(), NULL, 0) 
      != SQL_SUCCESS)
    qSqlWarning ("QSAPDBDriver::record: Unable to execute column list", mDB);

  //----------------------------------------------------------------------------
  //  Fetch the result.
  //----------------------------------------------------------------------------
  while (SQLFetchScroll (hStmt, SQL_FETCH_NEXT, 0) == SQL_SUCCESS) 
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
  }

  //----------------------------------------------------------------------------
  //  Release the statement handle.
  //----------------------------------------------------------------------------
  if (SQLFreeHandle (SQL_HANDLE_STMT, hStmt) != SQL_SUCCESS)
    qSqlWarning ("QSAPDBDriver: Unable to free statement handle", mDB);

  return info;
}

//=============================================================================
// Retrieve some meta data of the columns of a given query.
//=============================================================================
QSqlRecordInfo QSAPDBDriver::recordInfo (const QSqlQuery & query) const
{
  QSqlRecordInfo info;
  QSAPDBResult*  result;
  SQLSMALLINT    count;
  QSqlFieldInfo  field;
  
  //----------------------------------------------------------------------------
  //  Check if the database connection is open.
  //----------------------------------------------------------------------------
  if (!isOpen())
    return info;

  if (query.isActive() && query.driver() == this) 
  {
    result = (QSAPDBResult*)query.result();

    //--------------------------------------------------------------------------
    //  Get the number of columns in the result of the query.
    //--------------------------------------------------------------------------
    if (SQLNumResultCols (result->mDB->hStmt, &count) != SQL_SUCCESS)
    {
      qSqlWarning ("QSAPDBDriver::record: Unable to count result columns", mDB);
      return info;
    }

    //--------------------------------------------------------------------------
    //  If there is a result set --> read it.
    //--------------------------------------------------------------------------
    if (count > 0) 
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

//=============================================================================
// Return the environment handle of the database connection.
//=============================================================================
SQLHANDLE QSAPDBDriver::environment()
{
  return mDB->hEnv;
}

//=============================================================================
// Return the connection handle of the database connection.
//=============================================================================
SQLHANDLE QSAPDBDriver::connection()
{
  return mDB->hDbc;
}

//=============================================================================
// Format the data and time values in a special manner.
//=============================================================================
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
