/****************************************************************************
** Meta object code from reading C++ file 'tcpclientfilesend.h'
**
** Created: Fri Feb 27 20:07:52 2015
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
      17,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      19,   18,   18,   18, 0x05,

 // slots: signature, parameters, type, tag, flags
      39,   18,   18,   18, 0x0a,
      47,   18,   18,   18, 0x0a,
      55,   18,   18,   18, 0x0a,
      71,   18,   18,   18, 0x0a,
      93,   84,   18,   18, 0x0a,
     134,  122,   18,   18, 0x0a,
     175,   18,   18,   18, 0x0a,
     186,   18,   18,   18, 0x0a,
     205,   18,  197,   18, 0x0a,
     225,   18,  218,   18, 0x0a,
     245,   18,  218,   18, 0x0a,
     262,   18,   18,   18, 0x0a,
     276,   18,   18,   18, 0x0a,
     295,   18,  197,   18, 0x0a,
     315,  308,   18,   18, 0x0a,
     339,  335,   18,   18, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_tcpClientFileSend[] = {
    "tcpClientFileSend\0\0emitImgZeroSignal()\0"
    "start()\0pause()\0startTransfer()\0"
    "parseImage()\0numBytes\0"
    "updateClientProgress(qint64)\0socketError\0"
    "displayErr(QAbstractSocket::SocketError)\0"
    "openFile()\0aboutVer()\0QString\0"
    "GetVersion()\0QImage\0grabframeGeometry()\0"
    "grabDeskScreen()\0ShutDownAll()\0"
    "disconnectSocket()\0ReadIpAddr()\0ipaddr\0"
    "SaveIpAddr(QString)\0str\0"
    "PrintInfoToFile(QString)\0"
};

void tcpClientFileSend::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        tcpClientFileSend *_t = static_cast<tcpClientFileSend *>(_o);
        switch (_id) {
        case 0: _t->emitImgZeroSignal(); break;
        case 1: _t->start(); break;
        case 2: _t->pause(); break;
        case 3: _t->startTransfer(); break;
        case 4: _t->parseImage(); break;
        case 5: _t->updateClientProgress((*reinterpret_cast< qint64(*)>(_a[1]))); break;
        case 6: _t->displayErr((*reinterpret_cast< QAbstractSocket::SocketError(*)>(_a[1]))); break;
        case 7: _t->openFile(); break;
        case 8: _t->aboutVer(); break;
        case 9: { QString _r = _t->GetVersion();
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = _r; }  break;
        case 10: { QImage _r = _t->grabframeGeometry();
            if (_a[0]) *reinterpret_cast< QImage*>(_a[0]) = _r; }  break;
        case 11: { QImage _r = _t->grabDeskScreen();
            if (_a[0]) *reinterpret_cast< QImage*>(_a[0]) = _r; }  break;
        case 12: _t->ShutDownAll(); break;
        case 13: _t->disconnectSocket(); break;
        case 14: { QString _r = _t->ReadIpAddr();
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = _r; }  break;
        case 15: _t->SaveIpAddr((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 16: _t->PrintInfoToFile((*reinterpret_cast< QString(*)>(_a[1]))); break;
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
        if (_id < 17)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 17;
    }
    return _id;
}

// SIGNAL 0
void tcpClientFileSend::emitImgZeroSignal()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}
QT_END_MOC_NAMESPACE
