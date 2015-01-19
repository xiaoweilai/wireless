#include "dosendthread.h"
#include <windows.h>
#include <QtDebug>
#include <QDesktopWidget>


DoSendThread::DoSendThread(QObject *parent) :
    QThread(parent)
{

}

void DoSendThread::run()
{
    while(1)
    {
        qDebug() << "emit signal.....";
        emit emitMsgBoxSignal();
        Sleep(1000);
    }
}
