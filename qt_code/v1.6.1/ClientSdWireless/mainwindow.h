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

    /* socket 变量 */
    QTcpSocket *tcpClient;
    /* 传输相关变量 */
    quint64 picNametime;
    qint64 TotalBytes;
    qint64 byteWritten;
    QList<qint64> totalbyteslist;
    QList<qint64> bytesToWritelist;
    qint64 bytesToWrite;
    enum
    {
        RET_SUC,
        RET_ERR,
        ERT_UNKNOWN
    };

    QString m_connedipaddr; //已经连接的ip地址
    QBuffer buffer;
    QByteArray outBlock;
    QList<QByteArray> outBlocklst;
    float time_total;

    float time_totaltfs;

    int m_transfered;

    enum
    {
        STAT_USED,
        STAT_NOUSE,
        STAT_UNKNOWN
    };
    quint64 namecounter;
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
    void transferno();
    void transfered();
    int gettransfered();
private slots:
    void receiveMsgBoxSignal();
    void grabScreenSignal();
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
