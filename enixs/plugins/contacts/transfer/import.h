//#############################################################################
//#  import.h
//#  ========
//#
//#  Creation:       Thu Jul 25 22:19:30 CEST 2002
//#  Author:         Ralf Schneider  (ralf.schneider@b-connected.de)
//#  Copyright:      (C) 2002 by B-connected Linux Solutions GmbH
//#
//#  ------------------------------------------------------------------------
//#
//#  This file contains the definition of the class CImport.
//#  This class defines the dialog for importing data.
//#
//#############################################################################

#ifndef IMPORT_H
#define IMPORT_H

//=============================================================================
// Include files for QT.
//=============================================================================
#include <qwizard.h>
#include <qlabel.h>
#include <qvbox.h>
#include <qradiobutton.h>
#include <qcheckbox.h>
#include <qbuttongroup.h>

//=============================================================================
// Application specific includes.
//=============================================================================


#define WIZARD_WIDTH  	450

class CImport : public QWizard
{
  Q_OBJECT

public:
  CImport (QWidget *parent = 0, const char *name=0);
  ~CImport();

protected:
  void setupPage1();
  void setupPage2();
  void setupPage3();
  void setupPage4();

protected slots:
  void slotPageChanged  (const QString &title);
  void slotShowHelp     ();
 
private:
  QPixmap*          mImage;
  
  QFrame*			mPage1;
  
  QFrame*			mPage2;
  QButtonGroup*     mGroup;
  QRadioButton*		mVCard;
  QRadioButton*     mCSV;
  QRadioButton*     mOutlook;

  QFrame*			mPage3;
  QLabel*			mAddressL;
  
  QFrame*			mPage4;
  QLabel*			mIncidentsIntroL;
};

#endif
