/****************************************************************************
** Meta object code from reading C++ file 'outputmenu.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.2.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../settings/outputmenu.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'outputmenu.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.2.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_OutputMenu_t {
    const uint offsetsAndSize[24];
    char stringdata0[113];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(offsetof(qt_meta_stringdata_OutputMenu_t, stringdata0) + ofs), len 
static const qt_meta_stringdata_OutputMenu_t qt_meta_stringdata_OutputMenu = {
    {
QT_MOC_LITERAL(0, 10), // "OutputMenu"
QT_MOC_LITERAL(11, 16), // "closedOutputMenu"
QT_MOC_LITERAL(28, 0), // ""
QT_MOC_LITERAL(29, 6), // "addSet"
QT_MOC_LITERAL(36, 9), // "setEdited"
QT_MOC_LITERAL(46, 2), // "id"
QT_MOC_LITERAL(49, 10), // "saveEdited"
QT_MOC_LITERAL(60, 9), // "removeSet"
QT_MOC_LITERAL(70, 9), // "addNewSet"
QT_MOC_LITERAL(80, 15), // "removeSetAction"
QT_MOC_LITERAL(96, 7), // "editSet"
QT_MOC_LITERAL(104, 8) // "saveEdit"

    },
    "OutputMenu\0closedOutputMenu\0\0addSet\0"
    "setEdited\0id\0saveEdited\0removeSet\0"
    "addNewSet\0removeSetAction\0editSet\0"
    "saveEdit"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_OutputMenu[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       5,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    0,   68,    2, 0x06,    1 /* Public */,
       3,    0,   69,    2, 0x06,    2 /* Public */,
       4,    1,   70,    2, 0x06,    3 /* Public */,
       6,    0,   73,    2, 0x06,    5 /* Public */,
       7,    1,   74,    2, 0x06,    6 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       8,    0,   77,    2, 0x0a,    8 /* Public */,
       9,    1,   78,    2, 0x0a,    9 /* Public */,
      10,    1,   81,    2, 0x0a,   11 /* Public */,
      11,    0,   84,    2, 0x0a,   13 /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    5,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    5,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    5,
    QMetaType::Void, QMetaType::QString,    5,
    QMetaType::Void,

       0        // eod
};

void OutputMenu::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<OutputMenu *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->closedOutputMenu(); break;
        case 1: _t->addSet(); break;
        case 2: _t->setEdited((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 3: _t->saveEdited(); break;
        case 4: _t->removeSet((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 5: _t->addNewSet(); break;
        case 6: _t->removeSetAction((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 7: _t->editSet((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 8: _t->saveEdit(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (OutputMenu::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&OutputMenu::closedOutputMenu)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (OutputMenu::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&OutputMenu::addSet)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (OutputMenu::*)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&OutputMenu::setEdited)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (OutputMenu::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&OutputMenu::saveEdited)) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (OutputMenu::*)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&OutputMenu::removeSet)) {
                *result = 4;
                return;
            }
        }
    }
}

const QMetaObject OutputMenu::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_OutputMenu.offsetsAndSize,
    qt_meta_data_OutputMenu,
    qt_static_metacall,
    nullptr,
qt_incomplete_metaTypeArray<qt_meta_stringdata_OutputMenu_t
, QtPrivate::TypeAndForceComplete<OutputMenu, std::true_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<QString, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<QString, std::false_type>
, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<QString, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<QString, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>


>,
    nullptr
} };


const QMetaObject *OutputMenu::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *OutputMenu::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_OutputMenu.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int OutputMenu::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 9)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 9;
    }
    return _id;
}

// SIGNAL 0
void OutputMenu::closedOutputMenu()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void OutputMenu::addSet()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void OutputMenu::setEdited(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void OutputMenu::saveEdited()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}

// SIGNAL 4
void OutputMenu::removeSet(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
