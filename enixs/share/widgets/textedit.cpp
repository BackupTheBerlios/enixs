//#############################################################################
//#  textedit.cpp
//#  ==============
//#
//#  Creation:       Sat Jun 29 19:50:16 CEST 2002
//#  Author:         Ralf Schneider  (ralf.schneider@b-connected.de)
//#  Copyright:      (C) 2002 by B-connected Linux Solutions GmbH
//#
//#  ------------------------------------------------------------------------
//#
//#  This file contains the implementation of the class CTextEdit.
//#  This class derives the class QTextEdit and implements a mechanism for
//#  input checking.
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
// Application specific includes.
//=============================================================================
#include "textedit.h"

//=============================================================================
// Constructor of the class CTextEdit.
//=============================================================================
CTextEdit::CTextEdit (QWidget *parent, const char *name)
    : QTextEdit (parent, name)
{
  setTextFormat (Qt::PlainText);

  mLength = 0;
}

//=============================================================================
// Constructor of the class CTextEdit.
//=============================================================================
CTextEdit::~CTextEdit()
{
}

//=============================================================================
// Set the text.
//=============================================================================
void CTextEdit::setText (const QString & text)
{
  mLength = text.length();
  QTextEdit::setText (text);
}

//=============================================================================
// Handle the keyPress event.
//=============================================================================
void CTextEdit::keyPressEvent (QKeyEvent *e)
{
  //----------------------------------------------------------------------------
  //  Check if the user pressed the backspace key --> decrement mLength.
  //----------------------------------------------------------------------------
  if (e->key() == Qt::Key_Backspace)
  {
    if (mLength > 0)
      mLength--;
  }
  else
  {
    if (mLength >= mMaxLength)
      return;

    mLength++;
  }

  //----------------------------------------------------------------------------
  //  Delegate the KeyPress event to the parent class.
  //----------------------------------------------------------------------------
  QTextEdit::keyPressEvent (e);
}
