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

#define VGASTDHEIGHT 1024
#define VGASTDWIDTH  768


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
#if 1
    ui->setupUi(this);

    tcpClient  =new QTcpSocket;

    threadsd = new threadSd(this/*, tcpClient*/);
//    threadsd->start();

    qDebug() << "sizeof(qint64)*2:" <<sizeof(qint64)*2;

    //    timer = new QTimer;
    //    timer->start(100);
    //    connect(timer,SIGNAL(timeout()),this,SLOT(saveImage()));
    picNametime = 1;

    loadSize = 4*1024;
    TotalBytes = 0;
    byteWritten = 0;

    ui->startButton->setEnabled(true);
    ui->lineEditHost->setText(ReadIpAddr());
    ui->lineEditPort->setText(DEFAULT_PORT);


    connect(ui->startButton,SIGNAL(clicked()),this,SLOT(start()));
    connect(ui->quitButton,SIGNAL(clicked()),this,SLOT(ShutDownAll()));
    connect(ui->pauseButton,SIGNAL(clicked()),this,SLOT(pauseOps()));

    connect(tcpClient,SIGNAL(connected()),this,
            SLOT(startTransfer()));
    connect(tcpClient,SIGNAL(bytesWritten(qint64)),this,
            SLOT(updateClientProgress(qint64)));
    connect(tcpClient,SIGNAL(error(QAbstractSocket::SocketError)),this,
            SLOT(displayErr(QAbstractSocket::SocketError)));

#else //org
    ui->setupUi(this);

    loadSize = 4*1024;
    TotalBytes = 0;
    byteWritten = 0;

    ui->startButton->setEnabled(false);
    ui->lineEditHost->setText(DEFAULT_HOSTADDR);
    ui->lineEditPort->setText(DEFAULT_PORT);


    connect(ui->startButton,SIGNAL(clicked()),this,SLOT(start()));
    connect(ui->quitButton,SIGNAL(clicked()),this,SLOT(close()));
    connect(ui->openButton,SIGNAL(clicked()),this,SLOT(openFile()));

    connect(&tcpClient,SIGNAL(connected()),this,
            SLOT(startTransfer()));
    connect(&tcpClient,SIGNAL(bytesWritten(qint64)),this,
            SLOT(updateClientProgress(qint64)));
    connect(&tcpClient,SIGNAL(error(QAbstractSocket::SocketError)),this,
            SLOT(displayErr(QAbstractSocket::SocketError)));

#endif
}


void tcpClientFileSend::start()
{
    ui->startButton->setEnabled(true);
    ui->startButton->setEnabled(false);
    ui->pauseButton->setEnabled(true);
#ifdef SHOWCURSOR
    QApplication::setOverrideCursor(Qt::WaitCursor);
#endif
    byteWritten = 0;
    ui->clientStatusLabel->setText(str_china("连接中..."));
    tcpClient->connectToHost(ui->lineEditHost->text(),
                            ui->lineEditPort->text().toInt());

    threadsd->start();
}
void tcpClientFileSend::pauseOps()
{
    byteWritten = 0;
    tcpClient->disconnectFromHost();
    //    tcpClient.close();
    ui->clientProgressBar->reset();
    ui->clientStatusLabel->setText(str_china("客户端就绪"));
    ui->startButton->setEnabled(true);
    ui->pauseButton->setEnabled(false);

}

/* 一旦连接建立成功，QTcpSocket类将发出connected消息，继而调用
startTransfer()槽函数。
 */
void tcpClientFileSend::startTransfer()
{
#if 1

#ifdef USEIMAGEVEC//too slow
    //    fileImage = new QFile(fileName);
    if(!imageVec.count())
    {
        //        QMessageBox::warning(this,str_china("应用程序"),
        //                             str_china("无法读取图片，数量为零:\n%1.")
        //                             .arg(localFile->errorString()));
        return;
    }

#endif

    //    Sleep(30);

#ifdef USEIMAGEVEC
    fileImage = imageVec.at(0);
#else

#if 0//软件尺寸
    fileImage = grabframeGeometry();
#else//桌面尺寸
    fileImage = grabDeskScreen();
    threadsd->getImage(fileImage, (void*)tcpClient);
//    fileImage = covertPixTo1024768(fileImage);
#endif
//    qDebug() << "get image height:" <<fileImage.height();
//    qDebug() << "get image width :" <<fileImage.width();

    //处理图片为1024 * 768



#endif

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
    //    PrintInfoToFile(QString("reset outBlock size:%1 \n")
    //                    .arg(outBlock.size()));
    //图片名称
    /*
数据流发送顺序：


文件名称 abc.jpg
文件名称的长度  7
图片大小  10000
图片高度 1366
图片宽度  768




*/

    QString currentFile = QString("blog%1.%2").arg(picNametime).arg(SUFIXNAME);
    DDbg() << "send pic name:"<<currentFile;




    //发送文件名称
    sendOut <<qint64(0) <<qint64(0) <<currentFile;
    //TotalBytes为总数据长度，即（数据量长度+文件名长度+文件名）
    TotalBytes += outBlock.size(); //加上图片名称长度
    sendOut.device()->seek(0);
    /* 发送文件数据格式 ：
    总长度 (8bytes) | 文件名长度(qint64() 8Bytes) |由文件名长度指定
*/
    //    PrintInfoToFile(QString("before outBlock size:%1 \n")
    //                    .arg(outBlock.size()));
    //    PrintInfoToFile(QString("file length:%1 \n")
    //                    .arg(currentFile.length()));
    PrintInfoToFile(QString("TotalBytes:%1\n")
                    .arg(TotalBytes));

    DDbg() << "send pic name len:"<<qint64((outBlock.size() - sizeof(qint64)*2));
    PrintInfoToFile(QString("send pic name len:%1\n")
                    .arg(qint64((outBlock.size() - sizeof(qint64)*2))));

    //    PrintInfoToFile(QString("after outBlock size:%1 \n")
    //                    .arg(outBlock.size()));

    PrintInfoToFile(QString("send pic name:%1 \n\n")
                    .arg(currentFile));

    //发送文件总大小 ，
    sendOut << TotalBytes << qint64((outBlock.size() - sizeof(qint64)*2));

    bytesToWrite = TotalBytes - tcpClient->write(outBlock);//将名称发出后，剩余图片大小
    ui->clientStatusLabel->setText(str_china("已连接"));
#ifdef DEBUG
    DDbg() << currentFile << TotalBytes;
#endif
    outBlock.resize(0);

#else

    localFile = new QFile(fileName);
    if(!localFile->open(QFile::ReadOnly))
    {
        QMessageBox::warning(this,str_china("应用程序"),
                             str_china("无法读取文件 %1:\n%2.")
                             .arg(fileName)
                             .arg(localFile->errorString()));
        return;
    }
    TotalBytes = localFile->size();
    QDataStream sendOut(&outBlock, QIODevice::WriteOnly);
    sendOut.setVersion(QDataStream::Qt_4_0);

    QString currentFile = fileName.right(fileName.size() -
                                         fileName.lastIndexOf('/') -1);
    sendOut <<qint64(0) <<qint64(0) <<currentFile;
    TotalBytes += outBlock.size();
    sendOut.device()->seek(0);
    /* 发送文件数据格式 ：
    总长度 (8bytes) | 文件名长度(qint64() 8Bytes) |由文件名长度指定
*/
    sendOut << TotalBytes << qint64((outBlock.size() - sizeof(qint64)*2));
    bytesToWrite = TotalBytes - tcpClient.write(outBlock);
    ui->clientStatusLabel->setText(str_china("已连接"));
    DDbg() << currentFile << TotalBytes;
    outBlock.resize(0);
#endif

    ui->clientStatusLabel->setText(str_china("已连接"));



}

void tcpClientFileSend::updateClientProgress(qint64 numBytes)
{
#ifdef USEIMAGEVEC
    if(!imageVec.count())
        return;

#endif
#ifdef DEBUG
    DDbg() << "numBytes:--------->>"<<numBytes;
#endif
    byteWritten += (int)numBytes;
    if(bytesToWrite > 0)
    {
        //        outBlock = localFile->read(qMin(bytesToWrite,loadSize));
        outBlock =  buffer.data();
        bytesToWrite -= (int)tcpClient->write(outBlock);
        outBlock.resize(0);
        //        QTime cTime;
        //        cTime.start();
        //        DDbg() << "consume time:" << cTime.elapsed();
    }
    else
    {
        //        localFile->close();
#ifdef USEIMAGEVEC
        imageVec.remove(0);
#endif
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
#ifdef DEBUG_ERROR
    QMessageBox::information(this,str_china("网络"),
                             str_china("产生如下错误： %1")
                             .arg(tcpClient.errorString()));
#endif
    tcpClient->close();
    ui->clientProgressBar->reset();
    ui->clientStatusLabel->setText(str_china("客户端就绪"));
    ui->startButton->setEnabled(true);

#ifdef SHOWCURSOR
    QApplication::restoreOverrideCursor();
#endif
#ifdef USEIMAGEVEC
    imageVec.clear();
#endif
}

void tcpClientFileSend::saveImage()
{
#ifdef USEIMAGEVEC
    if(imageVec.count() >= OVERIMAGENUMS )
        return;
    imageVec.append(grabframeGeometry());
    DDbg() << "gpn:"<<imageVec.count();
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
    return QPixmap::grabWindow(QApplication::desktop()->winId(),
                               pos().x(),
                               pos().y(),
                               frameGeometry().width(),
                               frameGeometry().height()).toImage();

}

void tcpClientFileSend::ShutDownAll()
{
    threadsd->deleteLater();//线程干掉

    byteWritten = 0;
    tcpClient->disconnectFromHost();
    tcpClient->close();

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


QImage tcpClientFileSend::covertPixTo1024768(QImage & img)
{
    int imgHt = img.width();
    int imgWt = img.height();
    if(imgHt >=VGASTDHEIGHT && imgWt >= VGASTDWIDTH)
        return img.scaled(VGASTDHEIGHT, VGASTDWIDTH);

}

tcpClientFileSend::~tcpClientFileSend()
{
    delete ui;
}
