//#############################################################################
//#  import.cpp
//#  ==========
//#
//#  Creation:       Thu Jul 25 22:19:30 CEST 2002
//#  Author:         Ralf Schneider  (ralf.schneider@b-connected.de)
//#  Copyright:      (C) 2002 by B-connected Linux Solutions GmbH
//#
//#  ------------------------------------------------------------------------
//#
//#  This file contains the implementation of the class CImport.
//#  This class defines the dialog for importing data.
//#
//#############################################################################

//=============================================================================
// Include files for QT.
//=============================================================================
#include <qlabel.h>
#include <qfile.h>
#include <qpushbutton.h>
#include <qimage.h>

//=============================================================================
// Application specific includes.
//=============================================================================
#include "import.h"
#include "../bitmaps/import.xpm"

//=============================================================================
// Constructor of the class CImport.
//=============================================================================
CImport::CImport (QWidget *parent, const char *name)
    : QWizard (parent, name, TRUE)
{
  setCaption   (tr ("EniXs Data Import Wizard"));
  setFixedSize (WIZARD_WIDTH, 300);

  mImage = new QPixmap     (import_image);
  mImage->convertFromImage (mImage->convertToImage().smoothScale (100, 200));
  
  setupPage1();			// Show a short intro message
  setupPage2();			// Selection of the file type
  setupPage3();			// Select the file
  setupPage4();			// Import the data

  finishButton()->setText (tr ("Import"));

  connect (this,         SIGNAL (selected       (const QString &)),
           this,         SLOT   (slotPageChanged(const QString &)));
  connect (helpButton(), SIGNAL (clicked()),
           this,         SLOT   (slotShowHelp()));
}

//=============================================================================
// Constructor of the class CImport.
//=============================================================================
CImport::~CImport()
{
}

//=============================================================================
// Setup the first page (selection of the file type).
//=============================================================================
void CImport::setupPage1()
{
  QLabel   *intro, *panel;
  QFont 	f(font());

  f.setBold      (true);
  f.setPointSize (f.pointSize() + 1);

  //---------------------------------------------------------------------------
  //  Create the frame of the page.
  //---------------------------------------------------------------------------
  mPage1 = new QFrame (this);

  //---------------------------------------------------------------------------
  //  Create the image panel.
  //---------------------------------------------------------------------------
  panel = new QLabel (mPage1);
  panel->setGeometry (10, 10, 100, 200);
  panel->setPixmap   (*mImage);
  
  //---------------------------------------------------------------------------
  //  Create the intro label.
  //---------------------------------------------------------------------------
  intro = new QLabel (tr ("This is the Data Import Wizard.\n\n"
                          "It will guide you step by step through\n"
                          "the process of importing one or more\n"
                          "contacts into the database."), mPage1);
  intro->setGeometry (160, 60, WIZARD_WIDTH - 160, 100);
  intro->setFont     (f);

  addPage (mPage1, tr("Introduction"));
}

//=============================================================================
// Setup the seconnd page (selection of single data fields).
//=============================================================================
void CImport::setupPage2()
{
  QLabel   *intro, *panel;
  QFont 	f(font());

  f.setBold      (true);
  f.setPointSize (f.pointSize() + 1);

  //---------------------------------------------------------------------------
  //  Create the frame of the page.
  //---------------------------------------------------------------------------
  mPage2 = new QFrame (this);

  //---------------------------------------------------------------------------
  //  Create the image panel.
  //---------------------------------------------------------------------------
  panel = new QLabel (mPage2);
  panel->setGeometry (10, 10, 100, 200);
  panel->setPixmap   (*mImage);
  
  //---------------------------------------------------------------------------
  //  Create the intro label.
  //---------------------------------------------------------------------------
  intro = new QLabel  (tr ("Please select the type of data to import:"), mPage2);
  intro->setGeometry  (130, 40, WIZARD_WIDTH - 160, 25);
  intro->setAlignment (AlignHCenter);
  intro->setFont      (f);
  
  //---------------------------------------------------------------------------
  //  Create the button group and the radio buttons.
  //---------------------------------------------------------------------------
  mGroup = new QButtonGroup   (mPage2, "group box");
  mGroup->setFrameStyle       (QFrame::NoFrame);
  mGroup->setGeometry         (160, 60, WIZARD_WIDTH - 160, 200);

  mVCard = new QRadioButton   (tr ("VCard (*.vcf, *.gcrd)"), mGroup);
  mVCard->setChecked          (true);
  mVCard->setGeometry         (50, 20, 140, 25);

  mCSV = new QRadioButton     (tr ("CSV (*.csv, *.txt)"), mGroup);
  mCSV->setGeometry           (50, 50, 140, 25);

  mOutlook = new QRadioButton (tr ("Outlook (*.mbx)"), mGroup);
  mOutlook->setGeometry       (50, 80, 140, 25);

  addPage (mPage2, tr("Step 1 of 3"));
}

//=============================================================================
// Setup the third page.
//=============================================================================
void CImport::setupPage3()
{
  QLabel   *intro, *panel;
  QFont 	f(font());

  f.setBold      (true);
  f.setPointSize (f.pointSize() + 1);

  //---------------------------------------------------------------------------
  //  Create the frame of the page.
  //---------------------------------------------------------------------------
  mPage3 = new QFrame (this);

  //---------------------------------------------------------------------------
  //  Create the image panel.
  //---------------------------------------------------------------------------
  panel = new QLabel (mPage3);
  panel->setGeometry (10, 10, 100, 200);
  panel->setPixmap   (*mImage);
  
  //---------------------------------------------------------------------------
  //  Create the intro label.
  //---------------------------------------------------------------------------
  if (mGroup->id (mGroup->selected()) == 0)
    intro = new QLabel (tr ("Please select one or more data files:"), mPage3);
  else
    intro = new QLabel (tr ("Please select a data file:"), mPage3);
    
  intro->setGeometry  (130, 40, WIZARD_WIDTH - 160, 25);
  intro->setAlignment (AlignHCenter);
  intro->setFont      (f);

  addPage (mPage3, tr("Step 2 of 3"));
}

//=============================================================================
// Setup the fourth page.
//=============================================================================
void CImport::setupPage4()
{
  QLabel   *intro, *panel;
  QFont 	f(font());

  f.setBold      (true);
  f.setPointSize (f.pointSize() + 1);

  //---------------------------------------------------------------------------
  //  Create the frame of the page.
  //---------------------------------------------------------------------------
  mPage4 = new QFrame (this);

  //---------------------------------------------------------------------------
  //  Create the image panel.
  //---------------------------------------------------------------------------
  panel = new QLabel (mPage4);
  panel->setGeometry (10, 10, 100, 200);
  panel->setPixmap   (*mImage);
  
  //---------------------------------------------------------------------------
  //  Create the intro label.
  //---------------------------------------------------------------------------
  if (mGroup->id (mGroup->selected()) == 0)
    intro = new QLabel (tr ("Please select one or more data files:"), mPage4);
  else
    intro = new QLabel (tr ("Please select a data file:"), mPage4);
    
  intro->setGeometry  (130, 40, WIZARD_WIDTH - 160, 25);
  intro->setAlignment (AlignHCenter);
  intro->setFont      (f);

  addPage (mPage4, tr("Step 3 of 3"));

  setFinishEnabled (mPage4, true);
}

//=============================================================================
// SLOT:   The current page of the wizard changed.
//=============================================================================
void CImport::slotPageChanged (const QString &title)
{
  int      i;

  //----------------------------------------------------------------------------
  // On all pages except the last make the next button the default button,
  //----------------------------------------------------------------------------
  if (title != tr("Step 3 of 3"))
  {
    nextButton()->setDefault (true);
    return;
  }

  finishButton()->setDefault (true);
}

//=============================================================================
// SLOT:   Show the context sensitive help for the currently displayed page.
//=============================================================================
void CImport::slotShowHelp()
{
//  showHelp ("datavorfall.html", 
//            "genreport" + QString::number (indexOf (currentPage())));
}
