#include "tcpclientfilesend.h"
#include "ui_tcpclientfilesend.h"
#include <QTextCodec>
#include <QPixmap>
#include <QDesktopWidget>
#include <windows.h>
#include <QIODevice>

//#define DEBUG    /* 调试信息 */
//#define TIMETEST /* 耗时测试 */

const char version_filetransClient[]="v1.3";


#if 1
//编码汉字
//#define str_china(A)     QString::fromLocal8Bit(#A)
#define str_china(A)     QString::fromLocal8Bit(A)
//#define str_china(A)     QString::fromUtf8(#A)
#else
#define str_china(A)     codec->toUnicode(#A)
#endif

#if 1
#define DEFAULT_HOSTADDR "192.168.1.104"
//#define DEFAULT_HOSTADDR "169.254.194.157"
#else
#define DEFAULT_HOSTADDR "127.0.0.1"
#endif
#define DEFAULT_PORT   "16689"

#define OVERIMAGENUMS (500)



tcpClientFileSend::tcpClientFileSend(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::tcpClientFileSend)
{

    ui->setupUi(this);

    picNametime = 1;
    TotalBytes = 0;
    byteWritten = 0;

    jpgnameNo = 1; //图片名称计数
    time_total = 0.0;//耗时时间
    namelst.clear();//保存的文件名列表
    sizelst.clear();//保存的文件大小列表
    sendDoneFlag = SEND_DONE;//发送结束标志
    emitSigNums = 0;//发送信号的次数
    curstate = STATE_PAUSE;//当前状态，开启或暂停
    p_tcpClient = NULL;//tcp socket
    imgVecArray.clear();
    imgLstArray.clear();

    ui->openButton->setEnabled(true);
    ui->startButton->setEnabled(false);
    ui->pauseButton->setEnabled(false);
    ui->lineEditHost->setText(ReadIpAddr());
    //    ui->lineEditPort->setText(DEFAULT_PORT);
    ui->checkBox_speed->setChecked(true);

    QStringList screensize;
    screensize.clear();
    screensize <<"gemetory size"  //0
              <<"800 X 600"
             <<"1024 X 768"
            <<"1366 X 768"
           <<"fullscreen";
    ui->comboBox_grabScreenSize->addItems(screensize);
    ui->comboBox_grabScreenSize->setCurrentIndex(3);//"800 X 600"

//    dirname = QString("images");
//    QDir dir(QDir::currentPath());
//    if(!dir.exists(dirname))
//    {
//        dir.mkdir(dirname);
//    }

    connect(ui->startButton,SIGNAL(clicked()),this,SLOT(start()));
    connect(ui->pauseButton,SIGNAL(clicked()),this,SLOT(pause()));
    connect(ui->openButton,SIGNAL(clicked()),this,SLOT(openFile()));
    connect(ui->pushbtn_about,SIGNAL(clicked()),this,SLOT(aboutVer()));


    timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,
            SLOT(startTransfer()));

    connect(this,SIGNAL(emitImgZeroSignal()),this,
            SLOT(parseImage()));

    connect(qApp,SIGNAL(lastWindowClosed()),this,SLOT(ShutDownAll()));

}

tcpClientFileSend::~tcpClientFileSend()
{
    delete ui;
}
//连接
void tcpClientFileSend::openFile()
{
    ui->startButton->setEnabled(true);
    ui->clientStatusLabel->setText(str_china("请点击开始远程传输"));
    //创建tcpsocket
    p_tcpClient = new QTcpSocket;
    if(!p_tcpClient)
        return;

    connect(p_tcpClient,SIGNAL(connected()),this,
            SLOT(startTransfer()));
    connect(p_tcpClient,SIGNAL(bytesWritten(qint64)),this,
            SLOT(updateClientProgress(qint64)));
    connect(p_tcpClient,SIGNAL(error(QAbstractSocket::SocketError)),this,
            SLOT(displayErr(QAbstractSocket::SocketError)));

}


void tcpClientFileSend::start()
{
#ifdef SHOWCURSOR
    QApplication::setOverrideCursor(Qt::WaitCursor);
#endif
    byteWritten = 0;
    curstate = STATE_START;
    ui->startButton->setEnabled(false);
    ui->pauseButton->setEnabled(true);


    ui->clientStatusLabel->setText(str_china("连接中..."));
    p_tcpClient->connectToHost(ui->lineEditHost->text(),
                               QString(DEFAULT_PORT).toInt());
    p_tcpClient->setSocketOption(QAbstractSocket::LowDelayOption, 1);//优化为最低延迟，后面的1代码启用该优化。


    //waitForConnected()等待连接知道超过最大等待时间。如果连接建立函数返回true；否则返回false。
    //当返回false时可以调用error来确定无法连接的原因
    if(!p_tcpClient->waitForConnected(3000))
    {
        qDebug() <<"here:" << p_tcpClient;
        if(NULL != p_tcpClient)
        {
            qDebug() <<"Error: "<<p_tcpClient->errorString();
            p_tcpClient->deleteLater();
            p_tcpClient = NULL;

            QMessageBox::information(this,str_china("网络"),
                                 str_china("产生如下错误：连接失败"));
        }
        timer->stop();

        curstate = STATE_PAUSE;
        ui->openButton->setEnabled(true);
        ui->startButton->setEnabled(false);
        ui->pauseButton->setEnabled(false);
        ui->clientStatusLabel->setText(str_china("连接失败，请输入正确IP连接"));
    }else{
        if(!timer->isActive())
        {
            timer->start(100);
            ui->openButton->setEnabled(false);
            ui->startButton->setEnabled(false);
            ui->pauseButton->setEnabled(true);
            curstate = STATE_START;
        }
        SaveIpAddr(ui->lineEditHost->text());
        ui->clientStatusLabel->setText(str_china("连接成功"));
    }

}
//暂停
void tcpClientFileSend::pause()
{
    curstate = STATE_PAUSE;
    ui->startButton->setEnabled(true);
    ui->pauseButton->setEnabled(false);
    ui->clientStatusLabel->setText(str_china("暂停中..."));
}

/* 一旦连接建立成功，QTcpSocket类将发出connected消息，继而调用
startTransfer()槽函数。
 */
void tcpClientFileSend::startTransfer()
{
    if(STATE_PAUSE == curstate)
    {
        timer->stop();
    }

#ifdef TIMETEST /* 耗时测试 */
    QTime time;
    time.start(); //开始计时，以ms为单位

#endif /* 耗时测试 */

    //    ui->clientStatusLabel->setText(str_china("连接成功"));

    if(namelst.count() > 2)
    {
        //        if(socketError == QTcpSocket::RemoteHostClosedError)
        //        {

        //        }
        return;
    }

    emitSigNums = 0;//发送信号归零

    fileImage = grabframeGeometry();
    if(ui->checkBox_speed->isChecked())
    {
        Sleep(200);
    }else
    {
        Sleep(10);
    }

    //    fileImage =fileImage.convertToFormat(QImage::Format_Indexed8,Qt::AutoColor);

    QString fileName = QString("%1/%2.jpg").arg(dirname)
            .arg(jpgnameNo++);

#ifdef DEBUG
    qDebug() << "filename:" << fileName;
#endif

    namelst.append(fileName);

//    fileImage.save(fileName,STREAM_PIC_FORT);
    QByteArray bytearry;
    QBuffer buffer;
    buffer.setBuffer(&bytearry);
    fileImage.save(&buffer,STREAM_PIC_FORT);
    buffer.data();
    imgVecArray.append(bytearry);

#ifdef TIMETEST /* 耗时测试 */

    int time_Diff = time.elapsed(); //返回从上次start()或restart()开始以来的时间差，单位ms
    //以下方法是将ms转为s
    float f = time_Diff / 1000.0;
    time_total += f;

    qDebug() << "save  elaspe:" <<time_Diff <<"ms";
    qDebug() << "current time:" <<time_total <<"s";

#endif /* 耗时测试 */


    //    emit emitImgZeroSignal();
    parseImage();

    return;
}

void tcpClientFileSend::parseImage()
{

    if(namelst.count() == 0)
    {

#ifdef DEBUG
        qDebug() << "namelst count is 0!!";
#endif
        return;
    }

#ifdef DEBUG
    qDebug() << "namelst count:" << namelst.count();
#endif
    if(SEND_ING == sendDoneFlag)
        return;

    sendDoneFlag = SEND_ING;

    outBlock.resize(0);
    QString readFname = namelst.at(0);
#ifdef DEBUG
    qDebug() << "read file name:" << readFname;
#endif
    namelst.removeAt(0);
#ifdef DEBUG
    qDebug() << "after delete namelst count:" << namelst.count();
#endif

//    QDir dir(QDir::currentPath());
//    if(!dir.exists(dirname))
//    {
//        dir.mkdir(dirname);
//    }

//    QFile file(readFname);
//    if(!file.open(QIODevice::ReadOnly)) {
//#ifdef DEBUG
//        qDebug()<<"Can't open the file!"<<readFname;
//#endif
//        return;
//    }

    /*---------------------------------------
发送文件数据格式 ：

1.总长度 (8bytes) -- 总字节(文件大小 + 8字节 + 文件名)
2.文件名长度(qint64() 8Bytes)
3.文件名
---------------------------------------*/


//    outBlockFile = file.readAll();
    outBlockFile = imgVecArray.at(0);
    imgVecArray.remove(0);
#ifdef DEBUG
    qDebug() <<"read file size:" << outBlockFile.size() ;
#endif

    TotalBytes = outBlockFile.size();



    outBlock.resize(0);
    QDataStream sendOut(&outBlock, QIODevice::WriteOnly);
    sendOut.resetStatus();
    sendOut.setVersion(QDataStream::Qt_4_0);

    //发送文件名称
    sendOut <<qint64(0) <<qint64(0) <<readFname;
    //TotalBytes为总数据长度，即（数据量长度+文件名长度+文件名）
    TotalBytes += outBlock.size(); //加上图片名称长度
    sendOut.device()->seek(0);

    //总字节(文件大小 + 8字节 + 文件名) ，
    sendOut << TotalBytes << qint64((outBlock.size() - sizeof(qint64)*2));

    bytesToWrite = TotalBytes - p_tcpClient->write(outBlock);//将名称发出后，剩余图片大小
    ui->clientStatusLabel->setText(str_china("已连接"));
#ifdef DEBUG
    qDebug() << "TotalBytes:" << TotalBytes;
#endif

//    file.close();
//    dir.remove(readFname);//删除文件


    return;

}

void tcpClientFileSend::updateClientProgress(qint64 numBytes)
{

#ifdef DEBUG
    qDebug() << "numBytes:--------->>"<<numBytes;
#endif
    byteWritten += (int)numBytes;
    if(bytesToWrite > 0)
    {
#ifdef DEBUG
        qDebug() <<"-->:outBlockFile size:" << outBlockFile.size();
#endif

        bytesToWrite -= (int)p_tcpClient->write(outBlockFile);


#ifdef DEBUG
        qDebug() <<"-->:bytesToWrite size:" << bytesToWrite;
#endif
    }
    else
    {
#ifdef DEBUG
        qDebug() << "-->: send image done!!";
#endif
        picNametime++;
        TotalBytes = 0;
        byteWritten = 0;
        sendDoneFlag = SEND_DONE;
        parseImage();
        if(STATE_PAUSE == curstate)
        {
            timer->stop();
            ui->clientStatusLabel->setText(str_china("暂停中..."));
        }else{
            ui->clientStatusLabel->setText(str_china("传输中..."));
        }
    }



}

void tcpClientFileSend::displayErr(QAbstractSocket::SocketError socketError)
{
    qDebug() << "display err";
    if(NULL == p_tcpClient)
    {
        QMessageBox::information(this,str_china("网络"),
                             str_china("产生如下错误：连接失败"));
    }else{
        QMessageBox::information(this,str_china("网络"),
                             str_china("产生如下错误： %1")
                             .arg(p_tcpClient->errorString()));
    }

    timer->stop();
    if(NULL != p_tcpClient)
    {
        p_tcpClient->abort();
        p_tcpClient->waitForDisconnected();
        p_tcpClient->disconnectFromHost();
        p_tcpClient->close();
        p_tcpClient->deleteLater();
        p_tcpClient = NULL;
    }
    deleteImgs();
    namelst.clear();
    ui->clientStatusLabel->setText(str_china("客户端就绪"));
    ui->startButton->setEnabled(false);
    ui->openButton->setEnabled(true);


    picNametime = 1;
    TotalBytes = 0;
    byteWritten = 0;

    jpgnameNo = 1; //图片名称计数
    time_total = 0.0;//耗时时间
    namelst.clear();//保存的文件名列表
    sizelst.clear();//保存的文件大小列表
    sendDoneFlag = SEND_DONE;//发送结束标志
    emitSigNums = 0;//发送信号的次数
    curstate = STATE_PAUSE;
    ui->openButton->setEnabled(true);
    ui->startButton->setEnabled(false);
    ui->pauseButton->setEnabled(false);


#ifdef SHOWCURSOR
    QApplication::restoreOverrideCursor();
#endif
}

//桌面尺寸
QImage tcpClientFileSend::grabDeskScreen()
{
    return  QPixmap::grabWindow(QApplication::desktop()->winId(),
                                0,
                                0,
                                QApplication::desktop()->width(),
                                QApplication::desktop()->height()).toImage();
}

//界面尺寸
QImage tcpClientFileSend::grabframeGeometry()
{
    if(ui->comboBox_grabScreenSize->currentText().contains("800"))
    {
        return QPixmap::grabWindow(QApplication::desktop()->winId(),
                                   pos().x(),
                                   pos().y(),
                                   800,
                                   600).toImage();
    }
    else if(ui->comboBox_grabScreenSize->currentText().contains("1024"))
    {
        return QPixmap::grabWindow(QApplication::desktop()->winId(),
                                   pos().x(),
                                   pos().y(),
                                   1024,
                                   768).toImage();
    }
    else if(ui->comboBox_grabScreenSize->currentText().contains("fullscreen"))
    {
        return grabDeskScreen();
    }
    else if(ui->comboBox_grabScreenSize->currentText().contains("1366"))
    {
        return QPixmap::grabWindow(QApplication::desktop()->winId(),
                                   0,
                                   0,
                                   1366,
                                   768).toImage();
    }
    else
    {
        return QPixmap::grabWindow(QApplication::desktop()->winId(),
                                   pos().x(),
                                   pos().y(),
                                   frameGeometry().width(),
                                   frameGeometry().height()).toImage();
    }

}

void tcpClientFileSend::deleteImgs()
{
//    QDir dir(QDir::currentPath());
//    if(!dir.exists(dirname))
//    {
//        dir.mkdir(dirname);
//    }


//    for(int i=0;i <namelst.count();i++)
//    {
//#ifdef DEBUG
//        qDebug() <<"remove filename:"<<namelst.at(i);
//#endif
//        dir.remove(namelst.at(i));
//    }
//    //    dir.remove(dirname);

    imgVecArray.clear();
    imgLstArray.clear();
}

void tcpClientFileSend::ShutDownAll()
{
    timer->stop();

    if(NULL != p_tcpClient)
    {
        p_tcpClient->abort();
        p_tcpClient->waitForDisconnected();
        p_tcpClient->disconnectFromHost();
        p_tcpClient->close();
        p_tcpClient->deleteLater();
        p_tcpClient = NULL;
    }

    deleteImgs();
    namelst.clear();
    ui->clientStatusLabel->setText(str_china("客户端就绪"));

    picNametime = 1;
    TotalBytes = 0;
    byteWritten = 0;

    jpgnameNo = 1; //图片名称计数
    time_total = 0.0;//耗时时间
    namelst.clear();//保存的文件名列表
    sizelst.clear();//保存的文件大小列表
    sendDoneFlag = SEND_DONE;//发送结束标志
    emitSigNums = 0;//发送信号的次数
    curstate = STATE_PAUSE;
    ui->openButton->setEnabled(true);
    ui->startButton->setEnabled(false);
    ui->pauseButton->setEnabled(false);
    close();
}

QString tcpClientFileSend::ReadIpAddr()
{
    QFile file("./serverip.conf");

    if(file.exists())
    {
        QByteArray dataFromFile;
        QString ipaddr;
        file.open(QIODevice::ReadOnly);
        dataFromFile=file.readAll();
        file.close();

#ifdef DEBUG
        qDebug() << "read ip:" << dataFromFile;
#endif
        ipaddr = QString(dataFromFile);
        if(!ipaddr.contains("192.168.1"))
        {
            return QString(DEFAULT_HOSTADDR);
        }

#ifdef DEBUG
        qDebug() << "ipaddr :" << ipaddr;
#endif
        return ipaddr;
    }
    else
    {
        file.open(QIODevice::WriteOnly);
        file.write(DEFAULT_HOSTADDR);
        file.close();
#ifdef DEBUG
        qDebug() << "read ip:" << DEFAULT_HOSTADDR;
#endif
        return QString(DEFAULT_HOSTADDR);
    }
}

void tcpClientFileSend::SaveIpAddr(QString ipaddr)
{
    QFile file("./serverip.conf");
    static uint8_t saveflag = 0;

    if(file.exists() && 0==saveflag)
    {
        file.open(QIODevice::WriteOnly);
        file.write(ipaddr.toLocal8Bit());
        file.close();
        saveflag = 1;
    }
}

void tcpClientFileSend::PrintInfoToFile(QString str)
{
    QFile file("./dbginfo.txt");

    if(file.exists())
    {
        file.open(QIODevice::Append);
        file.write(str.toLocal8Bit());
        file.close();
    }
    else
    {
        file.open(QIODevice::WriteOnly);
        file.write(str.toLocal8Bit());
        file.close();
    }
}

QString tcpClientFileSend::GetVersion(void)
{
    return str_china("桌面传输系统")
            +"\n"
            +str_china("by小魏莱")
            +"\n"
            +version_filetransClient;
}

void tcpClientFileSend::aboutVer()
{
    QMessageBox::information(NULL, str_china("版本"), GetVersion(),NULL,NULL);
    return;
}
