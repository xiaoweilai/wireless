/****************************************************************************
** Meta object code from reading C++ file 'tcpclientfilesend.h'
**
** Created: Fri Jan 30 00:52:31 2015
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../fileClient/tcpclientfilesend.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'tcpclientfilesend.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_tcpClientFileSend[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      11,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      19,   18,   18,   18, 0x0a,
      27,   18,   18,   18, 0x0a,
      52,   43,   18,   18, 0x0a,
      93,   81,   18,   18, 0x0a,
     134,   18,   18,   18, 0x0a,
     152,   18,  145,   18, 0x0a,
     172,   18,  145,   18, 0x0a,
     189,   18,   18,   18, 0x0a,
     211,   18,  203,   18, 0x0a,
     231,  224,   18,   18, 0x0a,
     255,  251,   18,   18, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_tcpClientFileSend[] = {
    "tcpClientFileSend\0\0start()\0startTransfer()\0"
    "numBytes\0updateClientProgress(qint64)\0"
    "socketError\0displayErr(QAbstractSocket::SocketError)\0"
    "openFile()\0QImage\0grabframeGeometry()\0"
    "grabDeskScreen()\0ShutDownAll()\0QString\0"
    "ReadIpAddr()\0ipaddr\0SaveIpAddr(QString)\0"
    "str\0PrintInfoToFile(QString)\0"
};

void tcpClientFileSend::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        tcpClientFileSend *_t = static_cast<tcpClientFileSend *>(_o);
        switch (_id) {
        case 0: _t->start(); break;
        case 1: _t->startTransfer(); break;
        case 2: _t->updateClientProgress((*reinterpret_cast< qint64(*)>(_a[1]))); break;
        case 3: _t->displayErr((*reinterpret_cast< QAbstractSocket::SocketError(*)>(_a[1]))); break;
        case 4: _t->openFile(); break;
        case 5: { QImage _r = _t->grabframeGeometry();
            if (_a[0]) *reinterpret_cast< QImage*>(_a[0]) = _r; }  break;
        case 6: { QImage _r = _t->grabDeskScreen();
            if (_a[0]) *reinterpret_cast< QImage*>(_a[0]) = _r; }  break;
        case 7: _t->ShutDownAll(); break;
        case 8: { QString _r = _t->ReadIpAddr();
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = _r; }  break;
        case 9: _t->SaveIpAddr((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 10: _t->PrintInfoToFile((*reinterpret_cast< QString(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData tcpClientFileSend::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject tcpClientFileSend::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_tcpClientFileSend,
      qt_meta_data_tcpClientFileSend, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &tcpClientFileSend::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *tcpClientFileSend::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *tcpClientFileSend::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_tcpClientFileSend))
        return static_cast<void*>(const_cast< tcpClientFileSend*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int tcpClientFileSend::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 11)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 11;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
