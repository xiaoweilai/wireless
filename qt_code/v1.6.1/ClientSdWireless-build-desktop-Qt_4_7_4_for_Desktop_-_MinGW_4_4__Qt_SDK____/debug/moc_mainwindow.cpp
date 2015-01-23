/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created: Thu Jan 22 23:53:54 2015
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../ClientSdWireless/mainwindow.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_MainWindow[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
      15,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: signature, parameters, type, tag, flags
      12,   11,   11,   11, 0x05,
      31,   11,   11,   11, 0x05,

 // slots: signature, parameters, type, tag, flags
      45,   11,   11,   11, 0x08,
      67,   11,   11,   11, 0x08,
      86,   11,   11,   11, 0x08,
     103,   11,   11,   11, 0x08,
     117,   11,   11,   11, 0x08,
     131,   11,   11,   11, 0x08,
     144,   11,   11,   11, 0x08,
     160,   11,   11,   11, 0x08,
     186,  174,   11,   11, 0x08,
     227,   11,   11,   11, 0x08,
     242,   11,   11,   11, 0x08,
     265,  256,   11,   11, 0x08,
     294,   11,   11,   11, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_MainWindow[] = {
    "MainWindow\0\0removelistonesig()\0"
    "transfersig()\0receiveMsgBoxSignal()\0"
    "grabScreenSignal()\0savetimertobuf()\0"
    "startActive()\0pauseActive()\0quitActive()\0"
    "startTransfer()\0transferjpg()\0socketError\0"
    "displayErr(QAbstractSocket::SocketError)\0"
    "startConnect()\0stopConnect()\0numBytes\0"
    "updateClientProgress(qint64)\0removelist()\0"
};

const QMetaObject MainWindow::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_MainWindow,
      qt_meta_data_MainWindow, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &MainWindow::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_MainWindow))
        return static_cast<void*>(const_cast< MainWindow*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: removelistonesig(); break;
        case 1: transfersig(); break;
        case 2: receiveMsgBoxSignal(); break;
        case 3: grabScreenSignal(); break;
        case 4: savetimertobuf(); break;
        case 5: startActive(); break;
        case 6: pauseActive(); break;
        case 7: quitActive(); break;
        case 8: startTransfer(); break;
        case 9: transferjpg(); break;
        case 10: displayErr((*reinterpret_cast< QAbstractSocket::SocketError(*)>(_a[1]))); break;
        case 11: startConnect(); break;
        case 12: stopConnect(); break;
        case 13: updateClientProgress((*reinterpret_cast< qint64(*)>(_a[1]))); break;
        case 14: removelist(); break;
        default: ;
        }
        _id -= 15;
    }
    return _id;
}

// SIGNAL 0
void MainWindow::removelistonesig()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void MainWindow::transfersig()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}
QT_END_MOC_NAMESPACE
