#include "cpop.h"
#include "email.h"

#include <qtextstream.h>
#include <qdatastream.h>
#include <qfile.h>
#include <qsocket.h>
#include <qdns.h>
#include <qtimer.h>
#include <qapplication.h>
#include <qmessagebox.h>
#include <qregexp.h>
#include <qcstring.h>


CPOP::CPOP() {
}

CPOP::CPOP(CEmail* parent, QString &hostname)
{
  p = parent;
  mFileTransfer = false;
  mRemainingBytes = 24191;
  socket = new QSocket( this, "EMails empfangen" );
  connect ( socket, SIGNAL( readyRead() ),
	    this, SLOT( readyRead() ) );
  connect ( socket, SIGNAL( connected() ),
	    this, SLOT( connected() ) );
  
  mxLookup = new QDns( hostname, QDns::Mx );
  connect( mxLookup, SIGNAL(resultsReady()),
	   this, SLOT(dnsLookupHelper()) );
    
    /** message = QString::fromLatin1( "From: " ) + from +
	QString::fromLatin1( "\nTo: " ) + to +
	QString::fromLatin1( "\nSubject: " ) + subject +
	QString::fromLatin1( "\n\n" ) + body + "\n";
	message.replace( QRegExp( QString::fromLatin1( "\n" ) ),
	QString::fromLatin1( "\r\n" ) );
	message.replace( QRegExp( QString::fromLatin1( "\r\n.\r\n" ) ),
	QString::fromLatin1( "\r\n..\r\n" ) );
	
	this->from = from;
	rcpt = to;
    */	
    state = USER;
    
}


CPOP::~CPOP()
{
  delete t;
  delete socket;
}


void CPOP::dnsLookupHelper()
{
  QValueList<QDns::MailServer> s = mxLookup->mailServers();
  
  if ( s.isEmpty() && mxLookup->isWorking() )
    return;
  
  emit status( tr( "Connecting to %1" ).arg( s.first().name ) );

  std::cout << tr( "Connecting to %1" ).arg( s.first().name ) << endl;

  socket->connectToHost( s.first().name, 110 ); // Port 110 ist fuer POP3
  t = new QTextStream( socket );
}


void CPOP::connected()
{
  emit status( tr( "Connected to %1" ).arg( socket->peerName() ) );
  std::cout << tr( "Connected to %1" ).arg( socket->peerName() ) << endl;
}


int CPOP::readBinaryData() {
  QByteArray    buffer;
  QByteArray    buffer_dec64; // Decoded Buffer
  QString       sql;
  int           bytes, len;
  bool          continueReading = false;
    
  //---------------------------------------------------------------------------
  // Check how many bytes are available.
  //---------------------------------------------------------------------------
  bytes = socket->bytesAvailable();
  std::cout << bytes << endl;
  
  /**if (mRemainingBytes < bytes)
     {
     bytes = mRemainingBytes;
     continueReading = true;
     }
  */
  //---------------------------------------------------------------------------
  // Read from the socket..
  //---------------------------------------------------------------------------
  buffer.resize (bytes);
  len = socket->readBlock ((char *)(const char *)buffer, bytes);
  buffer_dec64 = MIMECodec::decodeBase64(buffer);
  std::cout << len << endl;
  //-----------------------------------------------------------------------
  // Append the read data to the file.
  //-----------------------------------------------------------------------
  //QFile  file (mFilename);
  QFile  file ("vmarc.jpg");
  
  if (file.open (IO_WriteOnly | IO_Append))
    {
      file.writeBlock (buffer, len);
      file.close();
    }
  
  mRemainingBytes -= bytes;
  
  //-----------------------------------------------------------------------
  // If the file transfer is complete, write the file into the database.
  //-----------------------------------------------------------------------
  //if (mRemainingBytes <= 0)
  if (bytes <= 0)
    mFileTransfer = false;
  
  //if (continueReading)
  //readyRead();
  
  return len;
}

bool CPOP::translateBinaryData(QString tlBdata) {
  QByteArray buffer,buffer64;
  QString data= tlBdata.mid(tlBdata.findRev("attachment")+33,tlBdata.length()-tlBdata.findRev("attachment")-234);
  //buffer.setRawData(tlBdata.latin1(),sizeof(tlBdata.latin1()));
  //buffer.fill(data.latin1(),-1);
  //buffer64.resize(data.length());
  buffer64 = MIMECodec::decodeBase64(buffer);
  std::cout << buffer << endl;
  std::cout << tlBdata.length()-tlBdata.findRev("attachment")-34 << endl;
    
  //-----------------------------------------------------------------------
  // Append the read data to the file.
  //-----------------------------------------------------------------------
  QFile  file ("vmarc.jpg");
     
  if (file.open (IO_WriteOnly))
    {
      file.writeBlock (buffer64, data.length());
      file.close();
    }
  return true;
}

void CPOP::readyRead()
{
  // POP3 is line-oriented
  if ( !socket->canReadLine() ){
    return;
  }
  
  QString responseLine;
  do  {
    responseLine = socket->readLine();
    //std::cout << responseLine << endl;
    if ((responseLine.find("+OK",0,true)!=0) && (responseLine.find("-ERR",0,true)!=0) ) {
      if ((state==BIN) && (responseLine.find("--")==0)) state=CLOS;  
      if (state!=BIN) {
	response += responseLine; 
	std::cout << responseLine << endl;
      }
      else {
	readBinaryData();
	std::cout << responseLine << endl;	
      }
      //std:: cout << responseLine.find(".\r\n") << endl;
      if (responseLine.find(".\r\n")==0) state=CLOS;
      if (responseLine.find("Content-Disposition: attachment;")==0) {
	std::cout << "Binary Mode" << endl;
	state=BIN;
      }
    }
    
  } while( socket->canReadLine());
  responseLine.truncate( 3 );

  if (state==USER) {
    *t << "USER svenali@t-online.de\r\n";
    std::cout << state << endl;
    state=PASS;
  } else if (state==PASS) {
    *t << "PASS asusp3bf\r\n";
    state=RETR;
  } else if (state==RETR) {    
    mFileTransfer=true;
    *t << "RETR 2\r\n"; 
    std::cout << state << endl;
    //if (mRemainingBytes<=0) 
    //if (response.findRev('.')==0) state=QUIT;
    state=QUIT;
  } else if (state==QUIT) {
    std::cout << state << endl;
    //p->updateAllViews(); // Update the Email Client
    //*t << "QUIT\r\n"; 
  } else if (state==CLOS) {
    std::cout << state << endl;
    p->updateAllViews(); // Update the Email Client
    //*t << "QUIT\r\n"; 
    //translateBinaryData(response);
  }
    
  //std::cout << response << endl;
  //p->updateAllViews(); // Update the Email Client
}

