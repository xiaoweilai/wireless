#ifndef THREADSD_H
#define THREADSD_H

#include <QThread>
#include <QtGui>
#include <QTcpSocket>
//#include "tcpclientfilesend.h"

class tcpClientFileSend;

class threadSd : public QThread
{
    Q_OBJECT
public:
    explicit threadSd(QObject *parent = 0/* , void *tcpClient*/);
    void getImage(QImage  img, void *tcpclient);
    void PrintInfoToFile(QString str);
protected:
    void run();
    QImage covertPixTo1024768(QImage & img);
signals:
    
public slots:


public:
    QTcpSocket *mp_client;//

private:
    quint64 counter;

    qint64 TotalBytes;
    quint64 picNametime;
    qint64 bytesToWrite;


    QBuffer buffer;//数据buffer
    QByteArray outBlock;
    QImage  rvimg; //接收到的图片
    QVector<QImage> imgset;


};

#endif // THREADSD_H
