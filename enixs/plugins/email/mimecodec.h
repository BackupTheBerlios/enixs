/*
    Copyright (C) 2001-2002 by theKompany.com <www.thekompany.com>
    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

    In addition, as a special exception, theKompany.com gives permission
    to link the code of this program with the tkwidgets library (or with
    modified versions of tkwidgets that use the same license as tkwidgets),
    and distribute linked combinations including the two.  You must obey
    the GNU General Public License in all respects for all of the code used
    other than tkwidgets.  If you modify this file, you may extend this
    exception to your version of the file, but you are not obligated to do so.
    If you do not wish to do so, delete this exception statement from your
    version.

    This license grants you the ability to use tkwidgets with Aethera only
    and may not be used outside of Aethera.
    See also http://www.thekompany.com/products/license.txt for details.
*/
#ifndef __MIMECodec__
#define __MIMECodec__

#include <qcstring.h>

namespace MIMECodec
{
	/** wrap a text without CR-LF chars on the 76 character boundary.*/
	QCString canonicalForm(const QCString &);
	
	/** translate a MIME-encoded text.*/
	QCString translate(const QCString &);
	
	/** MIME-fold text (used for headers).*/
	QCString fold(const QCString &);
	
	/** unfold MIME-folded text.*/
	QCString unfold(const QCString &);
	
	/** encode UNIX text (cr-ended) into MIME text (crlf-ended).*/
	QCString encodeEndLine(const QCString &);
	
	/** decode MIME text (crlf-ended) into UNIX text (cr-ended).*/
	QCString decodeEndLine(const QCString &);
	
	/** encode text in quoted-printable format (the bool parameter forces extra MIME gateway compatibility).*/
	QCString encodeQuotedPrintable(const QCString &, bool compat=false);
	
	/** decode quoted-printable encoded text.*/
	QCString decodeQuotedPrintable(const QCString &);
	
	/** encode binary data in base64 format.*/
	QByteArray encodeBase64(const QByteArray &);
	
	/** decode base64-encoded binary data.*/
	QByteArray decodeBase64(const QByteArray &);
	
	/** encode binary data using the given encoding name ("base64", "quoted-printable", "7bit", "8bit", "binary", "none", "quoted-printable-compat" <- this can be used only for encoding).*/
	QByteArray encode(const QByteArray &, const char *);   // buffer, encoding
	
	/** decode binary data using encoding name.*/
	QByteArray decode(const QByteArray &, const char *); // encoded buffer, encoding
	
	/** encode the message id.
	  * It has a unique number/string and the user name like parameter.
	  */
	QString encodeMessageId(const QString &, const QString &);
};

#endif
