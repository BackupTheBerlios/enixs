//#############################################################################
//#  phonevalidator.h
//#  ================
//#
//#  Creation:       Mon May 27 2002
//#  Author:         Ralf Schneider  (ralf.schneider@b-connected.de)
//#  Copyright:      (C) 2002 by B-connected Linux Solutions GmbH
//#
//#  ------------------------------------------------------------------------
//#
//#  This file contains the definition of the class CPhoneValidator.
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

#ifndef PHONEVALIDATOR_H
#define PHONEVALIDATOR_H

#include <qvalidator.h>


class CPhoneValidator : public QValidator
{
public: 
  CPhoneValidator (QWidget *parent, const char *name=0);
  ~CPhoneValidator();

  QValidator::State validate (QString &input, int &pos) const;
};

#endif
