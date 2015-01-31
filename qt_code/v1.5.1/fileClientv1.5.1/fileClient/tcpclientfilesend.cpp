#include "tcpclientfilesend.h"
#include "ui_tcpclientfilesend.h"
#include <QTextCodec>
#include <QPixmap>
#include <QDesktopWidget>
#include <windows.h>
#include <QIODevice>

#define DEBUG    /* 调试信息 */
#define TIMETEST /* 耗时测试 */

const char version_filetransClient[]="v1.0";


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

    ui->startButton->setEnabled(false);
    ui->lineEditHost->setText(ReadIpAddr());
    //    ui->lineEditPort->setText(DEFAULT_PORT);

    QStringList screensize;
    screensize.clear();
    screensize <<"gemetory size"  //0
              <<"800 X 600"
             <<"1024 X 768"
            <<"1366 X 768"
           <<"fullscreen";
    ui->comboBox_grabScreenSize->addItems(screensize);
    ui->comboBox_grabScreenSize->setCurrentIndex(3);//"800 X 600"

    dirname = QString("images");
    QDir dir(QDir::currentPath());
    if(!dir.exists(dirname))
    {
        dir.mkdir(dirname);
    }

    connect(ui->startButton,SIGNAL(clicked()),this,SLOT(start()));
    connect(ui->quitButton,SIGNAL(clicked()),this,SLOT(ShutDownAll()));
    connect(ui->openButton,SIGNAL(clicked()),this,SLOT(openFile()));
    connect(ui->pushbtn_about,SIGNAL(clicked()),this,SLOT(aboutVer()));

    connect(&tcpClient,SIGNAL(connected()),this,
            SLOT(startTransfer()));
    connect(&tcpClient,SIGNAL(bytesWritten(qint64)),this,
            SLOT(updateClientProgress(qint64)));
    connect(&tcpClient,SIGNAL(error(QAbstractSocket::SocketError)),this,
            SLOT(displayErr(QAbstractSocket::SocketError)));

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

void tcpClientFileSend::openFile()
{
    ui->startButton->setEnabled(true);
    ui->clientStatusLabel->setText(str_china("请点击开始远程传输"));
}


void tcpClientFileSend::start()
{
#ifdef SHOWCURSOR
    QApplication::setOverrideCursor(Qt::WaitCursor);
#endif
    byteWritten = 0;
    ui->clientStatusLabel->setText(str_china("连接中..."));
    tcpClient.connectToHost(ui->lineEditHost->text(),
                            QString(DEFAULT_PORT).toInt());
    tcpClient.setSocketOption(QAbstractSocket::LowDelayOption, 1);//优化为最低延迟，后面的1代码启用该优化。

    //waitForConnected()等待连接知道超过最大等待时间。如果连接建立函数返回true；否则返回false。
    //当返回false时可以调用error来确定无法连接的原因
    if(!tcpClient.waitForConnected(3000))
    {
        qDebug() <<"Error: "<<tcpClient.errorString();
        displayErr(QAbstractSocket::SocketTimeoutError);
    }
}

/* 一旦连接建立成功，QTcpSocket类将发出connected消息，继而调用
startTransfer()槽函数。
 */
void tcpClientFileSend::startTransfer()
{
#ifdef TIMETEST /* 耗时测试 */
    QTime time;
    time.start(); //开始计时，以ms为单位

#endif /* 耗时测试 */

    //    ui->clientStatusLabel->setText(str_china("连接成功"));

    timer->start(10);
    SaveIpAddr(ui->lineEditHost->text());

    if(namelst.count() > 2)
    {
        emit emitImgZeroSignal();
        return;
    }


    fileImage = grabframeGeometry();

    //    fileImage =fileImage.convertToFormat(QImage::Format_Indexed8,Qt::AutoColor);

    QString fileName = QString("%1/%2.jpg").arg(dirname)
            .arg(jpgnameNo++);

#ifdef DEBUG
    qDebug() << "filename:" << fileName;
#endif

    namelst.append(fileName);

    fileImage.save(fileName,STREAM_PIC_FORT);

#ifdef TIMETEST /* 耗时测试 */

    int time_Diff = time.elapsed(); //返回从上次start()或restart()开始以来的时间差，单位ms
    //以下方法是将ms转为s
    float f = time_Diff / 1000.0;
    time_total += f;

#ifdef DEBUG
    qDebug() << "save  elaspe:" <<time_Diff <<"ms";
    qDebug() << "current time:" <<time_total <<"s";
#endif

#endif /* 耗时测试 */


    emit emitImgZeroSignal();

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

    QDir dir(QDir::currentPath());
    if(!dir.exists(dirname))
    {
        dir.mkdir(dirname);
    }

    QFile file(readFname);
    if(!file.open(QIODevice::ReadOnly)) {
#ifdef DEBUG
        qDebug()<<"Can't open the file!"<<readFname;
#endif
        return;
    }

    /*---------------------------------------
发送文件数据格式 ：

1.总长度 (8bytes) -- 总字节(文件大小 + 8字节 + 文件名)
2.文件名长度(qint64() 8Bytes)
3.文件名
---------------------------------------*/


    outBlockFile = file.readAll();
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

    bytesToWrite = TotalBytes - tcpClient.write(outBlock);//将名称发出后，剩余图片大小
    ui->clientStatusLabel->setText(str_china("已连接"));
#ifdef DEBUG
    qDebug() << "TotalBytes:" << TotalBytes;
#endif

    file.close();
    dir.remove(readFname);//删除文件


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

        bytesToWrite -= (int)tcpClient.write(outBlockFile);


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
    }

    ui->clientStatusLabel->setText(str_china("传输中..."));

}

void tcpClientFileSend::displayErr(QAbstractSocket::SocketError socketError)
{
    if(socketError == QTcpSocket::RemoteHostClosedError)
        return;
    QMessageBox::information(this,str_china("网络"),
                             str_china("产生如下错误： %1")
                             .arg(tcpClient.errorString()));

    tcpClient.close();

    ui->clientStatusLabel->setText(str_china("客户端就绪"));
    ui->startButton->setEnabled(true);
    deleteImgs();
    namelst.clear();
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
    QDir dir(QDir::currentPath());
    if(!dir.exists(dirname))
    {
        dir.mkdir(dirname);
    }


    for(int i=0;i <namelst.count();i++)
    {
#ifdef DEBUG
        qDebug() <<"remove filename:"<<namelst.at(i);
#endif
        dir.remove(namelst.at(i));
    }
//    dir.remove(dirname);
}

void tcpClientFileSend::ShutDownAll()
{
    timer->stop();

    byteWritten = 0;
    tcpClient.disconnectFromHost();
    tcpClient.waitForDisconnected(5000);
    tcpClient.close();

    deleteImgs();


    ui->clientStatusLabel->setText(str_china("客户端就绪"));
    ui->startButton->setEnabled(true);
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
