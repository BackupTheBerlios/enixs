//#############################################################################
//#  currencyvalidator.cpp
//#  =====================
//#
//#  Creation:       Mon May 27 2002
//#  Author:         Ralf Schneider  (ralf.schneider@b-connected.de)
//#  Copyright:      (C) 2002 by B-connected Linux Solutions GmbH
//#
//#  ------------------------------------------------------------------------
//#
//#  This file contains the implementation of the class CCurrencyValidator.
//#  This validator checks if the given input string is currency value and
//#  if it is in the given range.
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
#include "currencyvalidator.h"

//=============================================================================
// Constructor of the class CCurrencyValidator.
//=============================================================================
CCurrencyValidator::CCurrencyValidator (double bottom, double top, bool sign,
                                        QWidget *parent, const char *name)
    : QValidator ((QObject *)parent, name)
{
  mBottom = bottom;
  mTop    = top;
  mSign   = sign;
}

//=============================================================================
// Destructor of the class CCurrencyValidator.
//=============================================================================
CCurrencyValidator::~CCurrencyValidator()
{
}

//=============================================================================
// Validate the given input string.
//=============================================================================
QValidator::State CCurrencyValidator::validate (QString &input, int &pos) const
{
  double 	x;
  bool 		ok;
  QString   s = input;

  //---------------------------------------------------------------------------
  //  If input is empty it is Intermediate.
  //---------------------------------------------------------------------------
  if (input.isEmpty())
    return Intermediate;

  //---------------------------------------------------------------------------
  //  Check if the sign (- and +) are valid.
  //---------------------------------------------------------------------------
  if (mSign)
  {
  	if ((input == "-") && (mBottom < 0))
      return Intermediate;

  	if ((input == "+") && (mTop >= 0))
      return Intermediate;
  }

  //---------------------------------------------------------------------------
  //  Dots are ignored and change the decimal comma to a dot.
  //---------------------------------------------------------------------------
  s.replace (QRegExp("\\."), "");
  s.replace (QRegExp(","), ".");

  if ((s.contains (".") == 1) && (s.mid (s.find (".") + 1).length() > 2))
    return Invalid;

  //---------------------------------------------------------------------------
  //  Check if the value is inside the defined range.
  //---------------------------------------------------------------------------
  x = s.toDouble(&ok);
  
  if (ok)
  {
    if ((x < mBottom) || (x > mTop))
      return Invalid;
  }
  else
    return Invalid;

  return Acceptable;
}
