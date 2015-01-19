#ifndef TCPCLIENTFILESEND_H
#define TCPCLIENTFILESEND_H

#include <QMainWindow>
#include <QDialog>
#include <QtGui>
#include <QAbstractSocket>
#include <QTcpSocket>
#include <QHostAddress>
#include "threadsd.h" //发送线程


#if 0 //ok 2pic/s
#define STREAM_PIC_FORT "PNG"
#define SUFIXNAME       "png"
#elif 0 //err
#define STREAM_PIC_FORT "BMP"
#define SUFIXNAME       "bmp"
#elif 0  //ok, 6pic/s
#define STREAM_PIC_FORT "JPG"
#define SUFIXNAME       "jpg"
#elif 1 //ok, 6pic/s
#define STREAM_PIC_FORT "JPEG"
#define SUFIXNAME       "jpeg"
#elif 0 //err,size too little
#define STREAM_PIC_FORT "GIF"
#define SUFIXNAME       "gif"
#elif 0 //err,too big
#define STREAM_PIC_FORT "TIFF"
#define SUFIXNAME       "tiff"
#elif 1 //not show
#define STREAM_PIC_FORT "PPM"
#define SUFIXNAME       "ppm"
#endif


//class threadSd;

namespace Ui {
class tcpClientFileSend;
}

class tcpClientFileSend : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit tcpClientFileSend(QWidget *parent = 0);
    ~tcpClientFileSend();



public slots:
    void start();
    void pauseOps();//暂停
    void startTransfer();
    void updateClientProgress(qint64 numBytes);
    void displayErr(QAbstractSocket::SocketError socketError);
    void saveImage();
    QImage grabframeGeometry();
    QImage grabDeskScreen();
    QImage covertPixTo1024768(QImage & img);
    void ShutDownAll();
    QString ReadIpAddr();
    void PrintInfoToFile(QString str);

public:
    QTcpSocket *tcpClient;

private:
    Ui::tcpClientFileSend *ui;

    threadSd *threadsd;//发送数据线程


    qint64 TotalBytes;
    qint64 byteWritten;
    qint64 bytesToWrite;
    qint64 loadSize;
    QString fileName;
    QFile *localFile;
    QByteArray outBlock;

    QTimer *timer;//发送定时器
    QVector<QImage> imageVec;
    QImage fileImage;
    quint64 picNametime;
    QBuffer buffer;
};

#endif // TCPCLIENTFILESEND_H
