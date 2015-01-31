#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QAbstractSocket>
#include <QTcpSocket>
#include <QHostAddress>
#include "dosendthread.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
private://var
    Ui::MainWindow *ui;
    DoSendThread *getImgThd;
    QList<QImage> imglist;
    QList<QTimer*> timerlist;
    QList<QTimer*> savetimerlist;

    /* socket 变量 */
    QTcpSocket *tcpClient;
    /* 传输相关变量 */
    quint64 picNametime;
    qint64 TotalBytes;
    qint64 byteWritten;
    qint64 bytesToWrite;
    enum
    {
        RET_SUC,
        RET_ERR,
        ERT_UNKNOWN
    };
    enum
    {
        FLAG_USED,
        FLAG_NOUSE,
        FLAG_UNKNOWN
    };

    QString m_connedipaddr; //已经连接的ip地址
    QBuffer buffer;
    quint8 flagbuf0;

    QBuffer buffer1;
    quint8 flagbuf1;

    QList<QBuffer> bufferlst;
    QBuffer buffertmp;
    QByteArray outBlock;
    float time_total;

    int m_transfered;

    enum
    {
        STAT_TRANSFERED,
        STAT_TRANSFERING,
        STAT_UNKNOWN
    };

//    QBuffer buffer1;
    QBuffer buffer2;
    QBuffer buffer3;
    QBuffer buffer4;
    QBuffer buffer5;
    QBuffer buffer6;
    QBuffer buffer7;
    QBuffer buffer8;
    QBuffer buffer9;
    QBuffer buffer10;
    quint8 bufused1;
    quint8 bufused2;
    quint8 bufused3;
    quint8 bufused4;
    quint8 bufused5;
    quint8 bufused6;
    quint8 bufused7;
    quint8 bufused8;
    quint8 bufused9;
    quint8 bufused10;

    enum
    {
        BUF_USED,
        BUF_NOUSE,
        BUF_UNKNOWN
    };

private://func
    QImage grabframeGeometry();
    QImage grabDeskScreen();
    QImage covertPixTo1024768(QImage img);
    void TimerSets();
    void ButtonSets();
    void SocketSets();
    void socketVarSets();
    quint32 getPort();
    QString getIP();
    void statusBarText(QString text);
    void PushBtnBegin();
    void PushBtnPause();
    void PushBtnAllFalse();
    void PushBtnRestart();
    void SignalSets();
    void transferring();
    void transfered();
    int gettransfered();
    void BufferSets();
private slots:
    void receiveMsgBoxSignal();
    void grabScreenSignal();
    void savetimertobuf();
    void startActive();
    void pauseActive();
    void quitActive();

    void startTransfer();
    void transferjpg();
    void displayErr(QAbstractSocket::SocketError socketError);
    void startConnect();
    void stopConnect();
    void updateClientProgress(qint64 numBytes);
    void removelist();
signals:
    void removelistonesig();
    void transfersig();
};

#endif // MAINWINDOW_H
