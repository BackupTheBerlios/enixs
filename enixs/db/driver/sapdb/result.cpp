//#############################################################################
//#  result.cpp
//#  ==========
//#
//#  Creation:       Mon Jul 08 17:39:16 CEST 2002
//#  Author:         Ralf Schneider  (ralf.schneider@b-connected.de)
//#  Copyright:      (C) 2002 by B-connected Linux Solutions GmbH
//#
//#  ------------------------------------------------------------------------
//#
//#  This file contains the implementation of the class QSAPDBResult.
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
#include "result.h"
#include "util.h"


#define SAPDB_CHECK_DRIVER


//=============================================================================
// Constructor of the result class.
//=============================================================================
QSAPDBResult::QSAPDBResult (const QSAPDBDriver* driver, QSAPDBHandles* db)
    : QSqlResult ((const QSqlDriver *)driver)
{
  mDB = new QSAPDBHandles();
  (*mDB) = (*db);
}

//=============================================================================
// Destructor of the result class.
//=============================================================================
QSAPDBResult::~QSAPDBResult()
{
  if (mDB->hStmt && driver()->isOpen())
  {
    if (SQLFreeHandle (SQL_HANDLE_STMT, mDB->hStmt) != SQL_SUCCESS)
      qSqlWarning ("QSAPDBDriver: Unable to free statement handle", mDB);
  }

  delete mDB;
}

//=============================================================================
// Reset the result and send the query to the database.
//=============================================================================
bool QSAPDBResult::reset (const QString & query)
{
  SQLSMALLINT  count;

  setActive (false);
  setAt     (QSql::BeforeFirst);

//  debug (query);
  if (mDB->hStmt) 
  {
    //--------------------------------------------------------------------------
    //  Close the statement so that it can be reused.
    //--------------------------------------------------------------------------
    if (SQLFreeStmt (mDB->hStmt, SQL_CLOSE) != SQL_SUCCESS) 
    {
      qSqlWarning( "QSAPDBResult::reset: Unable to close statement", mDB);
      return false;
    }
  } 
  else 
  {
    //--------------------------------------------------------------------------
    //  Allocate a statement handle.
    //--------------------------------------------------------------------------
    if (SQLAllocHandle (SQL_HANDLE_STMT, mDB->hDbc, &mDB->hStmt) != SQL_SUCCESS) 
    {
      qSqlWarning ("QSAPDBResult::reset: Unable to allocate statement handle", mDB);
      return false;
    }
    
    //--------------------------------------------------------------------------
    //  Set the cursor type to Static.
    //--------------------------------------------------------------------------
    if (SQLSetStmtAttr (mDB->hStmt, SQL_ATTR_CURSOR_TYPE,
                        (SQLPOINTER)SQL_CURSOR_STATIC, SQL_IS_UINTEGER) 
        != SQL_SUCCESS) 
    {
      qSqlWarning ("QSAPDBResult::reset: Unable to set statement attribute", mDB);
      return false;
    }
  }
  
  //----------------------------------------------------------------------------
  //  Execute the statement.
  //----------------------------------------------------------------------------
  if (SQLExecDirect (mDB->hStmt, (SQLCHAR*)(const char*)query, SQL_NTS) 
      != SQL_SUCCESS) 
  {
    setLastError (qMakeError ("Unable to execute statement", 
                              QSqlError::Statement, mDB));
    return false;
  }

  //----------------------------------------------------------------------------
  //  Read the number of columns in the result set.
  //----------------------------------------------------------------------------
  SQLNumResultCols (mDB->hStmt, &count);

  setSelect (count != 0);
  setActive (true);
  return true;
}

//=============================================================================
// Fetch one record at a given position.
//=============================================================================
bool QSAPDBResult::fetch (int i)
{
  int   index;
  
  //----------------------------------------------------------------------------
  //  If the cursor is already at the given position --> nothing has to be done.
  //----------------------------------------------------------------------------
  if (i == at())
    return true;

  mFieldCache.clear();
  mNullCache.clear();

  //----------------------------------------------------------------------------
  //  Position the cursor.
  //----------------------------------------------------------------------------
  index = i + 1;
  if (index <= 0) 
  {
    setAt (QSql::BeforeFirst);
    return false;
  }

  //----------------------------------------------------------------------------
  //  Fetch the record and set the new cursor position.
  //----------------------------------------------------------------------------
  if (SQLFetchScroll (mDB->hStmt, SQL_FETCH_ABSOLUTE, index) != SQL_SUCCESS)
    return false;

  setAt (i);

  return true;
}

//=============================================================================
// Fetch the first record.
//=============================================================================
bool QSAPDBResult::fetchFirst()
{
  mFieldCache.clear();
  mNullCache.clear();

  if (SQLFetchScroll (mDB->hStmt, SQL_FETCH_FIRST, 0) != SQL_SUCCESS)
    return false;

  setAt (0);
  
  return true;
}

//=============================================================================
// Fetch the lsst record.
//=============================================================================
bool QSAPDBResult::fetchLast()
{
  SQLINTEGER  row;

  mFieldCache.clear();
  mNullCache.clear();

  if (SQLFetchScroll (mDB->hStmt, SQL_FETCH_LAST, 0) != SQL_SUCCESS) 
    return false;

  if (SQLGetStmtAttr (mDB->hStmt, SQL_ROW_NUMBER, &row, SQL_IS_INTEGER, 0) 
      != SQL_SUCCESS)
    return false;

  setAt (row - 1);
  
  return true;
}

//=============================================================================
// Return the data of a specified field.
//=============================================================================
QVariant QSAPDBResult::data (int field)
{
  SQLRETURN         ret = 0;
  SQLINTEGER        length = 0;
  bool              isNull  = false;
  int               current;
  SQLDOUBLE         doubleBuffer;
  DATE_STRUCT       dateBuffer;
  TIME_STRUCT       timeBuffer;
  TIMESTAMP_STRUCT  datetimeBuffer;
  SQLCHAR*          byteBuffer;
  QString           fieldValue;
  
  if (mFieldCache.contains (field))
    return mFieldCache[field];

  current = mFieldCache.count();

  for (; current < (field + 1); current++) 
  {
    QSqlField info = qMakeField (mDB, current);

    //--------------------------------------------------------------------------
    //  Distinguish several data types.
    //--------------------------------------------------------------------------
    switch (info.type()) 
    {
      case QVariant::Int:
        isNull = false;
        mFieldCache[current] = QVariant (qGetIntData (mDB->hStmt, current, isNull));
        mNullCache[current]  = isNull;
        break;

      case QVariant::Double:
        ret = SQLGetData (mDB->hStmt, current + 1, SQL_C_DOUBLE,
                          (SQLPOINTER)&doubleBuffer, 0, &length);

        if (   (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO ) 
            && (length != SQL_NULL_DATA )) 
        {
          mFieldCache[current] = QVariant (doubleBuffer);
          mNullCache[current]  = false;
        } 
        else 
        {
          mFieldCache[current] = QVariant();
          mNullCache[current]  = true;
        }
        break;

      case QVariant::Date:
        ret = SQLGetData (mDB->hStmt, current + 1, SQL_C_DATE,
                          (SQLPOINTER)&dateBuffer, 0, &length);

        if (   (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO) 
            && (length != SQL_NULL_DATA)) 
        {
          mFieldCache[current] = QVariant (QDate (dateBuffer.year, 
                                                  dateBuffer.month,
                                                  dateBuffer.day));
          mNullCache[current]  = false;
        } 
        else 
        {
          mFieldCache[current] = QVariant (QDate());
          mNullCache[current]  = true;
        }
        break;

      case QVariant::Time:
        ret = SQLGetData (mDB->hStmt, current + 1, SQL_C_TIME,
                          (SQLPOINTER)&timeBuffer, 0, &length);

        if (   (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO) 
            && (length != SQL_NULL_DATA)) 
        {
          mFieldCache[current] = QVariant (QTime (timeBuffer.hour, 
                                                  timeBuffer.minute, 
                                                  timeBuffer.second));
          mNullCache[current]  = false;
        } 
        else 
        {
          mFieldCache[current] = QVariant (QTime());
          mNullCache[current]  = true;
        }
        break;

      case QVariant::DateTime:
        ret = SQLGetData (mDB->hStmt, current + 1, SQL_C_TIMESTAMP,
                          (SQLPOINTER)&datetimeBuffer, 0, &length);

        if (   (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO) 
            && (length != SQL_NULL_DATA)) 
        {
          mFieldCache[current] = QVariant (QDateTime(QDate(datetimeBuffer.year, 
                                                           datetimeBuffer.month, 
                                                           datetimeBuffer.day), 
                                                     QTime(datetimeBuffer.hour, 
                                                           datetimeBuffer.minute, 
                                                           datetimeBuffer.second)));
          mNullCache[current]  = false;
        } 
        else 
        {
          mFieldCache[current] = QVariant (QDateTime());
          mNullCache[current]  = true;
        }
        break;

      case QVariant::ByteArray:
        isNull               = false;
        mFieldCache[current] = QVariant (qGetBinaryData(mDB->hStmt,current,isNull));
        mNullCache[current]  = isNull;
        break;

      default:
      case QVariant::String:
        isNull     = false;
        fieldValue = qGetStringData (mDB->hStmt, current, length, isNull);

        mFieldCache[current] = QVariant (fieldValue);
        mNullCache[current]  = isNull;
        break;
    }
  }

  return mFieldCache[--current];
}

//=============================================================================
// Check if the field at the given position is NULL.
//=============================================================================
bool QSAPDBResult::isNull (int field)
{
  if (!mFieldCache.contains (field)) 
  {
    // since there is no good way to find out whether the value is NULL
    // without fetching the field we'll fetch it here.
    // (data() also sets the NULL flag)
    data (field);
  }

  return mNullCache[field];
}

//=============================================================================
// Return the size of the result.
//=============================================================================
int QSAPDBResult::size()
{
  int        size = -1;
  SQLINTEGER row  = 0;
  
  SQLGetStmtAttr (mDB->hStmt, SQL_ROW_NUMBER, &row, SQL_IS_INTEGER, 0);

  if (SQLFetchScroll (mDB->hStmt, SQL_FETCH_LAST, 0) == SQL_SUCCESS) 
  {
    if (SQLGetStmtAttr (mDB->hStmt, SQL_ROW_NUMBER, &row, SQL_IS_INTEGER, 0) 
        == SQL_SUCCESS)
      size = row;

    if (SQLFetchScroll (mDB->hStmt, SQL_FETCH_ABSOLUTE, row) != SQL_SUCCESS)
      qSqlWarning("QSAPDBResult::size: Unable to restore position", mDB);
  }

  return size;
}

//=============================================================================
// Return the number of rows affected by the last query.
//=============================================================================
int QSAPDBResult::numRowsAffected()
{
  SQLINTEGER affectedRowCount = 0;
  
  if (SQLRowCount (mDB->hStmt, &affectedRowCount) == SQL_SUCCESS)
    return affectedRowCount;
  else
    qSqlWarning("QSAPDBResult::numRowsAffected: Unable to count affected rows",mDB);

  return -1;
}

