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


QSAPDBResult::QSAPDBResult (const QSAPDBDriver* driver, QSAPDBHandles* db)
    : QSqlResult ((const QSqlDriver *)driver)
{
  mDB = new QSAPDBHandles();
  (*mDB) = (*db);
}

QSAPDBResult::~QSAPDBResult()
{
  SQLRETURN   ret;
  
  if (mDB->hStmt && driver()->isOpen())
  {
    ret = SQLFreeHandle (SQL_HANDLE_STMT, mDB->hStmt);
#ifdef QT_CHECK_RANGE
    if (ret != SQL_SUCCESS)
      qSqlWarning ("QSAPDBDriver: Unable to free statement handle" + 
                   QString::number(ret), mDB);
#endif
  }

  delete mDB;
}

bool QSAPDBResult::reset (const QString & query)
{
  SQLRETURN    ret;
  SQLSMALLINT  count;

  setActive (false);
  setAt     (QSql::BeforeFirst);

  // If a statement handle exists - reuse it
  if (mDB->hStmt) 
  {
    ret = SQLFreeStmt (mDB->hStmt, SQL_CLOSE);
    if (ret != SQL_SUCCESS) 
    {
#ifdef QT_CHECK_RANGE
      qSqlWarning( "QSAPDBResult::reset: Unable to close statement", mDB);
#endif
      return false;
    }
  } 
  else 
  {
    ret = SQLAllocHandle (SQL_HANDLE_STMT, mDB->hDbc, &mDB->hStmt);

    if (ret != SQL_SUCCESS) 
    {
#ifdef QT_CHECK_RANGE
      qSqlWarning ("QSAPDBResult::reset: Unable to allocate statement handle", mDB);
#endif
      return false;
    }
    
    ret = SQLSetStmtAttr (mDB->hStmt, SQL_ATTR_CURSOR_TYPE,
                          (SQLPOINTER)SQL_CURSOR_STATIC, SQL_IS_UINTEGER);

    if (ret != SQL_SUCCESS) 
    {
#ifdef QT_CHECK_RANGE
      qSqlWarning ("QSAPDBResult::reset: Unable to set statement attribute", mDB);
#endif
      return false;
    }
  }
  
  ret = SQLExecDirect (mDB->hStmt, (SQLCHAR*)(const char*)query, SQL_NTS);
  
  if (ret != SQL_SUCCESS) 
  {
    setLastError (qMakeError ("Unable to execute statement", 
                              QSqlError::Statement, mDB));
    return false;
  }

  ret = SQLNumResultCols (mDB->hStmt, &count);
  setSelect (count != 0);
  setActive (true);
  return true;
}

bool QSAPDBResult::fetch (int i)
{
  SQLRETURN ret;
  int       index;
  
  if (i == at())
    return true;

  mFieldCache.clear();
  mNullCache.clear();

  index = i + 1;
  if (index <= 0) 
  {
    setAt (QSql::BeforeFirst);
    return false;
  }

  ret = SQLFetchScroll (mDB->hStmt, SQL_FETCH_ABSOLUTE, index);
  if (ret != SQL_SUCCESS)
    return false;

  setAt (i);

  return true;
}

bool QSAPDBResult::fetchFirst()
{
  SQLRETURN ret;

  mFieldCache.clear();
  mNullCache.clear();

  ret = SQLFetchScroll (mDB->hStmt, SQL_FETCH_FIRST, 0);
  if (ret != SQL_SUCCESS)
    return false;

  setAt (0);
  
  return true;
}

bool QSAPDBResult::fetchLast()
{
  SQLRETURN   ret;
  SQLINTEGER  row;

  mFieldCache.clear();
  mNullCache.clear();

  ret = SQLFetchScroll (mDB->hStmt, SQL_FETCH_LAST, 0);
  if (ret != SQL_SUCCESS) 
    return false;

  ret = SQLGetStmtAttr (mDB->hStmt, SQL_ROW_NUMBER, &row, SQL_IS_INTEGER, 0);
  if (ret != SQL_SUCCESS)
    return false;

  setAt (row - 1);
  
  return true;
}

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
  QString           fieldValue;
  
  if (mFieldCache.contains (field))
    return mFieldCache[field];

  current = mFieldCache.count();

  for (; current < (field + 1); current++) 
  {
    QSqlField info = qMakeField (mDB, current);

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
          mFieldCache[current] = QVariant (QDateTime (QDate (datetimeBuffer.year, 
                                                             datetimeBuffer.month, 
                                                             datetimeBuffer.day), 
                                                      QTime (datetimeBuffer.hour, 
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

int QSAPDBResult::size()
{
  int        size = -1;
  int        at   = 0;
  SQLINTEGER row  = 0;
  SQLRETURN  ret;
  
  ret = SQLGetStmtAttr (mDB->hStmt, SQL_ROW_NUMBER, &row, SQL_IS_INTEGER, 0);
  at  = row;

  ret = SQLFetchScroll (mDB->hStmt, SQL_FETCH_LAST, 0);
  if (ret == SQL_SUCCESS) 
  {
    ret = SQLGetStmtAttr (mDB->hStmt, SQL_ROW_NUMBER, &row, SQL_IS_INTEGER, 0);
    if (ret == SQL_SUCCESS)
      size = row;

    ret = SQLFetchScroll (mDB->hStmt, SQL_FETCH_ABSOLUTE, row);
    if (ret != SQL_SUCCESS)
      qSqlWarning("QSAPDBResult::size: Unable to restore position", mDB);
  }

  return size;
}

int QSAPDBResult::numRowsAffected()
{
  SQLINTEGER affectedRowCount = 0;
  SQLRETURN  ret;
  
  ret = SQLRowCount (mDB->hStmt, &affectedRowCount);
  if (ret == SQL_SUCCESS)
    return affectedRowCount;
#ifdef QT_CHECK_RANGE
  else
    qSqlWarning ("QSAPDBResult::numRowsAffected: Unable to count affected rows",
                 mDB);
#endif
  return -1;
}

