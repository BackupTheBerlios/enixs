//#############################################################################
//#  main.cpp
//#  ========
//#
//#  Creation:       Die Apr  2 16:25:16 CEST 2002
//#  Author:         Ralf Schneider  (ralf.schneider@b-connected.de)
//#  Copyright:      (C) 2002 by Ralf Schneider
//#
//#  ------------------------------------------------------------------------
//#
//#  This is the main program of eniXs, the enterprise information exchange
//#  system.
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
#include <qapplication.h>
#include <qfont.h>
#include <qstring.h>
#include <qtextcodec.h>
#include <qtranslator.h>

//=============================================================================
// Application specific includes.
//=============================================================================
#include "enixs.h"
#include "userdata.h"
#include <dbconnection.h>
#include <stdlib.h>

void usage ();

CConnection*    mDB;
CUserData*      mCurrentUser;
QString         mLanguage;
QString         mEnixsDir;

//=============================================================================
// Main function of the EniXs application.
//=============================================================================
int main (int argc, char *argv[])
{
  int   i;
  QStringList  languages;
  
  languages << "en" << "de";
  
  //---------------------------------------------------------------------------
  // Parse the arguments.
  //---------------------------------------------------------------------------
  for (i = 1; i < argc; i++)
  {
    if (QString (argv[i]) == "-lang")
    {
      i++;
      if (languages.contains (argv[i]))
        mLanguage = argv[i];
      else
        usage();
    }
    else
      usage();
  }
  
  //---------------------------------------------------------------------------
  // Create the appliction object.
  //---------------------------------------------------------------------------
  QApplication a (argc, argv);
  a.setFont (QFont ("helvetica", 12));
  
  //---------------------------------------------------------------------------
  // Add the paths for the plugins and libs to the library path.
  //---------------------------------------------------------------------------
  mEnixsDir = QString (getenv("ENIXSDIR"));
  a.addLibraryPath (mEnixsDir + "/plugins");
  a.addLibraryPath (mEnixsDir + "/libs");

  //---------------------------------------------------------------------------
  // Install the translators depending on the specified language.
  //---------------------------------------------------------------------------
  QDir translations (mEnixsDir + "/translations", "*_" + mLanguage + ".qm");

  for (uint i = 0; i < translations.count(); i++)
  {
    QTranslator* translator = new QTranslator (0);
    translator->load    (translations[i], translations.path());
    a.installTranslator (translator);
  }

  //---------------------------------------------------------------------------
  // Create a database connection object and a user data object.
  //---------------------------------------------------------------------------
  mDB          = new CConnection();
  mCurrentUser = new CUserData();

  //---------------------------------------------------------------------------
  // Create the main application object and make it the main widget of the
  // application.
  //---------------------------------------------------------------------------
  CEnixsApp *enixs = new CEnixsApp();
  a.setMainWidget (enixs);

  enixs->show();
	
  //---------------------------------------------------------------------------
  // Enter the event loop of the application.
  //---------------------------------------------------------------------------
  return a.exec();
}

//=============================================================================
// Main function of the EniXs application.
//=============================================================================
void usage ()
{
  qWarning ("\n   EniXs - The Enterprise Application Server\n");
  qWarning ("   Usage:   enixs [-lang <language>]\n");
  qWarning ("   Parameters:");
  qWarning ("      -lang <language>   Specify the language environment.");
  qWarning ("                         Valid values are en and de (Default: en).\n");
  exit (1);
}
  
