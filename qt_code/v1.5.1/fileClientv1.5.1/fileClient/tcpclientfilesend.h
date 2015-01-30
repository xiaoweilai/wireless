#ifndef TCPCLIENTFILESEND_H
#define TCPCLIENTFILESEND_H

#include <QMainWindow>
#include <QDialog>
#include <QtGui>
#include <QAbstractSocket>
#include <QTcpSocket>
#include <QHostAddress>


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
    void startTransfer();
    void parseImage();
    void updateClientProgress(qint64 numBytes);
    void displayErr(QAbstractSocket::SocketError socketError);
    void openFile();
    void aboutVer();//软件版本信息
    QString GetVersion(void);
    QImage grabframeGeometry();
    QImage grabDeskScreen();
    void ShutDownAll();
    QString ReadIpAddr();
    void SaveIpAddr(QString ipaddr);
    void PrintInfoToFile(QString str);

private:
    void deleteImgs(); //关闭后删除文件夹


private:
    Ui::tcpClientFileSend *ui;

    QTcpSocket tcpClient;

    qint64 TotalBytes;
    qint64 byteWritten;
    qint64 bytesToWrite;
    QString fileName;
    QFile *localFile;
    QByteArray outBlock;

    QTimer *timer;//发送定时器
    QVector<QImage> imageVec;
    QImage fileImage;
    quint64 picNametime;
    QBuffer buffer;

    QString dirname;//文件夹名称
    quint64 jpgnameNo;
    double time_total;//耗时时间
    QList<QString> namelst; //保存文件名列表
    QList<quint32> sizelst; //保存文件大小列表
    QByteArray outBlockFile;//文件字节序列
    quint8 sendDoneFlag;//发送结束标志
    enum{
        SEND_DONE = 0,
        SEND_ING,
        SEND_UNKOWN
    };

signals:
    void emitImgZeroSignal();//当namelst为空时，发送信号调用parseImage();
};

#endif // TCPCLIENTFILESEND_H
