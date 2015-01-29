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

    ui->startButton->setEnabled(false);
    ui->lineEditHost->setText(ReadIpAddr());
    ui->lineEditPort->setText(DEFAULT_PORT);

    QStringList screensize;
    screensize.clear();
    screensize <<"gemetory size"
              <<"800 X 600"
             <<"1024 X 768"
            <<"fullscreen";
    ui->comboBox_grabScreenSize->addItems(screensize);
    ui->comboBox_grabScreenSize->setCurrentIndex(1);//"800 X 600"

    QString dirname = QString("images");
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
    SaveIpAddr(ui->lineEditHost->text());

    fileImage = grabframeGeometry();

    //    fileImage =fileImage.convertToFormat(QImage::Format_Indexed8,Qt::AutoColor);


    buffer.reset();
    outBlock.resize(0);
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
    outBlock.resize(0);

}

void tcpClientFileSend::updateClientProgress(qint64 numBytes)
{
#ifdef DEBUG
    DDbg() << "numBytes:--------->>"<<numBytes;
#endif
    byteWritten += (int)numBytes;
    if(bytesToWrite > 0)
    {
        outBlock =  buffer.data();
        bytesToWrite -= (int)tcpClient.write(outBlock);
        outBlock.resize(0);
    }
    else
    {
        picNametime++;
        TotalBytes = 0;
        byteWritten = 0;
        startTransfer();
    }

    ui->clientProgressBar->setMaximum(TotalBytes);
    ui->clientProgressBar->setValue(byteWritten);
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

