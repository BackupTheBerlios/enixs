//#############################################################################
//#  datevalidator.cpp
//#  =================
//#
//#  Creation:       Mon May 27 2002
//#  Author:         Ralf Schneider  (ralf.schneider@b-connected.de)
//#  Copyright:      (C) 2002 by B-connected Linux Solutions GmbH
//#
//#  ------------------------------------------------------------------------
//#
//#  This file contains the implementation of the class CDateValidator.
//#  This validator checks if the format of the given input string is a valid
//#  date value.
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
#include "datevalidator.h"

//=============================================================================
// Constructor of the class CDateValidator.
//=============================================================================
CDateValidator::CDateValidator (QWidget *parent, const char *name)
    : QValidator ((QObject *)parent, name)
{
}

//=============================================================================
// Destructor of the class CDateValidator.
//=============================================================================
CDateValidator::~CDateValidator()
{
}

//=============================================================================
// Validate the given input string.
//=============================================================================
QValidator::State CDateValidator::validate (QString &input, int &pos) const
{
  int pos1, pos2;

  //---------------------------------------------------------------------------
  //  If input is empty it is Intermediate.
  //---------------------------------------------------------------------------
  if (input.isEmpty())
    return Intermediate;

  //---------------------------------------------------------------------------
  //  All characters besides numbers and dots are invalid.
  //---------------------------------------------------------------------------
  if (input.find (QRegExp ("[^0-9.]")) != -1)
    return Invalid;

  //---------------------------------------------------------------------------
  //  Check how many dots are in the string.
  //---------------------------------------------------------------------------
  switch (input.contains ('.'))
  {
  	//-------------------------------------------------------------------------
  	//  The input string is interpreted as a day.
  	//-------------------------------------------------------------------------
    case 0:
      //-----------------------------------------------------------------------
      //  Check the parts of the date string for correctness.
      //-----------------------------------------------------------------------
      if (checkDate (false, input) == false)
        return Invalid;

      break;

    //-------------------------------------------------------------------------
    //  The input string is interpreted as a day and a month.
    //-------------------------------------------------------------------------
    case 1:
      pos1 = input.find ('.');

      //-----------------------------------------------------------------------
      //  If the last character is a dot, it's OK.
      //-----------------------------------------------------------------------
      if ((input.at (input.length() - 1) == '.') && (input.length() != 1))
      {
        if (checkDate (true, input.left (pos1)) == false)
          return Invalid;
        else
          return Acceptable;
      }

      //-----------------------------------------------------------------------
      //  Check the parts of the date string for correctness.
      //-----------------------------------------------------------------------
      if (checkDate (false, input.left (pos1), input.mid  (pos1 + 1)) == false)
        return Invalid;
      
      break;

    //-------------------------------------------------------------------------
    //  The input string is interpreted as a day, a month and a year.
    //-------------------------------------------------------------------------
    case 2:
      pos1 = input.find ('.');
      pos2 = input.findRev('.');

      //-----------------------------------------------------------------------
      //  Two dots at the end are not allowed.
      //-----------------------------------------------------------------------
      if (input.mid (input.length() - 2) == "..")
        return Invalid;

      //-----------------------------------------------------------------------
      //  If the last character is a dot, it's OK.
      //-----------------------------------------------------------------------
      if (input.at (input.length() - 1) == '.')
      {
        if (checkDate (true, input.left (pos1), 
                       input.mid  (pos1 + 1, pos2 - pos1 - 1)) == false)
          return Invalid;
        else
          return Acceptable;
      }

      //-----------------------------------------------------------------------
      //  Check the parts of the date string for correctness.
      //-----------------------------------------------------------------------
      if (checkDate (false, input.left (pos1),
                     input.mid  (pos1 + 1, pos2 - pos1 - 1),
                     input.mid (pos2 + 1)) == false)
        return Invalid;

      break;

    default:
      return Invalid;
  }
  
  return Acceptable;
}

//=============================================================================
// Check the given parts of a date string
//=============================================================================
bool CDateValidator::checkDate (bool lastDot, const QString day, 
                                const QString month, const QString year) const
{
  int	d, m=0, y=0, maxdays;

  //---------------------------------------------------------------------------
  //  Check the year.
  //---------------------------------------------------------------------------
  if (year)
  {
  	//-------------------------------------------------------------------------
  	//  Maximum of 4 digits.
  	//-------------------------------------------------------------------------
    if (year.length() > 4)
      return false;

  	//-------------------------------------------------------------------------
  	//  The year value must be between 1800 and 2100.
  	//-------------------------------------------------------------------------
    y = year.toInt();

    if ((year.length() == 4) && ((y < 1800) || (y > 2100)))
      return false;
  }

  //---------------------------------------------------------------------------
  //  Check the month.
  //---------------------------------------------------------------------------
  if (month)
  {
  	//-------------------------------------------------------------------------
  	//  Maximum of 2 digits.
  	//-------------------------------------------------------------------------
    if (month.length() > 2)
      return false;

  	//-------------------------------------------------------------------------
  	//  The month value must be between 1 and 12.
  	//-------------------------------------------------------------------------
    m = month.toInt();
    
    if (lastDot)
    {
      if ((m < 1) || (m > 12))
        return false;
    }
    else
    {
      if ((month.length() == 2) && ((m < 1) || (m > 12)))
        return false;
    }
  }

  //---------------------------------------------------------------------------
  //  Check the day.
  //---------------------------------------------------------------------------
  if ((day.length() < 1) || (day.length() > 2))
    return false;

  //-----------------------------------------------------------------------
  //  Check the maximum day number depending on the month.
  //-----------------------------------------------------------------------
  switch (m)
  {
    case 4:
    case 6:
    case 9:
    case 11:
      maxdays = 30;
      break;

    case 2:
      if ((y % 1000) == 0)
        maxdays = 29;
      else if ((y % 100) == 0)
        maxdays = 28;
      else if ((y % 4) == 0)
        maxdays = 29;
      else
        maxdays = 28;
      break;

    default:
      maxdays = 31;
  }

  //-----------------------------------------------------------------------
  //  The day must be between 1 and maxdays.
  //-----------------------------------------------------------------------
  d = day.toInt();
  
  if (lastDot)
  {
    if ((d < 1) || (d > maxdays))
      return false;
  }
  else
  {
    if ((day.length() == 2) && ((d < 1) || (d > maxdays)))
      return false;
  }
	
  return true;
}
