/****************************************************************************
** MyThread meta object code from reading C++ file 'mythread.h'
**
** Created: Fri Dec 29 15:00:51 2006
**      by: The Qt MOC ($Id: qt/moc_yacc.cpp   3.3.4   edited Jan 21 18:14 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "include/mythread.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *MyThread::className() const
{
    return "MyThread";
}

QMetaObject *MyThread::metaObj = 0;
static QMetaObjectCleanUp cleanUp_MyThread( "MyThread", &MyThread::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString MyThread::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "MyThread", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString MyThread::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "MyThread", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* MyThread::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QThread::staticMetaObject();
    metaObj = QMetaObject::new_metaobject(
	"MyThread", parentObject,
	0, 0,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_MyThread.setMetaObject( metaObj );
    return metaObj;
}

void* MyThread::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "MyThread" ) )
	return this;
    return QThread::qt_cast( clname );
}

bool MyThread::qt_invoke( int _id, QUObject* _o )
{
    return QThread::qt_invoke(_id,_o);
}

bool MyThread::qt_emit( int _id, QUObject* _o )
{
    return QThread::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool MyThread::qt_property( int id, int f, QVariant* v)
{
    return QThread::qt_property( id, f, v);
}

bool MyThread::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
