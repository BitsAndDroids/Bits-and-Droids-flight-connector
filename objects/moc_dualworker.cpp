/****************************************************************************
** Meta object code from reading C++ file 'dualworker.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.2.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../dual/dualworker.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#include <QtCore/QList>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'dualworker.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.2.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_DualWorker_t {
    const uint offsetsAndSize[16];
    char stringdata0[91];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(offsetof(qt_meta_stringdata_DualWorker_t, stringdata0) + ofs), len 
static const qt_meta_stringdata_DualWorker_t qt_meta_stringdata_DualWorker = {
    {
QT_MOC_LITERAL(0, 10), // "DualWorker"
QT_MOC_LITERAL(11, 16), // "updateActiveCom1"
QT_MOC_LITERAL(28, 0), // ""
QT_MOC_LITERAL(29, 13), // "lastActiveCom"
QT_MOC_LITERAL(43, 18), // "GameConnectionMade"
QT_MOC_LITERAL(62, 3), // "con"
QT_MOC_LITERAL(66, 4), // "mode"
QT_MOC_LITERAL(71, 19) // "BoardConnectionMade"

    },
    "DualWorker\0updateActiveCom1\0\0lastActiveCom\0"
    "GameConnectionMade\0con\0mode\0"
    "BoardConnectionMade"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_DualWorker[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    1,   32,    2, 0x06,    1 /* Public */,
       4,    2,   35,    2, 0x06,    3 /* Public */,
       7,    2,   40,    2, 0x06,    6 /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QStringList,    3,
    QMetaType::Void, QMetaType::Int, QMetaType::Int,    5,    6,
    QMetaType::Void, QMetaType::Int, QMetaType::Int,    5,    6,

       0        // eod
};

void DualWorker::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<DualWorker *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->updateActiveCom1((*reinterpret_cast< QList<QString>(*)>(_a[1]))); break;
        case 1: _t->GameConnectionMade((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 2: _t->BoardConnectionMade((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (DualWorker::*)(QList<QString> );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&DualWorker::updateActiveCom1)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (DualWorker::*)(int , int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&DualWorker::GameConnectionMade)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (DualWorker::*)(int , int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&DualWorker::BoardConnectionMade)) {
                *result = 2;
                return;
            }
        }
    }
}

const QMetaObject DualWorker::staticMetaObject = { {
    QMetaObject::SuperData::link<QThread::staticMetaObject>(),
    qt_meta_stringdata_DualWorker.offsetsAndSize,
    qt_meta_data_DualWorker,
    qt_static_metacall,
    nullptr,
qt_incomplete_metaTypeArray<qt_meta_stringdata_DualWorker_t
, QtPrivate::TypeAndForceComplete<DualWorker, std::true_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<QList<QString>, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<int, std::false_type>, QtPrivate::TypeAndForceComplete<int, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<int, std::false_type>, QtPrivate::TypeAndForceComplete<int, std::false_type>



>,
    nullptr
} };


const QMetaObject *DualWorker::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *DualWorker::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_DualWorker.stringdata0))
        return static_cast<void*>(this);
    return QThread::qt_metacast(_clname);
}

int DualWorker::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QThread::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 3)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void DualWorker::updateActiveCom1(QList<QString> _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void DualWorker::GameConnectionMade(int _t1, int _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void DualWorker::BoardConnectionMade(int _t1, int _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
