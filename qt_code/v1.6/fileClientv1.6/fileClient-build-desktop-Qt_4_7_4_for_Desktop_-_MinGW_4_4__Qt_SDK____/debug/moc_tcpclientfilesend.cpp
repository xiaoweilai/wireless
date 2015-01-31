/****************************************************************************
** Meta object code from reading C++ file 'tcpclientfilesend.h'
**
** Created: Sun Jan 18 18:20:08 2015
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../fileClient/tcpclientfilesend.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'tcpclientfilesend.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_tcpClientFileSend[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
      12,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      19,   18,   18,   18, 0x0a,
      27,   18,   18,   18, 0x0a,
      38,   18,   18,   18, 0x0a,
      63,   54,   18,   18, 0x0a,
     104,   92,   18,   18, 0x0a,
     145,   18,   18,   18, 0x0a,
     164,   18,  157,   18, 0x0a,
     184,   18,  157,   18, 0x0a,
     205,  201,  157,   18, 0x0a,
     233,   18,   18,   18, 0x0a,
     255,   18,  247,   18, 0x0a,
     272,  268,   18,   18, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_tcpClientFileSend[] = {
    "tcpClientFileSend\0\0start()\0pauseOps()\0"
    "startTransfer()\0numBytes\0"
    "updateClientProgress(qint64)\0socketError\0"
    "displayErr(QAbstractSocket::SocketError)\0"
    "saveImage()\0QImage\0grabframeGeometry()\0"
    "grabDeskScreen()\0img\0covertPixTo1024768(QImage&)\0"
    "ShutDownAll()\0QString\0ReadIpAddr()\0"
    "str\0PrintInfoToFile(QString)\0"
};

const QMetaObject tcpClientFileSend::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_tcpClientFileSend,
      qt_meta_data_tcpClientFileSend, 0 }
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
        switch (_id) {
        case 0: start(); break;
        case 1: pauseOps(); break;
        case 2: startTransfer(); break;
        case 3: updateClientProgress((*reinterpret_cast< qint64(*)>(_a[1]))); break;
        case 4: displayErr((*reinterpret_cast< QAbstractSocket::SocketError(*)>(_a[1]))); break;
        case 5: saveImage(); break;
        case 6: { QImage _r = grabframeGeometry();
            if (_a[0]) *reinterpret_cast< QImage*>(_a[0]) = _r; }  break;
        case 7: { QImage _r = grabDeskScreen();
            if (_a[0]) *reinterpret_cast< QImage*>(_a[0]) = _r; }  break;
        case 8: { QImage _r = covertPixTo1024768((*reinterpret_cast< QImage(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QImage*>(_a[0]) = _r; }  break;
        case 9: ShutDownAll(); break;
        case 10: { QString _r = ReadIpAddr();
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = _r; }  break;
        case 11: PrintInfoToFile((*reinterpret_cast< QString(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 12;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
