/****************************************************************************
** CEmail meta object code from reading C++ file 'email.h'
**
** Created: Thu Jul 11 16:32:43 2002
**      by: The Qt MOC ($Id: moc_email.cpp,v 1.1 2002/07/11 16:51:50 svenali Exp $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "email.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 19)
#error "This file was generated using the moc from 3.0.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *CEmail::className() const
{
    return "CEmail";
}

QMetaObject *CEmail::metaObj = 0;
static QMetaObjectCleanUp cleanUp_CEmail;

#ifndef QT_NO_TRANSLATION
QString CEmail::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "CEmail", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString CEmail::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "CEmail", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* CEmail::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QWidget::staticMetaObject();
    static const QUMethod slot_0 = {"downloadEmails", 0, 0 };
    static const QUMethod slot_1 = {"updateAllViews", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "downloadEmails()", &slot_0, QMetaData::Public },
	{ "updateAllViews()", &slot_1, QMetaData::Public }
    };
    metaObj = QMetaObject::new_metaobject(
	"CEmail", parentObject,
	slot_tbl, 2,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_CEmail.setMetaObject( metaObj );
    return metaObj;
}

void* CEmail::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "CEmail" ) ) return (CEmail*)this;
    return QWidget::qt_cast( clname );
}

bool CEmail::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: downloadEmails(); break;
    case 1: updateAllViews(); break;
    default:
	return QWidget::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool CEmail::qt_emit( int _id, QUObject* _o )
{
    return QWidget::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool CEmail::qt_property( int _id, int _f, QVariant* _v)
{
    return QWidget::qt_property( _id, _f, _v);
}
#endif // QT_NO_PROPERTIES
