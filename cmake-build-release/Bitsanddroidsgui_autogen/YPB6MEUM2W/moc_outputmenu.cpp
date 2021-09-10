/****************************************************************************
** Meta object code from reading C++ file 'outputmenu.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../settings/outputmenu.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'outputmenu.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_OutputMenu_t {
    QByteArrayData data[11];
    char stringdata0[96];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_OutputMenu_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_OutputMenu_t qt_meta_stringdata_OutputMenu = {
    {
QT_MOC_LITERAL(0, 0, 10), // "OutputMenu"
QT_MOC_LITERAL(1, 11, 6), // "addSet"
QT_MOC_LITERAL(2, 18, 0), // ""
QT_MOC_LITERAL(3, 19, 9), // "setEdited"
QT_MOC_LITERAL(4, 29, 2), // "id"
QT_MOC_LITERAL(5, 32, 10), // "saveEdited"
QT_MOC_LITERAL(6, 43, 9), // "removeSet"
QT_MOC_LITERAL(7, 53, 9), // "addNewSet"
QT_MOC_LITERAL(8, 63, 15), // "removeSetAction"
QT_MOC_LITERAL(9, 79, 7), // "editSet"
QT_MOC_LITERAL(10, 87, 8) // "saveEdit"

    },
    "OutputMenu\0addSet\0\0setEdited\0id\0"
    "saveEdited\0removeSet\0addNewSet\0"
    "removeSetAction\0editSet\0saveEdit"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_OutputMenu[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   54,    2, 0x06 /* Public */,
       3,    1,   55,    2, 0x06 /* Public */,
       5,    0,   58,    2, 0x06 /* Public */,
       6,    1,   59,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       7,    0,   62,    2, 0x0a /* Public */,
       8,    1,   63,    2, 0x0a /* Public */,
       9,    1,   66,    2, 0x0a /* Public */,
      10,    0,   69,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    4,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    4,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    4,
    QMetaType::Void, QMetaType::QString,    4,
    QMetaType::Void,

       0        // eod
};

void OutputMenu::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<OutputMenu *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->addSet(); break;
        case 1: _t->setEdited((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 2: _t->saveEdited(); break;
        case 3: _t->removeSet((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 4: _t->addNewSet(); break;
        case 5: _t->removeSetAction((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 6: _t->editSet((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 7: _t->saveEdit(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (OutputMenu::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&OutputMenu::addSet)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (OutputMenu::*)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&OutputMenu::setEdited)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (OutputMenu::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&OutputMenu::saveEdited)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (OutputMenu::*)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&OutputMenu::removeSet)) {
                *result = 3;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject OutputMenu::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_OutputMenu.data,
    qt_meta_data_OutputMenu,
    qt_static_metacall,
    nullptr,
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
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 8)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 8;
    }
    return _id;
}

// SIGNAL 0
void OutputMenu::addSet()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void OutputMenu::setEdited(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void OutputMenu::saveEdited()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void OutputMenu::removeSet(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
