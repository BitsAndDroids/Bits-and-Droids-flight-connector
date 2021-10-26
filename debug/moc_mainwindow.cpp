/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.2.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../headers/mainwindow.h"
#include <QtGui/qtextcursor.h>
#include <QScreen>
#include <QtCharts/qlineseries.h>
#include <QtCharts/qabstractbarseries.h>
#include <QtCharts/qvbarmodelmapper.h>
#include <QtCharts/qboxplotseries.h>
#include <QtCharts/qcandlestickseries.h>
#include <QtCore/qabstractitemmodel.h>
#include <QtCharts/qpieseries.h>
#include <QtCore/qabstractitemmodel.h>
#include <QtCharts/qboxplotseries.h>
#include <QtCore/qabstractitemmodel.h>
#include <QtCharts/qpieseries.h>
#include <QtCharts/qpieseries.h>
#include <QtCore/qabstractitemmodel.h>
#include <QtCharts/qxyseries.h>
#include <QtCharts/qxyseries.h>
#include <QtCore/qabstractitemmodel.h>
#include <QtCore/qabstractitemmodel.h>
#include <QtCharts/qboxplotseries.h>
#include <QtCore/qabstractitemmodel.h>
#include <QtCharts/qpieseries.h>
#include <QtCore/qabstractitemmodel.h>
#include <QtCharts/qxyseries.h>
#include <QtCore/qabstractitemmodel.h>
#include <QtNetwork/QSslError>
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#include <QtCore/QList>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.2.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_MainWindow_t {
    const uint offsetsAndSize[58];
    char stringdata0[372];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(offsetof(qt_meta_stringdata_MainWindow_t, stringdata0) + ofs), len 
static const qt_meta_stringdata_MainWindow_t qt_meta_stringdata_MainWindow = {
    {
QT_MOC_LITERAL(0, 10), // "MainWindow"
QT_MOC_LITERAL(11, 16), // "closedOutputMenu"
QT_MOC_LITERAL(28, 0), // ""
QT_MOC_LITERAL(29, 17), // "closedOptionsMenu"
QT_MOC_LITERAL(47, 15), // "updateLastValUI"
QT_MOC_LITERAL(63, 7), // "lastVal"
QT_MOC_LITERAL(71, 18), // "updateLastStatusUI"
QT_MOC_LITERAL(90, 16), // "updateActiveCom1"
QT_MOC_LITERAL(107, 12), // "startPressed"
QT_MOC_LITERAL(120, 4), // "mode"
QT_MOC_LITERAL(125, 14), // "refreshPressed"
QT_MOC_LITERAL(140, 11), // "stopPressed"
QT_MOC_LITERAL(152, 10), // "addPressed"
QT_MOC_LITERAL(163, 17), // "onUpdateLastValUI"
QT_MOC_LITERAL(181, 20), // "onUpdateLastStatusUI"
QT_MOC_LITERAL(202, 10), // "lastStatus"
QT_MOC_LITERAL(213, 9), // "startMode"
QT_MOC_LITERAL(223, 11), // "refreshComs"
QT_MOC_LITERAL(235, 8), // "stopMode"
QT_MOC_LITERAL(244, 6), // "addCom"
QT_MOC_LITERAL(251, 23), // "on_updateButton_clicked"
QT_MOC_LITERAL(275, 16), // "outputMenuClosed"
QT_MOC_LITERAL(292, 16), // "optionMenuClosed"
QT_MOC_LITERAL(309, 8), // "onfinish"
QT_MOC_LITERAL(318, 14), // "QNetworkReply*"
QT_MOC_LITERAL(333, 3), // "rep"
QT_MOC_LITERAL(337, 14), // "convertComPort"
QT_MOC_LITERAL(352, 11), // "std::string"
QT_MOC_LITERAL(364, 7) // "comText"

    },
    "MainWindow\0closedOutputMenu\0\0"
    "closedOptionsMenu\0updateLastValUI\0"
    "lastVal\0updateLastStatusUI\0updateActiveCom1\0"
    "startPressed\0mode\0refreshPressed\0"
    "stopPressed\0addPressed\0onUpdateLastValUI\0"
    "onUpdateLastStatusUI\0lastStatus\0"
    "startMode\0refreshComs\0stopMode\0addCom\0"
    "on_updateButton_clicked\0outputMenuClosed\0"
    "optionMenuClosed\0onfinish\0QNetworkReply*\0"
    "rep\0convertComPort\0std::string\0comText"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MainWindow[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
      20,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       9,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    0,  134,    2, 0x06,    1 /* Public */,
       3,    0,  135,    2, 0x06,    2 /* Public */,
       4,    1,  136,    2, 0x06,    3 /* Public */,
       6,    1,  139,    2, 0x06,    5 /* Public */,
       7,    1,  142,    2, 0x06,    7 /* Public */,
       8,    1,  145,    2, 0x06,    9 /* Public */,
      10,    1,  148,    2, 0x06,   11 /* Public */,
      11,    1,  151,    2, 0x06,   13 /* Public */,
      12,    1,  154,    2, 0x06,   15 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
      13,    1,  157,    2, 0x0a,   17 /* Public */,
      14,    1,  160,    2, 0x0a,   19 /* Public */,
      16,    1,  163,    2, 0x0a,   21 /* Public */,
      17,    1,  166,    2, 0x0a,   23 /* Public */,
      18,    1,  169,    2, 0x0a,   25 /* Public */,
      19,    1,  172,    2, 0x0a,   27 /* Public */,
      20,    0,  175,    2, 0x08,   29 /* Private */,
      21,    0,  176,    2, 0x08,   30 /* Private */,
      22,    0,  177,    2, 0x08,   31 /* Private */,
      23,    1,  178,    2, 0x08,   32 /* Private */,
      26,    1,  181,    2, 0x08,   34 /* Private */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    5,
    QMetaType::Void, QMetaType::QString,    5,
    QMetaType::Void, QMetaType::QStringList,    5,
    QMetaType::Void, QMetaType::Int,    9,
    QMetaType::Void, QMetaType::Int,    9,
    QMetaType::Void, QMetaType::Int,    9,
    QMetaType::Void, QMetaType::Int,    9,

 // slots: parameters
    QMetaType::Void, QMetaType::QString,    5,
    QMetaType::Void, QMetaType::QString,   15,
    QMetaType::Void, QMetaType::Int,    9,
    QMetaType::Void, QMetaType::Int,    9,
    QMetaType::Void, QMetaType::Int,    9,
    QMetaType::Void, QMetaType::Int,    9,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 24,   25,
    0x80000000 | 27, QMetaType::QString,   28,

       0        // eod
};

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<MainWindow *>(_o);
        (void)_t;
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
        default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
        case 18:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 0:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< QNetworkReply* >(); break;
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

const QMetaObject MainWindow::staticMetaObject = { {
    QMetaObject::SuperData::link<QMainWindow::staticMetaObject>(),
    qt_meta_stringdata_MainWindow.offsetsAndSize,
    qt_meta_data_MainWindow,
    qt_static_metacall,
    nullptr,
qt_incomplete_metaTypeArray<qt_meta_stringdata_MainWindow_t
, QtPrivate::TypeAndForceComplete<MainWindow, std::true_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<QString, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<QString, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<QList<QString>, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<int, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<int, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<int, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<int, std::false_type>
, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<const QString &, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<const QString &, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<int, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<int, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<int, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<int, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<QNetworkReply *, std::false_type>, QtPrivate::TypeAndForceComplete<std::string, std::false_type>, QtPrivate::TypeAndForceComplete<QString, std::false_type>


>,
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
