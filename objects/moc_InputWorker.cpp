/****************************************************************************
** Meta object code from reading C++ file 'InputWorker.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.2.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../Inputs/InputWorker.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'InputWorker.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.2.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_InputWorker_t {
    const uint offsetsAndSize[20];
    char stringdata0[115];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(offsetof(qt_meta_stringdata_InputWorker_t, stringdata0) + ofs), len 
static const qt_meta_stringdata_InputWorker_t qt_meta_stringdata_InputWorker = {
    {
QT_MOC_LITERAL(0, 11), // "InputWorker"
QT_MOC_LITERAL(12, 15), // "updateLastValUI"
QT_MOC_LITERAL(28, 0), // ""
QT_MOC_LITERAL(29, 7), // "lastVal"
QT_MOC_LITERAL(37, 18), // "updateLastStatusUI"
QT_MOC_LITERAL(56, 10), // "lastStatus"
QT_MOC_LITERAL(67, 18), // "GameConnectionMade"
QT_MOC_LITERAL(86, 3), // "con"
QT_MOC_LITERAL(90, 4), // "mode"
QT_MOC_LITERAL(95, 19) // "BoardConnectionMade"

    },
    "InputWorker\0updateLastValUI\0\0lastVal\0"
    "updateLastStatusUI\0lastStatus\0"
    "GameConnectionMade\0con\0mode\0"
    "BoardConnectionMade"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_InputWorker[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    1,   38,    2, 0x06,    1 /* Public */,
       4,    1,   41,    2, 0x06,    3 /* Public */,
       6,    2,   44,    2, 0x06,    5 /* Public */,
       9,    2,   49,    2, 0x06,    8 /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, QMetaType::QString,    5,
    QMetaType::Void, QMetaType::Int, QMetaType::Int,    7,    8,
    QMetaType::Void, QMetaType::Int, QMetaType::Int,    7,    8,

       0        // eod
};

void InputWorker::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<InputWorker *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->updateLastValUI((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 1: _t->updateLastStatusUI((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 2: _t->GameConnectionMade((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 3: _t->BoardConnectionMade((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (InputWorker::*)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&InputWorker::updateLastValUI)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (InputWorker::*)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&InputWorker::updateLastStatusUI)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (InputWorker::*)(int , int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&InputWorker::GameConnectionMade)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (InputWorker::*)(int , int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&InputWorker::BoardConnectionMade)) {
                *result = 3;
                return;
            }
        }
    }
}

const QMetaObject InputWorker::staticMetaObject = { {
    QMetaObject::SuperData::link<QThread::staticMetaObject>(),
    qt_meta_stringdata_InputWorker.offsetsAndSize,
    qt_meta_data_InputWorker,
    qt_static_metacall,
    nullptr,
qt_incomplete_metaTypeArray<qt_meta_stringdata_InputWorker_t
, QtPrivate::TypeAndForceComplete<InputWorker, std::true_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<QString, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<QString, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<int, std::false_type>, QtPrivate::TypeAndForceComplete<int, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<int, std::false_type>, QtPrivate::TypeAndForceComplete<int, std::false_type>



>,
    nullptr
} };


const QMetaObject *InputWorker::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *InputWorker::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_InputWorker.stringdata0))
        return static_cast<void*>(this);
    return QThread::qt_metacast(_clname);
}

int InputWorker::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QThread::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 4)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 4;
    }
    return _id;
}

// SIGNAL 0
void InputWorker::updateLastValUI(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void InputWorker::updateLastStatusUI(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void InputWorker::GameConnectionMade(int _t1, int _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void InputWorker::BoardConnectionMade(int _t1, int _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
