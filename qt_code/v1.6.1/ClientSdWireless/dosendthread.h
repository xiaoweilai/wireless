#ifndef DOSENDTHREAD_H
#define DOSENDTHREAD_H

#include <QThread>
#include <QtGui>

class DoSendThread : public QThread
{
    Q_OBJECT
public:
    explicit DoSendThread(QObject *parent = 0);
    
protected:
    void run();
signals:
    void emitMsgBoxSignal();
public slots:
    
};

#endif // DOSENDTHREAD_H
