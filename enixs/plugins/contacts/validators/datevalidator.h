//#############################################################################
//#  datevalidator.h
//#  ===============
//#
//#  Creation:       Mon May 27 2002
//#  Author:         Ralf Schneider  (ralf.schneider@b-connected.de)
//#  Copyright:      (C) 2002 by B-connected Linux Solutions GmbH
//#
//#  ------------------------------------------------------------------------
//#
//#  This file contains the definition of the class CDateValidator.
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

#ifndef DATEVALIDATOR_H
#define DATEVALIDATOR_H

#include <qvalidator.h>


class CDateValidator : public QValidator
{
public: 
  CDateValidator (QWidget *parent, const char *name = 0);
  ~CDateValidator();

  QValidator::State validate (QString &input, int &pos) const;
	
protected:
  bool checkDate (bool lastDot, const QString day,
                  const QString month=0, const QString year=0) const;

private:
  int	mLength;
};

#endif
