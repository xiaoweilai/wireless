#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDesktopWidget>
#include <QDesktopServices>
#include <QTime>
#include <QRegExp>

/*定时器个数*/
#define TIMERNUMS 100
#define SAVETIMERNUMS 100

//分辨率比此大，则转换为此分辨率
#define VGASTDHEIGHT 1024
#define VGASTDWIDTH  768

//打印调试信息
//#define DEBUGINFO
//转换分辨率,压缩
#define ZIPCONVERTPIX
//传输全屏幕
#define DESKSCREENTS

#if 1
#define DEFAULT_HOSTADDR "192.168.1.104"
#else
#define DEFAULT_HOSTADDR "127.0.0.1"
#endif
#define DEFAULT_PORT   "16689"



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


#if 1
//编码汉字
#define str_china(A)     QString::fromLocal8Bit(A)
#else
#define str_china(A)     codec->toUnicode(#A)
#endif


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

#if 1
    getImgThd = new DoSendThread(NULL);
    getImgThd->start();
    connect(getImgThd,SIGNAL(emitMsgBoxSignal()),this,SLOT(receiveMsgBoxSignal()),Qt::DirectConnection);
#endif

    TimerSets();
    ButtonSets();
    SocketSets();
    SignalSets();
    BufferSets();


}

void MainWindow::displayErr(QAbstractSocket::SocketError socketError)
{
    qDebug() << "socket err:" << socketError;
    //    if(socketError == QTcpSocket::RemoteHostClosedError)
    //        return;
    QMessageBox::information(this,str_china("网络"),
                             str_china("产生如下错误： %1")
                             .arg(tcpClient->errorString()));
    pauseActive();
    stopConnect();
    tcpClient->close();
    statusBarText(str_china("客户端就绪"));
    PushBtnRestart();
}

void MainWindow::transferjpg()
{
    if(!imglist.count())
        return;

    if(STAT_TRANSFERING != gettransfered())
        return;
    if((FLAG_NOUSE == flagbuf0) && (FLAG_NOUSE == flagbuf1))
        return;

//    if(0 == buffer.data().size())
//        return;
    QTime time;
    time.start(); //开始计时，以ms为单位



    transferring();

//    buffer.reset();
//    outBlock.resize(0);
#if 0
    qDebug() << "imlist count:" << imglist.count();
    imglist.at(0).save(&buffer,STREAM_PIC_FORT);
    imglist.removeFirst();
    qDebug() << "remove imlist count:" << imglist.count();
#elif 0
    img.save(&buffer,STREAM_PIC_FORT);
#endif



    /* 发送文件数据格式 ：
    总长度 (8bytes) | 文件名长度(qint64() 8Bytes) |由文件名长度指定
    */

    //图片大小
    if(FLAG_USED == flagbuf0)
        TotalBytes = (qint64)buffer.data().size();
    else if(FLAG_USED == flagbuf1)
        TotalBytes = (qint64)buffer1.data().size();


    QDataStream sendOut(&outBlock, QIODevice::ReadWrite);
    sendOut.resetStatus();
    sendOut.setVersion(QDataStream::Qt_4_0);
    QString currentFile = QString("blog%1.%2").arg(picNametime).arg(SUFIXNAME);
#ifdef DEBUGINFO
    qDebug() << "send pic name:"<<currentFile;
#endif
    //发送文件名称
    sendOut <<qint64(0) <<qint64(0) <<currentFile;
    //TotalBytes为总数据长度，即（数据量长度+文件名长度+文件名）
    TotalBytes += outBlock.size(); //加上图片名称长度
    sendOut.device()->seek(0);

    //发送文件总大小 ，
    sendOut << TotalBytes << qint64((outBlock.size() - sizeof(qint64)*2));

    bytesToWrite = TotalBytes - tcpClient->write(outBlock);//将名称发出后，剩余图片大小

    //    statusBarText(str_china("已连接"));
#ifdef DEBUGINFO
    qDebug() << currentFile << TotalBytes;
#endif
    outBlock.resize(0);


    int time_Diff = time.elapsed(); //返回从上次start()或restart()开始以来的时间差，单位ms
    qDebug() << "save to buffer elaspe:" <<time_Diff <<"ms";


}

void MainWindow::updateClientProgress(qint64 numBytes)
{
    qDebug() << "update bytes:" << numBytes;
    byteWritten += numBytes;
    if(bytesToWrite > 0)
    {
        if(FLAG_USED == flagbuf0)
        {
            outBlock =  buffer.data();
            bytesToWrite -= (int)tcpClient->write(outBlock);
            outBlock.resize(0);
        }
        else if(FLAG_USED == flagbuf1)
        {
            outBlock =  buffer1.data();
            bytesToWrite -= (int)tcpClient->write(outBlock);
            outBlock.resize(0);
        }
    }
    else
    {
        picNametime++;
        TotalBytes = 0;
        byteWritten = 0;
        QString jpgname = QString("send %1,eclpsed %2s")
                .arg(picNametime)
                .arg(time_total);
        statusBarText(jpgname);
//        emit removelistonesig();
        transfered();

//        transferjpg();
        if(FLAG_USED == flagbuf0)
            flagbuf0 = FLAG_NOUSE;
        else if(FLAG_USED == flagbuf1)
            flagbuf1 = FLAG_NOUSE;
    }

}

void MainWindow::startTransfer()
{
#ifdef DEBUGINFO
    qDebug() << "start transfer...";
#endif
    statusBarText(str_china("连接成功"));
    PushBtnBegin();
    ui->pushButton_connect->setEnabled(false);
    socketVarSets();
}

QString MainWindow::getIP()
{
    QString m_curipaddr = QString("%1.%2.%3.%4")
            .arg(ui->spinBox_ipadr1->text().toInt())
            .arg(ui->spinBox_ipadr2->text().toInt())
            .arg(ui->spinBox_ipadr3->text().toInt())
            .arg(ui->spinBox_ipadr4->text().toInt());
#ifdef DEBUGINFO
    qDebug() << "ip addr:" << m_curipaddr;
#endif
    return m_curipaddr;
}

quint32 MainWindow::getPort()
{
    return QString(DEFAULT_PORT).toInt();
}

void MainWindow::stopConnect()
{
    tcpClient->disconnectFromHost();
    statusBarText(str_china("暂停"));
}

void MainWindow::startConnect()
{
    m_connedipaddr = getIP();
    tcpClient->connectToHost(m_connedipaddr, getPort());
    statusBarText(str_china("连接中..."));
}
void MainWindow::statusBarText(QString text)
{
    ui->statusBar->showMessage(text);
}

void MainWindow::socketVarSets()
{
    picNametime = 0;
    TotalBytes = 0;
    byteWritten = 0;
    bytesToWrite = 0;

    m_transfered = STAT_TRANSFERING;

}
void MainWindow::transfered()
{
    m_transfered = STAT_TRANSFERED;
}

void MainWindow::transferring()
{
    m_transfered = STAT_TRANSFERING;
}

int MainWindow::gettransfered()
{
    return m_transfered;
}

void MainWindow::removelist()
{
#ifdef DEBUGINFO
    qDebug() << "delete one jpg";
    qDebug() << "imglist.count:" << imglist.count();
#endif
    if(imglist.count())
        imglist.removeAt(0);
}

void MainWindow::BufferSets()
{
//    for(int i=0;i < bufferlst.count();i++)
//        bufferlst.at(i).reset();
}

void MainWindow::SignalSets()
{
    connect(this,SIGNAL(removelistonesig()),this,SLOT(removelist()));
    connect(this,SIGNAL(transfersig()),this,SLOT(transferjpg()));
}

void MainWindow::SocketSets()
{
    socketVarSets();
    tcpClient  =new QTcpSocket;

    connect(tcpClient,SIGNAL(connected()),this,
            SLOT(startTransfer()));
    connect(tcpClient,SIGNAL(bytesWritten(qint64)),this,
            SLOT(updateClientProgress(qint64)));
    connect(tcpClient,SIGNAL(error(QAbstractSocket::SocketError)),this,
            SLOT(displayErr(QAbstractSocket::SocketError)));
}


void MainWindow::ButtonSets()
{
    connect(ui->pushButton_start,SIGNAL(clicked()),
            this,SLOT(startActive()));
    connect(ui->pushButton_pause,SIGNAL(clicked()),
            this,SLOT(pauseActive()));
    connect(ui->pushButton_quit,SIGNAL(clicked()),
            this,SLOT(quitActive()));
    connect(ui->pushButton_connect,SIGNAL(clicked()),
            this,SLOT(startConnect()));

    PushBtnAllFalse();
}

//定时器集
void MainWindow::TimerSets()
{
    for( int i=0;i<TIMERNUMS;i++)
    {
        QTimer *tmptimer = new QTimer;
        timerlist.push_back(tmptimer);
    }

    QList<QTimer*>::iterator i;
    for (i = timerlist.begin(); i != timerlist.end(); ++i) {
        //      *i = (*i).toLower(); // 使用 * 运算符获取遍历器所指的元素
        //       qDebug()<<*i;
        connect(*i,SIGNAL(timeout()),this,SLOT(grabScreenSignal()),Qt::DirectConnection);

    }


    for( int i=0;i<SAVETIMERNUMS;i++)
    {
        QTimer *tmptimer = new QTimer;
        savetimerlist.push_back(tmptimer);
    }

    QList<QTimer*>::iterator j;
    for (j = savetimerlist.begin(); j != savetimerlist.end(); ++j) {
        //      *i = (*i).toLower(); // 使用 * 运算符获取遍历器所指的元素
        //       qDebug()<<*i;
        connect(*j,SIGNAL(timeout()),this,SLOT(savetimertobuf()),Qt::DirectConnection);

    }
    time_total = 0;



    flagbuf0 = FLAG_NOUSE;
    flagbuf1 = FLAG_NOUSE;

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::receiveMsgBoxSignal()
{
    //    qDebug() << "receiveMsgBoxSignal";
    qDebug() << "===============================================";
    //    imglist.push_back(grabframeGeometry());
    //    QImage img = grabframeGeometry();
    //    qDebug() << "grab list size:"<<imglist.count();
}

void MainWindow::savetimertobuf()
{
//    if(imglist.count() < 10)
//        return;
    if(!imglist.count())
        return;

    if(FLAG_NOUSE == flagbuf0)
    {
        buffer.reset();
        outBlock.resize(0);

        qDebug() << "imlist count:" << imglist.count();
        imglist.at(0).save(&buffer,STREAM_PIC_FORT);
        imglist.removeFirst();
        qDebug() << "remove imlist count:" << imglist.count();
        flagbuf0 = FLAG_USED;
    }
    else if(FLAG_NOUSE == flagbuf1)
    {
        buffer1.reset();
        outBlock.resize(0);

        qDebug() << "imlist count:" << imglist.count();
        imglist.at(0).save(&buffer1,STREAM_PIC_FORT);
        imglist.removeFirst();
        qDebug() << "remove imlist count:" << imglist.count();
        flagbuf1 = FLAG_USED;
    }

}

void MainWindow::grabScreenSignal()
{
    QTime time;
    time.start(); //开始计时，以ms为单位

    QImage img;
#ifndef DESKSCREENTS
    /*
grab list size: 2867
"time:30.8666 png/s"
now elaspe: 92.8837 s
    */
    imglist.push_back(covertPixTo1024768(grabframeGeometry()));

//    img = covertPixTo1024768(grabframeGeometry());
#else
    /*
grab list size: 2433
"time:20.5653 png/s"
now elaspe: 118.306 s
*/
    imglist.push_back(covertPixTo1024768(grabDeskScreen()));
//    img = covertPixTo1024768(grabDeskScreen());
#endif
    //    qDebug() << "grab list size:"<<imglist.count();


#if 0
    buffertmp;
    buffertmp.reset();

    qDebug() << "imlist count:" << imglist.count();
    imglist.at(0).save(&buffertmp,STREAM_PIC_FORT);
    imglist.removeFirst();
    qDebug() << "remove imlist count:" << imglist.count();
//    bufferlst.push_back(buffertmp);

#endif

    int time_Diff = time.elapsed(); //返回从上次start()或restart()开始以来的时间差，单位ms

    //以下方法是将ms转为s
    float f = time_Diff / 1000.0;
    time_total += f;

    //    float pngrate = imglist.count() / time_total ;
    //    QString tr_timeDiff = QString("time:%1 png/s").arg(pngrate); //float->QString
    //    qDebug() << tr_timeDiff;
    //    qDebug() << "now elaspe:" <<time_total <<"s";

//    transferjpg();
    emit transfersig();
}

//截屏，界面尺寸
QImage MainWindow::grabframeGeometry()
{
    return QPixmap::grabWindow(QApplication::desktop()->winId(),
                               pos().x(),
                               pos().y(),
                               frameGeometry().width(),
                               frameGeometry().height()).toImage();

}

//截屏，桌面尺寸
QImage MainWindow::grabDeskScreen()
{
    return  QPixmap::grabWindow(QApplication::desktop()->winId(),
                                0,
                                0,
                                QApplication::desktop()->width(),
                                QApplication::desktop()->height()).toImage();
}

//转化图片分辨率
QImage MainWindow::covertPixTo1024768(QImage  img)
{
    QImage image = img;
#ifdef ZIPCONVERTPIX
    int imgHt = image.width();
    int imgWt = image.height();

#ifdef DEBUGINFO
    qDebug() <<"bf img ht:" << imgHt;
    qDebug() <<"bf img wt:" << imgWt;
#endif
    if(imgHt >=VGASTDHEIGHT && imgWt >= VGASTDWIDTH)
        image =  img.scaled(VGASTDHEIGHT, VGASTDWIDTH);

#ifdef DEBUGINFO
    qDebug() <<"af img ht:" << image.height();
    qDebug() <<"af img wt:" << image.width();
#endif

#endif
    return image;
}

//连接真，开始假，暂停假
void MainWindow::PushBtnRestart()
{
    ui->pushButton_connect->setEnabled(true);
    ui->pushButton_start->setEnabled(false);
    ui->pushButton_pause->setEnabled(false);
}
//开始真，暂停假
void MainWindow::PushBtnBegin()
{
    ui->pushButton_start->setEnabled(true);
    ui->pushButton_pause->setEnabled(false);
}
//开始假，暂停真
void MainWindow::PushBtnPause()
{
    ui->pushButton_start->setEnabled(false);
    ui->pushButton_pause->setEnabled(true);
}
//开始假，暂停假
void MainWindow::PushBtnAllFalse()
{
    ui->pushButton_start->setEnabled(false);
    ui->pushButton_pause->setEnabled(false);
}

void MainWindow::startActive()
{
    PushBtnPause();
    QList<QTimer*>::iterator i;
    for (i = timerlist.begin(); i != timerlist.end(); ++i) {
        (*i)->start(100);

    }

    QList<QTimer*>::iterator j;
    for (j = savetimerlist.begin(); j != savetimerlist.end(); ++j) {
        (*j)->start(10);
    }

}

void MainWindow::pauseActive()
{
    PushBtnBegin();

    QList<QTimer*>::iterator i;
    for (i = timerlist.begin(); i != timerlist.end(); ++i) {
        (*i)->stop();

    }
    socketVarSets();
    imglist.clear();

}

void MainWindow::quitActive()
{
    pauseActive();
    socketVarSets();
    timerlist.clear();
    imglist.clear();
    close();
}
