//#############################################################################
//#  util.cpp
//#  ========
//#
//#  Creation:       Wed May 15 2002
//#  Author:         Ralf Schneider  (ralf.schneider@b-connected.de)
//#  Copyright:      (C) 2002 by B-connected Linux Solutions GmbH
//#
//#  ------------------------------------------------------------------------
//#
//#  This file contains the implementation of some utility functions
//#  independant of any class.
//#
//#  ------------------------------------------------------------------------
//#
//#  This program is free software; you can redistribute it and/or modify it
//#  under the terms of the GNU General Public License as published by the
//#  Free Software Foundation; either version 2 of the License, or (at your
//#  option) any later version.
//#
//#############################################################################

#include <qsqlquery.h>
#include "util.h"

//==============================================================================
// Converts the given String to a valid date string depending on the current
// locale.
//==============================================================================
QString formatDate (QString datetime)
{
  int      day, month, year;
  QDate*   date = new QDate();

  year  = datetime.left (4).toInt();
  month = datetime.mid  (5, 2).toInt();
  day	= datetime.mid  (8, 2).toInt();

  switch (month)
  {
    case 1:
    case 3:
    case 5:
    case 7:
    case 8:
    case 10:
    case 12:
      if ((day < 1) || (day > 31))
        return datetime;
      break;
    case 4:
    case 6:
    case 9:
    case 11:
      if ((day < 1) || (day > 30))
        return datetime;
      break;
    case 2:
      if ((day < 1) || (day > 29))
        return datetime;
      break;
    default:
      return datetime;
  }
  
  date->setYMD (year, month, day);
  return date->toString (Qt::LocalDate);
}

//==============================================================================
// Converts the given String to a valid time string depending on the current
// locale.
//==============================================================================
QString formatTime (QString datetime)
{
  int      hour, minute, second;
  QTime*   time = new QTime();

  hour   = datetime.mid  (11, 2).toInt();
  minute = datetime.mid  (14, 2).toInt();
  second = datetime.mid  (17, 2).toInt();

  if ((hour < 0) || (hour > 23))
    return datetime;

  if ((minute < 0) || (minute > 59))
    return datetime;

  if ((second < 0) || (second > 59))
    return datetime;
  
  time->setHMS (hour, minute, second);
  return time->toString (Qt::LocalDate);
}

//==============================================================================
// Converts the given String to a valid date and time string depending on the
// current locale.
//==============================================================================
QString formatDateTime (QString datetime)
{
  QString  date, time, s;
  
  if ((date = formatDate (datetime)) == datetime)
    return datetime;

  if ((time = formatTime (datetime)) == datetime)
    return datetime;

  s = date + QObject::tr("  um  ") + time;
  return s;
}

//==============================================================================
// Mask all the "invalid" characters in the given string.
//==============================================================================
QString textToDB (QString text)
{
  text.replace (QRegExp ("'"), "''");

  return text.latin1();
}

//==============================================================================
// Converts the given date string in the format YYYY-MM-DD.
//==============================================================================
QString dateToDB (QString date)
{
  int     pos1, pos2;
  QString day, month, year, currentYear, currentMonth;

  currentYear  = QString::number(QDate::currentDate().year()).rightJustify (2, '0');
  currentMonth = QString::number(QDate::currentDate().month()).rightJustify(2, '0');
	
  switch (date.contains ('.'))
  {
    case 0:
      day   = date.rightJustify (2, '0');
      month = currentMonth;
      year  = currentYear;
      break;

    case 1:
      pos1 = date.find ('.');

      day   = date.left(pos1).rightJustify (2, '0');
      if (pos1 == (int)date.length() - 1)
        month = currentMonth;
      else
        month = date.mid (pos1 + 1).rightJustify (2, '0');
      year  = currentYear;
      break;

    case 2:
      pos1 = date.find ('.');
      pos2 = date.findRev ('.');

      day   = date.left(pos1).rightJustify (2, '0');
      month = date.mid (pos1 + 1, pos2 - pos1 - 1).rightJustify (2, '0');
      year  = currentYear.left(4 - date.mid(pos2 + 1).length()) + date.mid(pos2+1);
      break;

    default:
      return "";
  }

  return year + "-" + month + "-" + day;
}

//==============================================================================
// Remove dots from the given currency value.
//==============================================================================
QString currencyToDB (QString value)
{
  value.replace (QRegExp("\\."), "");
  value.replace (QRegExp(","), ".");

  return DB_INT(value);
}

//==============================================================================
// Converts the current date to a valid date string depending on the current
// locale.
//==============================================================================
QString currentDate ()
{
  return QDate::currentDate().toString (Qt::LocalDate);
}

//==============================================================================
// Generate a unique filename for a temporary file.
//==============================================================================
QString createTempFilename (QString suffix)
{
  QDateTime  datetime = QDateTime::currentDateTime();
  QDate			 date = datetime.date();
  QTime			 time = datetime.time();

  QString file = "/tmp/enixs" 
                 + QString::number (date.year()) + QString::number (date.month()) 
                 + QString::number (date.day()) + QString::number (time.hour())
				 + QString::number (time.minute()) + QString::number (time.second())
				 + QString::number (time.msec()) + "." + suffix;

  return file;
}

//==============================================================================
// Delete the given file.
//==============================================================================
void deleteFile (QString filename)
{
  QDir d;
  d.remove (filename);
}

//==============================================================================
// View the given file.
//==============================================================================
void viewFile (QString filename)
{
//	KURL url (filename);
//	new KRun (url);
}

//==============================================================================
// Print the given file.
//==============================================================================
void printFile (QString filename)
{
// 	KPrinter printer;
// 	if (printer.setup ())
//	 	printer.printFiles (QStringList (filename));
}

//==============================================================================
// Delete all temporary files created since program start.
//==============================================================================
void deleteTempFiles ()
{
//  int	i;
//
//  for (i = 0; i < (int)tempFiles.count(); i++)
//    deleteFile (tempFiles[i]);
//  
//  tempFiles.clear();
}

//==============================================================================
// Insert the given list of QListViewItems to the given listview and check if
// each item is already in the listview.
//==============================================================================
bool insertSelection (QListView *listview, QList<QListViewItem> & list,
                      QList<QListViewItem> selection, bool sort, int index)
{
  QListViewItem*   	item;
  bool              error, changed = false;

  for (item = selection.first(); item; item = selection.next())
  {
    error = false;

    //--------------------------------------------------------------------------
    // Check if the item is already in the list.
    //--------------------------------------------------------------------------
    QListViewItemIterator it (listview);
    for ( ; it.current(); ++it)
  	{
      if (it.current()->text(index) == item->text(index))
      {
        QMessageBox::warning ((QWidget *)0, QObject::tr ("Fehler"),
                      QObject::tr ("Der ausgewählte Eintrag \"") + item->text(0) +
                      QObject::tr ("\" ist bereits in der Liste vorhanden."),
                      QMessageBox::Ok | QMessageBox::Default,
					  QMessageBox::NoButton, QMessageBox::NoButton);
        error = true;
        break;
      }
    }

    //--------------------------------------------------------------------------
    // Insert the current item to the listview if no error occured.
    //--------------------------------------------------------------------------
    if (!error)
    {
      listview->insertItem (item);
      list.append          (item);
      changed = true;
    }
  }

  if (sort)
    listview->sort();

  return changed;
}

//==============================================================================
// Remove the given list of QListViewItems from the given listview.
//==============================================================================
void removeSelection (QListView *listview, QList<QListViewItem> & list,
                      QList<QListViewItem> selection)
{
  QListViewItem*   	item;
  
  for (item = selection.first(); item; item = selection.next())
  {
    listview->takeItem  (item);
    list.append      		(item);
  }
}
