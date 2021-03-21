/****************************************************************************
** Meta object code from reading C++ file 'MediaPlayer.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.14.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../MediaPlayer.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'MediaPlayer.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.14.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_MediaPlayer_t {
    QByteArrayData data[13];
    char stringdata0[132];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MediaPlayer_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MediaPlayer_t qt_meta_stringdata_MediaPlayer = {
    {
QT_MOC_LITERAL(0, 0, 11), // "MediaPlayer"
QT_MOC_LITERAL(1, 12, 13), // "openFileOrUrl"
QT_MOC_LITERAL(2, 26, 0), // ""
QT_MOC_LITERAL(3, 27, 4), // "text"
QT_MOC_LITERAL(4, 32, 7), // "openUrl"
QT_MOC_LITERAL(5, 40, 11), // "playOrPause"
QT_MOC_LITERAL(6, 52, 4), // "stop"
QT_MOC_LITERAL(7, 57, 13), // "sliderPressed"
QT_MOC_LITERAL(8, 71, 14), // "sliderReleased"
QT_MOC_LITERAL(9, 86, 11), // "sliderMoved"
QT_MOC_LITERAL(10, 98, 12), // "clearUrlData"
QT_MOC_LITERAL(11, 111, 10), // "windowZoom"
QT_MOC_LITERAL(12, 122, 9) // "videoZoom"

    },
    "MediaPlayer\0openFileOrUrl\0\0text\0openUrl\0"
    "playOrPause\0stop\0sliderPressed\0"
    "sliderReleased\0sliderMoved\0clearUrlData\0"
    "windowZoom\0videoZoom"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MediaPlayer[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   64,    2, 0x08 /* Private */,
       4,    0,   67,    2, 0x08 /* Private */,
       5,    0,   68,    2, 0x08 /* Private */,
       6,    0,   69,    2, 0x08 /* Private */,
       7,    0,   70,    2, 0x08 /* Private */,
       8,    0,   71,    2, 0x08 /* Private */,
       9,    0,   72,    2, 0x08 /* Private */,
      10,    0,   73,    2, 0x08 /* Private */,
      11,    0,   74,    2, 0x08 /* Private */,
      12,    0,   75,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void MediaPlayer::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<MediaPlayer *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->openFileOrUrl((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 1: _t->openUrl(); break;
        case 2: _t->playOrPause(); break;
        case 3: _t->stop(); break;
        case 4: _t->sliderPressed(); break;
        case 5: _t->sliderReleased(); break;
        case 6: _t->sliderMoved(); break;
        case 7: _t->clearUrlData(); break;
        case 8: _t->windowZoom(); break;
        case 9: _t->videoZoom(); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject MediaPlayer::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_MediaPlayer.data,
    qt_meta_data_MediaPlayer,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *MediaPlayer::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MediaPlayer::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_MediaPlayer.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int MediaPlayer::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 10)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 10)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 10;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
