/****************************************************************************
** MyWidget meta object code from reading C++ file 'mywidget.h'
**
** Created: Thu May 3 10:07:08 2012
**      by: The Qt MOC ($Id: qt/moc_yacc.cpp   3.3.8   edited Feb 2 14:59 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "include/mywidget.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.8b. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *MyWidget::className() const
{
    return "MyWidget";
}

QMetaObject *MyWidget::metaObj = 0;
static QMetaObjectCleanUp cleanUp_MyWidget( "MyWidget", &MyWidget::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString MyWidget::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "MyWidget", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString MyWidget::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "MyWidget", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* MyWidget::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QWidget::staticMetaObject();
    static const QUParameter param_slot_0[] = {
	{ 0, &static_QUType_ptr, "QPaintEvent", QUParameter::In }
    };
    static const QUMethod slot_0 = {"paintEvent", 1, param_slot_0 };
    static const QUMethod slot_1 = {"play", 0, 0 };
    static const QUMethod slot_2 = {"open_file", 0, 0 };
    static const QUMethod slot_3 = {"save", 0, 0 };
    static const QUMethod slot_4 = {"skinthresChanged", 0, 0 };
    static const QUMethod slot_5 = {"thresChanged", 0, 0 };
    static const QUMethod slot_6 = {"algorithmChange", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "paintEvent(QPaintEvent*)", &slot_0, QMetaData::Protected },
	{ "play()", &slot_1, QMetaData::Protected },
	{ "open_file()", &slot_2, QMetaData::Protected },
	{ "save()", &slot_3, QMetaData::Protected },
	{ "skinthresChanged()", &slot_4, QMetaData::Protected },
	{ "thresChanged()", &slot_5, QMetaData::Protected },
	{ "algorithmChange()", &slot_6, QMetaData::Protected }
    };
    metaObj = QMetaObject::new_metaobject(
	"MyWidget", parentObject,
	slot_tbl, 7,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_MyWidget.setMetaObject( metaObj );
    return metaObj;
}

void* MyWidget::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "MyWidget" ) )
	return this;
    return QWidget::qt_cast( clname );
}

bool MyWidget::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: paintEvent((QPaintEvent*)static_QUType_ptr.get(_o+1)); break;
    case 1: play(); break;
    case 2: open_file(); break;
    case 3: save(); break;
    case 4: skinthresChanged(); break;
    case 5: thresChanged(); break;
    case 6: algorithmChange(); break;
    default:
	return QWidget::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool MyWidget::qt_emit( int _id, QUObject* _o )
{
    return QWidget::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool MyWidget::qt_property( int id, int f, QVariant* v)
{
    return QWidget::qt_property( id, f, v);
}

bool MyWidget::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
