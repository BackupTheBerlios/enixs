//#############################################################################
//#  phonevalidator.cpp
//#  ==================
//#
//#  Creation:       Mon May 27 2002
//#  Author:         Ralf Schneider  (ralf.schneider@b-connected.de)
//#  Copyright:      (C) 2002 by B-connected Linux Solutions GmbH
//#
//#  ------------------------------------------------------------------------
//#
//#  This file contains the implementation of the class CPhoneValidator.
//#  This validator checks if the given input string is a valid phone number.
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
#include "phonevalidator.h"

//=============================================================================
// Constructor of the class CPhoneValidator.
//=============================================================================
CPhoneValidator::CPhoneValidator (QWidget *parent, const char *name)
    : QValidator ((QObject *)parent, name)
{
}

//=============================================================================
// Destructor of the class CPhoneValidator.
//=============================================================================
CPhoneValidator::~CPhoneValidator()
{
}

//=============================================================================
// Validate the given input string.
//=============================================================================
QValidator::State CPhoneValidator::validate (QString &input, int &pos) const
{
  //---------------------------------------------------------------------------
  //  If input is empty it is Intermediate.
  //---------------------------------------------------------------------------
  if (input.isEmpty())
    return Intermediate;

  //---------------------------------------------------------------------------
  //  All characters besides numbers and - / + ( ) are invalid.
  //---------------------------------------------------------------------------
  if (input.find (QRegExp ("[^0-9\\-\\+/() ]")) != -1)
    return Invalid;

  //---------------------------------------------------------------------------
  //  + or ++ are only allowed at the beginning of the string.
  //---------------------------------------------------------------------------
  switch (input.contains ('+'))
  {
    case 0:
      break;

    case 1:
      if (input.left (1) != "+")
        return Invalid;
      break;

    case 2:
      if (input.left (2) != "++")
        return Invalid;
      break;

    default:
      return Invalid;
  }
		
  //---------------------------------------------------------------------------
  //  ( or ) are only allowed once in the string.
  //---------------------------------------------------------------------------
  if (input.contains ('(') > 1)
    return Invalid;
  
  if (input.contains (')') > 1)
    return Invalid;

  return Acceptable;
}
