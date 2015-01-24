#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDesktopWidget>
#include <QDesktopServices>
#include <QTime>
#include <QRegExp>

/*定时器个数*/
#define TIMERNUMS 1

//分辨率比此大，则转换为此分辨率
#define VGASTDHEIGHT 1024
#define VGASTDWIDTH  768

//打印调试信息
//#define DEBUGINFO
//转换分辨率,压缩
//#define ZIPCONVERTPIX
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

#if 0
    getImgThd = new DoSendThread(NULL);
    getImgThd->start();
    connect(getImgThd,SIGNAL(emitMsgBoxSignal()),this,SLOT(receiveMsgBoxSignal()),Qt::DirectConnection);
#endif

    TimerSets();
    ButtonSets();
    SocketSets();
    SignalSets();

    totalbyteslist.clear();
    bytesToWritelist.clear();
    outBlocklst.clear();
    outBlockheadlst.clear();
    namecounter = 0;
    time_totaltfs = 0.0;
    outBlockTotal.resize(0);
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
    img = covertPixTo1024768(grabframeGeometry());
//    if(0 == imglist.count()){
//        imglist.push_back(img);
//    }else{
//        QImage tmpimg = imglist.at(0);
//        if(img.byteCount() == tmpimg.byteCount())
//            return;
//    }
//    QString imgname =QString("%1.jpg").arg(namecounter++);
//    img.save(imgname);
    imglist.push_back(img);

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



//图片数据
    QByteArray outBlockjpgcout;
    outBlockjpgcout.resize(0);
    buffer.setBuffer(&outBlockjpgcout);
    buffer.open(QIODevice::WriteOnly);
    quint64 totalbytes = imglist.at(0).byteCount();
    totalbyteslist.push_back(totalbytes);
    qDebug() << "jpg size:"<<totalbytes;
    imglist.at(0).save(&buffer,STREAM_PIC_FORT);
    imglist.removeAt(0);
    buffer.close();
    qDebug() <<"buffer size:" <<buffer.data().size();

    outBlocklst.push_back(outBlockjpgcout);
    qDebug() <<"outBlocklst size:" <<outBlockjpgcout.size();



    ///////////////////////////////////////////

    /* 发送文件数据格式 ：
    总长度 (8bytes) | 文件名长度(qint64() 8Bytes) |由文件名长度指定
    */
//数据头

    QByteArray outBlockhead;
    outBlockhead.resize(0);
//        buffer.open(QIODevice::ReadOnly);
    QDataStream sendOut(&outBlockhead, QIODevice::WriteOnly);
    sendOut.resetStatus();
    sendOut.setVersion(QDataStream::Qt_4_0);

    QString currentFile = QString("blog%1.%2").arg(picNametime).arg(SUFIXNAME);
    qDebug() << "filename:"<<currentFile;
    //发送文件名称
    sendOut <<qint64(0) <<qint64(0) <<currentFile;
    //TotalBytes为总数据长度，即（数据量长度+文件名长度+文件名）
    TotalBytes = 0;
    TotalBytes += outBlockhead.size(); //加上图片名称长度
    qDebug() << "outBlock size jpglen:" <<TotalBytes;

    sendOut.device()->seek(0);
    //图片大小
    qDebug() << "jpg size:" <<totalbyteslist.at(0);
    TotalBytes += totalbyteslist.at(0);
    //发送（数据量长度+文件名长度+文件名）
    sendOut << TotalBytes << qint64((outBlock.size() - sizeof(qint64)*2));


    totalbyteslist.removeAt(0);
    qDebug() << "send TotalBytes:" <<TotalBytes;
    qDebug() << "head size:" <<outBlockhead.size();
    qDebug() << "cout size:" <<outBlockjpgcout.size();



    outBlockTotal.append(outBlockhead);//全成一体进行传输
    outBlockTotal.append(outBlockjpgcout);

    totalbyteslist.push_back(TotalBytes);
    outBlockhead.resize(0);
    outBlockjpgcout.resize(0);


    tcpClient->write(outBlockTotal);
    ///////////////////////////////////////////


    int time_Diff = time.elapsed(); //返回从上次start()或restart()开始以来的时间差，单位ms
    //以下方法是将ms转为s
    float f = time_Diff / 1000.0;
    time_total += f;

    qDebug() << "save to buffer elaspe:" <<time_Diff <<"ms";

    picNametime++;



//    emit transfersig();
}

void MainWindow::transferjpg()
{
//    if(imglist.count()<3)
//        return;
    if(STAT_NOUSE == m_transfered)
    {
        qDebug() << "-------------->>>>>>>>>:";
        QTime time;
        time.start(); //开始计时，以ms为单位
        m_transfered = STAT_USED;

        TotalBytes = totalbyteslist.at(0);
        bytesToWrite = TotalBytes - tcpClient->write(outBlockheadlst.at(0));//将名称发出后，剩余图片大小

        bytesToWritelist.push_back(bytesToWrite);

        qDebug() << "last TotalBytes:" <<TotalBytes;
        qDebug() << "last bytesToWrite:" <<bytesToWrite;

        int time_Diff = time.elapsed(); //返回从上次start()或restart()开始以来的时间差，单位ms
        qDebug() << "transfer buffer elaspe:" <<time_Diff <<"ms";
    }else{
        qDebug() << "!!!!!!!!!m_transfered:" <<m_transfered;

    }



}

void MainWindow::updateClientProgress(qint64 numBytes)
{

    QTime time;
    time.start(); //开始计时，以ms为单位

    byteWritten += numBytes;
    qint64 tcpwrite = 0;
    qDebug() <<"update ---->>>>:" ;
    if(outBlockTotal.count() >0)
    {
        tcpwrite = tcpClient->write(outBlockTotal);
        qDebug() <<"tcpwrite :" << tcpwrite ;
        qDebug() <<"byteWritten :" << byteWritten;
    }

#if 0
    if(bytesToWrite > 0)
    {

#ifdef DEBUGINFO
        qDebug() <<"update ---->>>>:" ;
        qDebug() <<"Totalbytes  :" << TotalBytes;
#endif
        qDebug() <<"update ---->>>>:" ;
        qDebug() <<"Totalbytes  :" << TotalBytes;
        qDebug() <<"byteWritten :" << byteWritten;
        outBlock.resize(0);
        outBlock = outBlocklst.at(0);
//        buffer.open(QIODevice::ReadOnly);
//        outBlock = buffer.data();
        quint64 outBlocksize = outBlock.size();
        qDebug() <<"outBlocksize :" << outBlocksize;
#ifdef DEBUGINFO
        qDebug() <<"outBlocksize:" << outBlocksize;
        qDebug() <<"byteWritten :" << byteWritten;
        qDebug() <<"byteWritten+outBlocksize :" << byteWritten+outBlocksize;
#endif
        qint64 tcpwrite = 0;
        if(byteWritten + outBlocksize >= TotalBytes)
        {
            tcpwrite = tcpClient->write(outBlock,bytesToWrite);

            int time_Diff = time.elapsed(); //返回从上次start()或restart()开始以来的时间差，单位ms
            //以下方法是将ms转为s
            float f = time_Diff ;
            time_totaltfs += f;
            qDebug() << "complete transfer:" <<time_totaltfs <<"s";

        }
        else{
            tcpwrite = tcpClient->write(outBlock);
        }
        bytesToWrite -= tcpwrite;

//        buffer.pos();
//        buffer.reset();
//        buffer.close();
#ifdef DEBUGINFO
        qDebug() <<"tcpwrite    :" <<tcpwrite;
        qDebug() <<"towrite    :" << bytesToWrite;
#endif

    }
    else
    {
        picNametime++;
        TotalBytes = 0;
        byteWritten = 0;
        bytesToWrite = 0;
        QString jpgname = QString("send %1,eclpsed %2s")
                .arg(picNametime)
                .arg(time_total);
        statusBarText(jpgname);
        bytesToWritelist.removeAt(0);
        totalbyteslist.removeAt(0);
        outBlocklst.removeAt(0);
        outBlockheadlst.removeAt(0);
        m_transfered = STAT_NOUSE;
        transferjpg();
    }

#endif


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

    m_transfered = STAT_NOUSE;

}
void MainWindow::transfered()
{
    m_transfered = STAT_USED;
}

void MainWindow::transferno()
{
    m_transfered = STAT_NOUSE;
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
//    if(imglist.count())
//        imglist.removeAt(0);
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
            SLOT(startTransfer()),Qt::DirectConnection);
    connect(tcpClient,SIGNAL(bytesWritten(qint64)),this,
            SLOT(updateClientProgress(qint64)),Qt::DirectConnection);
    connect(tcpClient,SIGNAL(error(QAbstractSocket::SocketError)),this,
            SLOT(displayErr(QAbstractSocket::SocketError)),Qt::DirectConnection);
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

    time_total = 0;
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
    tcpClient->disconnectFromHost();
    tcpClient->waitForDisconnected();
    close();
}
