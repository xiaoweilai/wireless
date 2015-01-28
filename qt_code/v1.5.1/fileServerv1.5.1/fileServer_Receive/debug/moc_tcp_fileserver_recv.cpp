/****************************************************************************
** Meta object code from reading C++ file 'tcp_fileserver_recv.h'
**
** Created: Mon Jan 26 20:31:23 2015
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../tcp_fileserver_recv.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'tcp_fileserver_recv.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Tcp_FileServer_Recv[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      21,   20,   20,   20, 0x0a,
      29,   20,   20,   20, 0x0a,
      36,   20,   20,   20, 0x0a,
      55,   20,   20,   20, 0x0a,
      90,   78,   20,   20, 0x0a,
     141,   20,  133,   20, 0x0a,
     161,  154,   20,   20, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_Tcp_FileServer_Recv[] = {
    "Tcp_FileServer_Recv\0\0start()\0stop()\0"
    "acceptConnection()\0updateServerProgress()\0"
    "socketError\0displayError(QAbstractSocket::SocketError)\0"
    "QString\0bindIpAddr()\0ipaddr\0"
    "showifconfig(QString&)\0"
};

const QMetaObject Tcp_FileServer_Recv::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_Tcp_FileServer_Recv,
      qt_meta_data_Tcp_FileServer_Recv, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Tcp_FileServer_Recv::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Tcp_FileServer_Recv::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Tcp_FileServer_Recv::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Tcp_FileServer_Recv))
        return static_cast<void*>(const_cast< Tcp_FileServer_Recv*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int Tcp_FileServer_Recv::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: start(); break;
        case 1: stop(); break;
        case 2: acceptConnection(); break;
        case 3: updateServerProgress(); break;
        case 4: displayError((*reinterpret_cast< QAbstractSocket::SocketError(*)>(_a[1]))); break;
        case 5: { QString _r = bindIpAddr();
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = _r; }  break;
        case 6: showifconfig((*reinterpret_cast< QString(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 7;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
