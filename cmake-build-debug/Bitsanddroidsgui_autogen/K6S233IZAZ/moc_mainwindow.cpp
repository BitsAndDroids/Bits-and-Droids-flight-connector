/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../headers/mainwindow.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#include <QtCore/QList>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_MainWindow_t {
    QByteArrayData data[30];
    char stringdata0[387];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MainWindow_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MainWindow_t qt_meta_stringdata_MainWindow = {
    {
QT_MOC_LITERAL(0, 0, 10), // "MainWindow"
QT_MOC_LITERAL(1, 11, 16), // "closedOutputMenu"
QT_MOC_LITERAL(2, 28, 0), // ""
QT_MOC_LITERAL(3, 29, 17), // "closedOptionsMenu"
QT_MOC_LITERAL(4, 47, 15), // "updateLastValUI"
QT_MOC_LITERAL(5, 63, 7), // "lastVal"
QT_MOC_LITERAL(6, 71, 18), // "updateLastStatusUI"
QT_MOC_LITERAL(7, 90, 16), // "updateActiveCom1"
QT_MOC_LITERAL(8, 107, 14), // "QList<QString>"
QT_MOC_LITERAL(9, 122, 12), // "startPressed"
QT_MOC_LITERAL(10, 135, 4), // "mode"
QT_MOC_LITERAL(11, 140, 14), // "refreshPressed"
QT_MOC_LITERAL(12, 155, 11), // "stopPressed"
QT_MOC_LITERAL(13, 167, 10), // "addPressed"
QT_MOC_LITERAL(14, 178, 17), // "onUpdateLastValUI"
QT_MOC_LITERAL(15, 196, 20), // "onUpdateLastStatusUI"
QT_MOC_LITERAL(16, 217, 10), // "lastStatus"
QT_MOC_LITERAL(17, 228, 9), // "startMode"
QT_MOC_LITERAL(18, 238, 11), // "refreshComs"
QT_MOC_LITERAL(19, 250, 8), // "stopMode"
QT_MOC_LITERAL(20, 259, 6), // "addCom"
QT_MOC_LITERAL(21, 266, 23), // "on_updateButton_clicked"
QT_MOC_LITERAL(22, 290, 16), // "outputMenuClosed"
QT_MOC_LITERAL(23, 307, 16), // "optionMenuClosed"
QT_MOC_LITERAL(24, 324, 8), // "onfinish"
QT_MOC_LITERAL(25, 333, 14), // "QNetworkReply*"
QT_MOC_LITERAL(26, 348, 3), // "rep"
QT_MOC_LITERAL(27, 352, 14), // "convertComPort"
QT_MOC_LITERAL(28, 367, 11), // "std::string"
QT_MOC_LITERAL(29, 379, 7) // "comText"

    },
    "MainWindow\0closedOutputMenu\0\0"
    "closedOptionsMenu\0updateLastValUI\0"
    "lastVal\0updateLastStatusUI\0updateActiveCom1\0"
    "QList<QString>\0startPressed\0mode\0"
    "refreshPressed\0stopPressed\0addPressed\0"
    "onUpdateLastValUI\0onUpdateLastStatusUI\0"
    "lastStatus\0startMode\0refreshComs\0"
    "stopMode\0addCom\0on_updateButton_clicked\0"
    "outputMenuClosed\0optionMenuClosed\0"
    "onfinish\0QNetworkReply*\0rep\0convertComPort\0"
    "std::string\0comText"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MainWindow[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      20,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       9,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,  114,    2, 0x06 /* Public */,
       3,    0,  115,    2, 0x06 /* Public */,
       4,    1,  116,    2, 0x06 /* Public */,
       6,    1,  119,    2, 0x06 /* Public */,
       7,    1,  122,    2, 0x06 /* Public */,
       9,    1,  125,    2, 0x06 /* Public */,
      11,    1,  128,    2, 0x06 /* Public */,
      12,    1,  131,    2, 0x06 /* Public */,
      13,    1,  134,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      14,    1,  137,    2, 0x0a /* Public */,
      15,    1,  140,    2, 0x0a /* Public */,
      17,    1,  143,    2, 0x0a /* Public */,
      18,    1,  146,    2, 0x0a /* Public */,
      19,    1,  149,    2, 0x0a /* Public */,
      20,    1,  152,    2, 0x0a /* Public */,
      21,    0,  155,    2, 0x08 /* Private */,
      22,    0,  156,    2, 0x08 /* Private */,
      23,    0,  157,    2, 0x08 /* Private */,
      24,    1,  158,    2, 0x08 /* Private */,
      27,    1,  161,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    5,
    QMetaType::Void, QMetaType::QString,    5,
    QMetaType::Void, 0x80000000 | 8,    5,
    QMetaType::Void, QMetaType::Int,   10,
    QMetaType::Void, QMetaType::Int,   10,
    QMetaType::Void, QMetaType::Int,   10,
    QMetaType::Void, QMetaType::Int,   10,

 // slots: parameters
    QMetaType::Void, QMetaType::QString,    5,
    QMetaType::Void, QMetaType::QString,   16,
    QMetaType::Void, QMetaType::Int,   10,
    QMetaType::Void, QMetaType::Int,   10,
    QMetaType::Void, QMetaType::Int,   10,
    QMetaType::Void, QMetaType::Int,   10,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 25,   26,
    0x80000000 | 28, QMetaType::QString,   29,

       0        // eod
};

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<MainWindow *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->closedOutputMenu(); break;
        case 1: _t->closedOptionsMenu(); break;
        case 2: _t->updateLastValUI((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 3: _t->updateLastStatusUI((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 4: _t->updateActiveCom1((*reinterpret_cast< QList<QString>(*)>(_a[1]))); break;
        case 5: _t->startPressed((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 6: _t->refreshPressed((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 7: _t->stopPressed((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 8: _t->addPressed((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 9: _t->onUpdateLastValUI((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 10: _t->onUpdateLastStatusUI((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 11: _t->startMode((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 12: _t->refreshComs((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 13: _t->stopMode((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 14: _t->addCom((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 15: _t->on_updateButton_clicked(); break;
        case 16: _t->outputMenuClosed(); break;
        case 17: _t->optionMenuClosed(); break;
        case 18: _t->onfinish((*reinterpret_cast< QNetworkReply*(*)>(_a[1]))); break;
        case 19: { std::string _r = _t->convertComPort((*reinterpret_cast< QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< std::string*>(_a[0]) = std::move(_r); }  break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 4:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QList<QString> >(); break;
            }
            break;
        case 18:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QNetworkReply* >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (MainWindow::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MainWindow::closedOutputMenu)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (MainWindow::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MainWindow::closedOptionsMenu)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (MainWindow::*)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MainWindow::updateLastValUI)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (MainWindow::*)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MainWindow::updateLastStatusUI)) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (MainWindow::*)(QList<QString> );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MainWindow::updateActiveCom1)) {
                *result = 4;
                return;
            }
        }
        {
            using _t = void (MainWindow::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MainWindow::startPressed)) {
                *result = 5;
                return;
            }
        }
        {
            using _t = void (MainWindow::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MainWindow::refreshPressed)) {
                *result = 6;
                return;
            }
        }
        {
            using _t = void (MainWindow::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MainWindow::stopPressed)) {
                *result = 7;
                return;
            }
        }
        {
            using _t = void (MainWindow::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MainWindow::addPressed)) {
                *result = 8;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject MainWindow::staticMetaObject = { {
    QMetaObject::SuperData::link<QMainWindow::staticMetaObject>(),
    qt_meta_stringdata_MainWindow.data,
    qt_meta_data_MainWindow,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_MainWindow.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 20)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 20;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 20)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 20;
    }
    return _id;
}

// SIGNAL 0
void MainWindow::closedOutputMenu()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void MainWindow::closedOptionsMenu()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void MainWindow::updateLastValUI(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void MainWindow::updateLastStatusUI(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void MainWindow::updateActiveCom1(QList<QString> _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void MainWindow::startPressed(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void MainWindow::refreshPressed(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}

// SIGNAL 7
void MainWindow::stopPressed(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 7, _a);
}

// SIGNAL 8
void MainWindow::addPressed(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 8, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
