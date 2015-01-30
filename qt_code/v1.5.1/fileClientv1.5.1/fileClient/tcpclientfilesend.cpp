#include "tcpclientfilesend.h"
#include "ui_tcpclientfilesend.h"
#include <QTextCodec>
#include <QPixmap>
#include <QDesktopWidget>
#include <windows.h>
#include <QIODevice>

#define DEBUG

#ifdef DEBUG
#define DDbg qDebug
#else
#define DDbg
#endif

#define TIMETEST /* 耗时测试 */

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
    ui->lineEditPort->setText(DEFAULT_PORT);

    QStringList screensize;
    screensize.clear();
    screensize <<"gemetory size"  //0
              <<"800 X 600"
             <<"1024 X 768"
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

    connect(&tcpClient,SIGNAL(connected()),this,
            SLOT(startTransfer()));
    connect(&tcpClient,SIGNAL(bytesWritten(qint64)),this,
            SLOT(updateClientProgress(qint64)));
    connect(&tcpClient,SIGNAL(error(QAbstractSocket::SocketError)),this,
            SLOT(displayErr(QAbstractSocket::SocketError)));

    timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,
            SLOT(startTransfer()));
//    connect(timer,SIGNAL(timeout()),this,
//            SLOT(parseImage));

    connect(this,SIGNAL(emitImgZeroSignal()),this,
            SLOT(parseImage()));


}

tcpClientFileSend::~tcpClientFileSend()
{
    delete ui;
}

void tcpClientFileSend::openFile()
{
    ui->startButton->setEnabled(true);
}


void tcpClientFileSend::start()
{
#ifdef SHOWCURSOR
    QApplication::setOverrideCursor(Qt::WaitCursor);
#endif
    byteWritten = 0;
    ui->clientStatusLabel->setText(str_china("连接中..."));
    tcpClient.connectToHost(ui->lineEditHost->text(),
                            ui->lineEditPort->text().toInt());

    tcpClient.setSocketOption(QAbstractSocket::LowDelayOption, 1);//优化为最低延迟，后面的1代码启用该优化。

}

/* 一旦连接建立成功，QTcpSocket类将发出connected消息，继而调用
startTransfer()槽函数。
 */
void tcpClientFileSend::startTransfer()
{
#ifdef TIMETEST /* */
    QTime time;
    time.start(); //开始计时，以ms为单位

#endif

    timer->start(10);
    SaveIpAddr(ui->lineEditHost->text());

    if(namelst.count() > 6)
    {
        emit emitImgZeroSignal();
        return;
    }


    fileImage = grabframeGeometry();

    //    fileImage =fileImage.convertToFormat(QImage::Format_Indexed8,Qt::AutoColor);

    QString fileName = QString("%1/%2.jpg").arg(dirname)
            .arg(jpgnameNo++);
    qDebug() << "filename:" << fileName;
    namelst.append(fileName);

#if 0 //获取文件大小，太慢了
    QBuffer buffertmp;
    qint64 ImgBytes;
    fileImage.save(&buffertmp,STREAM_PIC_FORT);
    ImgBytes = (qint64)buffertmp.data().size();
    qDebug() << "filesize:" << ImgBytes;
    sizelst.append(ImgBytes);
#endif

    fileImage.save(fileName,STREAM_PIC_FORT);

#ifdef TIMETEST /* */

    int time_Diff = time.elapsed(); //返回从上次start()或restart()开始以来的时间差，单位ms
    //以下方法是将ms转为s
    float f = time_Diff / 1000.0;
    time_total += f;

    qDebug() << "save  elaspe:" <<time_Diff <<"ms";
    qDebug() << "current time:" <<time_total <<"s";

#if 0
    static int usedflag = 0;
    if( 0 == usedflag)
    {
        parseImage();
        usedflag = 1;
    }
#else
//    parseImage();
    emit emitImgZeroSignal();
#endif

#endif
    return;
}

void tcpClientFileSend::parseImage()
{

    if(namelst.count() == 0)
    {
        qDebug() << "namelst count is 0!!";
//        emit emitImgZeroSignal();
        return;
    }
    qDebug() << "namelst count:" << namelst.count();
//    if(namelst.count() > 100)
//    {
//        emit emitImgZeroSignal();
//    }
    if(SEND_ING == sendDoneFlag)
        return;

    sendDoneFlag = SEND_ING;

    outBlock.resize(0);
    QString readFname = namelst.at(0);
    qDebug() << "read file name:" << readFname;
    namelst.removeAt(0);
    qDebug() << "after delete namelst count:" << namelst.count();

    QDir dir(QDir::currentPath());
    if(!dir.exists(dirname))
    {
        dir.mkdir(dirname);
    }

    QFile file(readFname);
    if(!file.open(QIODevice::ReadOnly)) {
        qDebug()<<"Can't open the file!"<<readFname;
    }

/*---------------------------------------
发送文件数据格式 ：

1.总长度 (8bytes) -- 总字节(文件大小 + 8字节 + 文件名)
2.文件名长度(qint64() 8Bytes)
3.文件名
---------------------------------------*/


    outBlockFile = file.readAll();
    qDebug() <<"read file size:" << outBlockFile.size() ;

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
//    outBlock.resize(0);





    file.close();
    dir.remove(readFname);//删除文件

    ui->clientProgressBar->setMaximum(TotalBytes);
    ui->clientProgressBar->setValue(byteWritten);


    return;


#if 0
    //读取图片
#if 0
    QImageWriter writer(&buffer, STREAM_PIC_FORT);

    writer.write(fileImage);//写入图片
#else
    //    QImage image;
    fileImage.save(&buffer,STREAM_PIC_FORT);
#endif

    //图片大小
    TotalBytes = (qint64)buffer.data().size();

    QDataStream sendOut(&outBlock, QIODevice::WriteOnly);
    sendOut.resetStatus();
    sendOut.setVersion(QDataStream::Qt_4_0);


    /*---------------------------------------
数据流发送顺序：
文件名称 abc.jpg
文件名称的长度  7
图片大小  10000
图片高度 1366
图片宽度  768
---------------------------------------*/

    QString currentFile = QString("blog%1.%2").arg(picNametime).arg(SUFIXNAME);
    DDbg() << "send pic name:"<<currentFile;




    //发送文件名称
    sendOut <<qint64(0) <<qint64(0) <<currentFile;
    //TotalBytes为总数据长度，即（数据量长度+文件名长度+文件名）
    TotalBytes += outBlock.size(); //加上图片名称长度
    sendOut.device()->seek(0);
    /*---------------------------------------
发送文件数据格式 ：

1.总长度 (8bytes)
2.文件名长度(qint64() 8Bytes)
3.由文件名长度指定
---------------------------------------*/
    PrintInfoToFile(QString("TotalBytes:%1\n")
                    .arg(TotalBytes));

    DDbg() << "send pic name len:"<<qint64((outBlock.size() - sizeof(qint64)*2));
    PrintInfoToFile(QString("send pic name len:%1\n")
                    .arg(qint64((outBlock.size() - sizeof(qint64)*2))));

    PrintInfoToFile(QString("send pic name:%1 \n\n")
                    .arg(currentFile));

    //发送文件总大小 ，
    sendOut << TotalBytes << qint64((outBlock.size() - sizeof(qint64)*2));

    bytesToWrite = TotalBytes - tcpClient.write(outBlock);//将名称发出后，剩余图片大小
    ui->clientStatusLabel->setText(str_china("已连接"));
#ifdef DEBUG
    qDebug() << currentFile << TotalBytes;
#endif
//    outBlock.resize(0);

#endif

}

void tcpClientFileSend::updateClientProgress(qint64 numBytes)
{

#ifdef DEBUG
    DDbg() << "numBytes:--------->>"<<numBytes;
#endif
    byteWritten += (int)numBytes;
    if(bytesToWrite > 0)
    {
        qDebug() <<"-->:outBlockFile size:" << outBlockFile.size();
//        outBlock =  buffer.data();
        bytesToWrite -= (int)tcpClient.write(outBlockFile);
//        outBlock.resize(0);
        ui->clientProgressBar->setValue(byteWritten);
        qDebug() <<"-->:bytesToWrite size:" << bytesToWrite;
    }
    else
    {
        qDebug() << "-->: send image done!!";
        picNametime++;
        TotalBytes = 0;
        byteWritten = 0;
//        parseImage();
        sendDoneFlag = SEND_DONE;
    }



    ui->clientStatusLabel->setText(str_china("已发送 %1MB")
                                   .arg(byteWritten / (1024 *1024)));
}

void tcpClientFileSend::displayErr(QAbstractSocket::SocketError socketError)
{
    if(socketError == QTcpSocket::RemoteHostClosedError)
        return;
    QMessageBox::information(this,str_china("网络"),
                             str_china("产生如下错误： %1")
                             .arg(tcpClient.errorString()));

    tcpClient.close();
    ui->clientProgressBar->reset();
    ui->clientStatusLabel->setText(str_china("客户端就绪"));
    ui->startButton->setEnabled(true);

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
    else
    {
        return QPixmap::grabWindow(QApplication::desktop()->winId(),
                                   pos().x(),
                                   pos().y(),
                                   frameGeometry().width(),
                                   frameGeometry().height()).toImage();
    }

}

void tcpClientFileSend::ShutDownAll()
{
    byteWritten = 0;
    tcpClient.disconnectFromHost();
    tcpClient.waitForDisconnected(5000);
    tcpClient.close();

    ui->clientProgressBar->reset();
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

        qDebug() << "read ip:" << dataFromFile;
        ipaddr = QString(dataFromFile);
        qDebug() << "ipaddr :" << ipaddr;
        return ipaddr;
    }
    else
    {
        file.open(QIODevice::WriteOnly);
        file.write(DEFAULT_HOSTADDR);
        file.close();
        qDebug() << "read ip:" << DEFAULT_HOSTADDR;
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

