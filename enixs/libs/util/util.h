//#############################################################################
//#  util.h
//#  ======
//#
//#  Creation:       Wed May 15 2002
//#  Author:         Ralf Schneider  (ralf.schneider@b-connected.de)
//#  Copyright:      (C) 2002 by B-connected Linux Solutions GmbH
//#
//#  ------------------------------------------------------------------------
//#
//#  This file contains the prototypes of some utility functions independant
//#  of any class.
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

//------------------------------------------------------------------------------
//  QT Includes
//------------------------------------------------------------------------------
#include <qstring.h>
#include <qdatetime.h>
#include <qlistview.h>
#include <qlist.h>
#include <qdir.h>
#include <qregexp.h>
#include <qcombobox.h>
#include <qlabel.h>
#include <qmessagebox.h>
#include <qasciidict.h>


//------------------------------------------------------------------------------
//  Makros
//------------------------------------------------------------------------------
#define FREE_PTR(p)       		if (p) { delete p; p = 0; }

#define SHOW_DB_ERROR(text, query)	QMessageBox::critical (this, \
                                      QObject::tr("ODBC Error"), text + ":\n\n" + \
                                      query.lastError().databaseText() + \
                                      QObject::tr("\n\nFile: ") + __FILE__ + \
                                      QObject::tr("\nLine: ") + \
                                      QString::number (__LINE__) + \
                                      QObject::tr("\nSQL: ") + query.lastQuery(), \
                                      QMessageBox::Ok, QMessageBox::NoButton);

#define DB_INT(value)				(value.isEmpty() ? QString("NULL") : value)

//------------------------------------------------------------------------------
//  Function declarations
//------------------------------------------------------------------------------
QString formatDate      	(QString datetime);
QString formatTime      	(QString datetime);
QString formatDateTime  	(QString datetime);
QString textToDB      		(QString text);
QString dateToDB      		(QString date);
QString currentDate     	();
QString currencyToDB    	(QString date);

QString createTempFilename 	(QString suffix);
void    viewFile       		(QString filename);
void    deleteFile     		(QString filename);
void    printFile     		(QString filename);
void    deleteTempFiles		();

bool 	insertSelection 	(QListView *listview, QList<QListViewItem> & list,
                             QList<QListViewItem> selection, bool sort=true,
                             int index=2);
void 	removeSelection 	(QListView *listview, QList<QListViewItem> & list,
                             QList<QListViewItem> selection);
void 	loadCategories  	(QAsciiDict<QString> &categories, QString user);

#endif 

