//#############################################################################
//#  textedit.h
//#  ==========
//#
//#  Creation:       Sat Jun 29 19:50:16 CEST 2002
//#  Author:         Ralf Schneider  (ralf.schneider@b-connected.de)
//#  Copyright:      (C) 2002 by B-connected Linux Solutions GmbH
//#
//#  ------------------------------------------------------------------------
//#
//#  This file contains the definition of the class CTextEdit.
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

#ifndef TEXTEDIT_H
#define TEXTEDIT_H

#include <qtextedit.h>


class CTextEdit : public QTextEdit
{
  Q_OBJECT

public: 
	CTextEdit (QWidget *parent = 0, const char *name=0);
	~CTextEdit();

	void setMaxLength (int length) { mMaxLength = length; }

public slots:
  void setText      (const QString & text);

protected:
	void keyPressEvent (QKeyEvent *e);

private:
	int        mLength;
	int        mMaxLength;
};

#endif
