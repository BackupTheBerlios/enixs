/****************************************************************************
** CPOP meta object code from reading C++ file 'cpop.h'
**
** Created: Thu Jul 11 16:10:03 2002
**      by: The Qt MOC ($Id: moc_cpop.cpp,v 1.1 2002/07/11 16:51:50 svenali Exp $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "cpop.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 19)
#error "This file was generated using the moc from 3.0.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *CPOP::className() const
{
    return "CPOP";
}

QMetaObject *CPOP::metaObj = 0;
static QMetaObjectCleanUp cleanUp_CPOP;

#ifndef QT_NO_TRANSLATION
QString CPOP::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "CPOP", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString CPOP::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "CPOP", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* CPOP::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QObject::staticMetaObject();
    static const QUMethod slot_0 = {"dnsLookupHelper", 0, 0 };
    static const QUMethod slot_1 = {"readyRead", 0, 0 };
    static const QUMethod slot_2 = {"connected", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "dnsLookupHelper()", &slot_0, QMetaData::Private },
	{ "readyRead()", &slot_1, QMetaData::Private },
	{ "connected()", &slot_2, QMetaData::Private }
    };
    static const QUParameter param_signal_0[] = {
	{ 0, &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod signal_0 = {"status", 1, param_signal_0 };
    static const QMetaData signal_tbl[] = {
	{ "status(const QString&)", &signal_0, QMetaData::Protected }
    };
    metaObj = QMetaObject::new_metaobject(
	"CPOP", parentObject,
	slot_tbl, 3,
	signal_tbl, 1,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_CPOP.setMetaObject( metaObj );
    return metaObj;
}

void* CPOP::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "CPOP" ) ) return (CPOP*)this;
    return QObject::qt_cast( clname );
}

// SIGNAL status
void CPOP::status( const QString& t0 )
{
    activate_signal( staticMetaObject()->signalOffset() + 0, t0 );
}

bool CPOP::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: dnsLookupHelper(); break;
    case 1: readyRead(); break;
    case 2: connected(); break;
    default:
	return QObject::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool CPOP::qt_emit( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->signalOffset() ) {
    case 0: status(static_QUType_QString.get(_o+1)); break;
    default:
	return QObject::qt_emit(_id,_o);
    }
    return TRUE;
}
#ifndef QT_NO_PROPERTIES

bool CPOP::qt_property( int _id, int _f, QVariant* _v)
{
    return QObject::qt_property( _id, _f, _v);
}
#endif // QT_NO_PROPERTIES
