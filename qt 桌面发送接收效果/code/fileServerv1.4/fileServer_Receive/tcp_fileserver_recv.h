#ifndef TCP_FILESERVER_RECV_H
#define TCP_FILESERVER_RECV_H

#include <QMainWindow>
#include <QtGui>
#include <QTcpServer>
#include <QTcpSocket>
#include <QAbstractSocket>
#include <QIODevice>

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
class Tcp_FileServer_Recv;
}

class Tcp_FileServer_Recv : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit Tcp_FileServer_Recv(QWidget *parent = 0);
    ~Tcp_FileServer_Recv();
    
private:
    Ui::Tcp_FileServer_Recv *ui;

public slots:
    void start();
    void acceptConnection();
    void updateServerProgress();
    void displayError(QAbstractSocket::SocketError socketError);

private:
//    QProgressBar *clientProgressBar;
//    QProgressBar *serverProgressBar;
//    QLabel *serverStatusLabel;
//    QPushButton *startButton;
//    QPushButton *quitButton;
//    QPushButton *openButton;
//    QDialogButtonBox *buttonBox;

    QTcpServer tcpServer;
    QTcpSocket *tcpServerConnection;
    qint64  TotalBytes;
    qint64  bytesReceived;
    qint64  fileNameSize;
    QString fileName;
    QFile *localFile;
    QByteArray inBlock;

};

#endif // TCP_FILESERVER_RECV_H
