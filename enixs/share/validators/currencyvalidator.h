//#############################################################################
//#  currencyvalidator.h
//#  ===================
//#
//#  Creation:       Mon May 27 2002
//#  Author:         Ralf Schneider  (ralf.schneider@b-connected.de)
//#  Copyright:      (C) 2002 by B-connected Linux Solutions GmbH
//#
//#  ------------------------------------------------------------------------
//#
//#  This file contains the definition of the class CCurrencyValidator.
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

#ifndef CURRENCYVALIDATOR_H
#define CURRENCYVALIDATOR_H

#include <qvalidator.h>


class CCurrencyValidator : public QValidator
{
public: 
  CCurrencyValidator (double bottom, double top, bool sign,
                      QWidget *parent, const char *name=0);
  ~CCurrencyValidator();

  QValidator::State validate (QString &input, int &pos) const;
	
private:
  double	mBottom;
  double    mTop;
  bool		mSign;
};

#endif
