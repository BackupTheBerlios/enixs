#ifndef CPOP_H
#define CPOP_H

#include <qobject.h>
#include <qstring.h>
#include "mimecodec.h"

class QSocket;
class QTextStream;
class QDataStream;
class QDns;
class CEmail;
class QFile;
class QCString;

class CPOP : public QObject
{
    Q_OBJECT

public:
    CPOP();
    CPOP(CEmail* parent, QString &hostname);
    ~CPOP();

    QString get_email() {return response;};

protected:
    int readBinaryData();
    bool translateBinaryData(QString tlBdata);

signals:
    void status( const QString & );

private slots:
    void dnsLookupHelper();
    void readyRead();
    void connected();

private:
    enum State {
      INIT,
      STAT,
      LIST, 
      RETR,
      DELE,
      NOOP,
      RSET,
      QUIT,
      TOP,
      UIDL,
      USER,
      PASS,
      APOP,
      CLOS,
      BIN
    };

    QString message;
    QString from;
    QString retr;
    QSocket *socket;
    QTextStream * t;
    State state;
    QString response;
    QDns * mxLookup;
    QString email;
    CEmail* p; // For calling methods in CPOP
    QString mFilename;
    bool mFileTransfer;
    int mRemainingBytes;
};

#endif

