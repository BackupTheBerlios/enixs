//#############################################################################
//#  numbervalidator.h
//#  =================
//#
//#  Creation:       Mon May 27 2002
//#  Author:         Ralf Schneider  (ralf.schneider@b-connected.de)
//#  Copyright:      (C) 2002 by B-connected Linux Solutions GmbH
//#
//#  ------------------------------------------------------------------------
//#
//#  This file contains the definition of the class CNumberValidator.
//#  This validator checks if the given input string is a number and if it is
//#  in the given range.
//#
//#  ------------------------------------------------------------------------
//#
//#  This program is free software; you can redistribute it and/or modify it
//#  under the terms of the GNU General Public License as published by the
//#  Free Software Foundation; either version 2 of the License, or (at your
//#  option) any later version.
//#
//#############################################################################

#ifndef NUMBERVALIDATOR_H
#define NUMBERVALIDATOR_H

#include <qvalidator.h>


class CNumberValidator : public QValidator
{
public: 
  CNumberValidator (int bottom, int top, bool sign,
                    QWidget *parent, const char *name=0);
  ~CNumberValidator();

  QValidator::State validate (QString &input, int &pos) const;
	
private:
  int		mBottom;
  int  		mTop;
  bool		mSign;
};

#endif
