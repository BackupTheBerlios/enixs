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
#include <stdio.h>

#include <qtextcodec.h>
#include <qtextstream.h>

#include "mimecodec.h"

const char *base64table="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/=";

QCString MIMECodec::canonicalForm(const QCString &str)
{
	const unsigned wrap=76;
	QCString outstr;
	unsigned index=0, lindex=0;
	
	while(index<str.length())
	{
		outstr+=((const char *)str)[index];
		((const char *)str)[index++]=='\n'?lindex=0:lindex==wrap?lindex=0,outstr+="\r\n":lindex++;
	}
	
	return outstr;
}

QCString MIMECodec::translate(const QCString &name)
{
  int old_len, index=0;
  QCString translated_name=name;
	bool sem=true;
  while(sem)
  {
    int begin, end;
    begin=translated_name.find("=?", index);
    end=translated_name.find("?=", begin+2);

    if(begin!=-1 && end!=-1)
    {
      QCString token=translated_name.mid(begin+2, end-begin-2), charset, encoding;

      old_len=token.length()+4;
      QCString text, enc;
      int pos=token.find('?');
      charset=token.left(pos);
      token.remove(0, pos+1);
      pos=token.find('?');
      encoding=token.left(pos);
      token.remove(0, pos+1);

      encoding=encoding.upper();
      text=(const char *)token;
      if(encoding=="Q")
      {
        enc="quoted-printable";
      }
      else
      {
        enc="base64";
      }
      decode(text, enc);
      token=text;

      QTextCodec *codec=QTextCodec::codecForName(charset);
      if(codec)
      {
        token=codec->fromUnicode(token);
        translated_name.replace(begin, end-begin+2, token);
      }
      else
      {
        qDebug("Warning: could not find textcodec for %s.", (const char *)charset);
        if(charset.upper()=="US-ASCII")
        {
          translated_name.replace(begin, end-begin+2, token);
        }
        else old_len=token.length();
      }
      index=end+2+token.length()-old_len;
    }
    else
		{
			sem=false;
		}
  }
	
  return translated_name;
}

QCString MIMECodec::fold(const QCString &str)
{
	const unsigned wrap=76;
	QCString outstr;
	unsigned index=0, lindex=0;
	
	while(index<str.length())
	{
		outstr+=((const char *)str)[index++];
		lindex++;
		if(lindex==wrap)
		{
			outstr+="\n\t";
			lindex=0;
		}
	}
	
	return outstr;
}

QCString MIMECodec::unfold(const QCString &str)
{
	QCString outstr;
	unsigned index=0;
	
	while(index<str.length())
	{
		outstr+=((const char *)str)[index];
		if(index<str.length()-2 && ((const char *)str)[index+1]=='\n' && ((const char *)str)[index+2]=='\t') index+=2;
		index++;
	}
	
	return outstr;
}

QCString MIMECodec::encodeEndLine(const QCString &str)
{
  QCString outstr;
  const char *data=(const char *)str;
  char *buf=new char[str.length()*2];
  int index=0;
  int size=str.length()+1;

  for(int i=0;i<size;i++)
  {
    char c=data[i];

    if(c=='\n')
    {
      buf[index++]=0x0d;
      buf[index++]=0x0a;
    }
    else
    {
      buf[index++]=c;
    }
  }

  outstr=QCString(buf, index);
  delete buf;

  return outstr;
}

QCString MIMECodec::decodeEndLine(const QCString &str)
{
	QCString outstr;
	const char *data=(const char *)str;
	char *buf=new char[str.length()];
	int index=0;
	int size=str.length()+1;
	
	for(int i=0;i<size;i++)
	{
		char c=data[i];
		
		if(c!='\r')
		{
			buf[index++]=c;
		}
	}
	
	outstr=QCString(buf, index);
	
	delete buf;
	
	return outstr;
}

QCString MIMECodec::encodeQuotedPrintable(const QCString &str, bool compat)
{
	const int wrap=76;
	QCString outstr;
	unsigned index=0, lindex=0;
	
	while(index<str.length())
	{
		char c=((const char *)str)[index++];

		if(((c>=33 && c<=60) || (c>=62 && c<=126) || c==9 || c==32) && !(compat && QCString("!\"#@[\\]^`{|}~").find(c)!=-1))
		{
			outstr+=c;
			lindex++;
		}
		else
		{
			char h1=(c&240)/16, h2=c&15;
			
			h1=h1<10?h1+'0':h1-10+'A';
			h2=h2<10?h2+'0':h2-10+'A';
			outstr+='=';
			outstr+=h1;
			outstr+=h2;
			lindex+=3;
		}
								
		char c1=index<str.length()?((const char *)str)[index]:'a', c2=index+1<str.length()?((const char *)str)[index+1]:'a', c3=index+2<str.length()?((const char *)str)[index+2]:'a';
		
		if(lindex==wrap-1 || ((c1<33 || (c1>60 && c1<62) || c1>126 || (compat && QCString("!\"#@[\\]^`{|}~").find(c1)!=-1) ||
			 c2<33 || (c2>60 && c2<62) || c2>126 || (compat && QCString("!\"#@[\\]^`{|}~").find(c2)!=-1) ||
			 c3<33 || (c3>60 && c3<62) || c3>126 || (compat && QCString("!\"#@[\\]^`{|}~").find(c3)!=-1)) && lindex==wrap-3))
		{
			outstr+="=\r\n";
			lindex=0;
		}
	}
	
	return outstr;
}

QCString MIMECodec::decodeQuotedPrintable(const QCString &str)
{
	QCString outstr;
	unsigned index=0;
	
	while(index<str.length())
	{
		char c=((const char *)str)[index];
		if(c=='=')
		{
			char c1=index<str.length()-1?((const char *)str)[index+1]:'\n', c2=index<str.length()-2?((const char *)str)[index+2]:'X';
			if(c1=='\r' && c2=='\n') index+=3;
			else if(c1=='\n') index+=2;
			else
			{
				c1=QChar(c1).upper();
				c2=QChar(c2).upper();
				char h=(((c1>='A'&&c1<='F')?(c1-'A'+10):(c1-'0'))*16)+((c2>='A'&&c2<='F')?(c2-'A'+10):(c2-'0'));
				outstr+=h;
				index+=3;
			}
		}
		else
		{
			outstr+=c;
			index++;
		}
	}
	
	return outstr;
}

QByteArray MIMECodec::encodeBase64(const QByteArray &buf)
{
	const unsigned wrap=76;
	unsigned index=0, lindex=0, oindex=0, outsize=((buf.size()+2)/3)*4;
	outsize+=(outsize+wrap-1/wrap)*2;

	QByteArray outbuf(outsize);
	
	while(index<buf.size())
	{
		unsigned char c1=buf[index], c2=index<buf.size()-1?buf[index+1]:0, c3=index<buf.size()-2?buf[index+2]:0;
		unsigned char e1=c1/4, e2=(c1&3)*16+(c2&240)/16, e3=(c2&15)*4+(c3&192)/64, e4=c3&63;
		
		if(lindex==wrap)
		{
			outbuf[oindex++]='\n';
			lindex=0;
		}
		outbuf[oindex++]=base64table[e1];
		outbuf[oindex++]=base64table[e2];
		lindex+=2;
		if(lindex==wrap)
		{
			outbuf[oindex++]='\n';
			lindex=0;
		}
		outbuf[oindex++]=base64table[e3];
		outbuf[oindex++]=base64table[e4];
		lindex+=2;
		
		index+=3;
	}
	
	int r=(buf.size())%3;
	if(r==1) outbuf[oindex-2]='=';
	if(r) outbuf[oindex-1]='=';
		
	outbuf[oindex++]='\n';
	outbuf[oindex]='\0';
	outbuf.truncate(oindex+1); //+1);
	return outbuf;
}

QByteArray MIMECodec::decodeBase64(const QByteArray &buf)
{
	QByteArray outbuf(((buf.size()+3)/4)*3);
	
	if(buf.size()<4) return outbuf;
	
	unsigned index=0, oindex=0, size=buf.size();
	const QCString table(base64table);
	
	int revbase64table[256];
	for(int i=0;i<256;i++) revbase64table[i]=-1;
	for(int i=65;i<91;i++) revbase64table[i]=i-65;
	for(int i=97;i<123;i++) revbase64table[i]=i-71;
	for(int i=48;i<58;i++) revbase64table[i]=i+4;
	revbase64table[43]=62;
	revbase64table[47]=63;
	revbase64table[61]=64;
	
	while(index<size)
	{
		if(revbase64table[buf[index]]==-1)
		{
			index++;
		}
		else
		{
			unsigned char e1=(((const char *)buf)[index++]);
			unsigned char e2=index<buf.size()?((const char *)buf)[index++]:'A';
			unsigned char e3=index<buf.size()?((const char *)buf)[index++]:'A';
			unsigned char e4=index<buf.size()?((const char *)buf)[index++]:'A';
			e1=revbase64table[e1];
			e2=revbase64table[e2];
			e3=revbase64table[e3];
			e4=revbase64table[e4];
			unsigned char c1=e1*4+(e2&48)/16, c2=(e2&15)*16+(e3&60)/4, c3=(e3&3)*64+e4;
			outbuf[oindex++]=c1;
			outbuf[oindex++]=c2;
			outbuf[oindex++]=c3;
		}
	}
	
	int last=0;
	for(int i=size-1;i>=0;i--)
	  if(revbase64table[buf[i]]!=-1)
	  {
	    last=i;
	    break;
	  }
	
	if(buf[last-1]=='=' && buf[last]=='=') outbuf.truncate(oindex-2);
	else if(buf[last]=='=') outbuf.truncate(oindex-1);
	else outbuf.truncate(oindex);
	
	return outbuf;
}

QByteArray MIMECodec::encode(const QByteArray &buf, const char *encoding)
{
	QCString enc(encoding);
	enc=enc.lower();
	QByteArray outbuf;
	
	if(enc=="7bit" || enc=="8bit" || enc=="none" || enc=="binary")
		outbuf=buf.copy();
	if(enc=="quoted-printable")
		outbuf=MIMECodec::encodeQuotedPrintable(QCString(buf));
	if(enc=="quoted-printable-compat")
		outbuf=MIMECodec::encodeQuotedPrintable(QCString(buf), true);
	if(enc=="base64")
		outbuf=MIMECodec::encodeBase64(buf);
	
	return outbuf;
}

QByteArray MIMECodec::decode(const QByteArray &buf, const char *encoding)
{
	QCString enc(encoding);
	enc=enc.lower();
	QByteArray outbuf;
	
	if(enc=="7bit" || enc=="8bit" || enc=="none" || enc=="binary")
		outbuf=buf.copy();
	if(enc=="quoted-printable")
		outbuf=MIMECodec::decodeQuotedPrintable(QCString(buf));
	if(enc=="base64")
		outbuf=MIMECodec::decodeBase64(buf);
	
	return outbuf;
}

QString MIMECodec::encodeMessageId(const QString &indexNr, const QString &userName)
{
  QCString messageId, temp;

  temp=indexNr;
  messageId=encodeBase64(temp);
  messageId.remove(messageId.length()-3,3);
  messageId+=".";// keep those 2 keys separately
  temp=userName;
  messageId+=encodeBase64(temp);
  messageId.remove(messageId.length()-3,3);

  return (QString)messageId;
}




