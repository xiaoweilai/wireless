#include "tcp_fileserver_recv.h"
#include "ui_tcp_fileserver_recv.h"

#if 1
//编码汉字
//#define str_china(A)     QString::fromLocal8Bit(#A)
#define str_china(A)     QString::fromLocal8Bit(A)
//#define str_china(A)     QString::fromUtf8(#A)
#else
#define str_china(A)     codec->toUnicode(#A)
#endif

#define T1M (1*1024*1024)
#define T1K (1*1024)

Tcp_FileServer_Recv::Tcp_FileServer_Recv(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Tcp_FileServer_Recv)
{
    ui->setupUi(this);

    TotalBytes = 0;
    bytesReceived = 0;
    fileNameSize = 0;

    connect(ui->startButton, SIGNAL(clicked()), this, SLOT(start()));
    connect(ui->quitButton, SIGNAL(clicked()), this, SLOT(close()));
    connect(&tcpServer, SIGNAL(newConnection()),
            this, SLOT(acceptConnection()));
}

Tcp_FileServer_Recv::~Tcp_FileServer_Recv()
{
    delete ui;
}

void Tcp_FileServer_Recv::start()
{
    ui->startButton->setEnabled(false);

    QApplication::setOverrideCursor(Qt::WaitCursor);
    bytesReceived = 0;

    while (!tcpServer.isListening() && !tcpServer.listen(QHostAddress::LocalHost,16689)) {
        QMessageBox::StandardButton ret = QMessageBox::critical(this,
                                                                str_china("回环"),
                                                                str_china("无法开始测试: %1.")
                                                                .arg(tcpServer.errorString()),
                                                                QMessageBox::Retry
                                                                | QMessageBox::Cancel);
        if (ret == QMessageBox::Cancel)
            return;
    }
    ui->serverStatusLabel->setText(str_china("监听"));
}

void Tcp_FileServer_Recv::acceptConnection()
{
    tcpServerConnection = tcpServer.nextPendingConnection();
    connect(tcpServerConnection, SIGNAL(readyRead()),
            this, SLOT(updateServerProgress()));
    connect(tcpServerConnection, SIGNAL(error(QAbstractSocket::SocketError)),
            this, SLOT(displayError(QAbstractSocket::SocketError)));

    ui->serverStatusLabel->setText(str_china("接受连接"));
    tcpServer.close();
}

void Tcp_FileServer_Recv::updateServerProgress()
{
#if 1
    QDataStream in(tcpServerConnection);
    in.setVersion(QDataStream::Qt_4_0);
//    QTime cTime;
//    cTime.start();
//    static quint64 etime = 0;


    if(bytesReceived <= sizeof(qint64)*2){
        if((tcpServerConnection->bytesAvailable() >= sizeof(qint64)*2)&&(fileNameSize ==0)){
            in>>TotalBytes>>fileNameSize;
            bytesReceived += sizeof(qint64)*2;
            inBlock.resize(0);
        }
        if((tcpServerConnection->bytesAvailable() >= fileNameSize)&&(fileNameSize !=0)){
            in>>fileName;
            qDebug() << "filename:" <<fileName;

            bytesReceived += fileNameSize;
//            localFile = new QFile(fileName);
//            if (!localFile->open(QFile::WriteOnly )) {
//                QMessageBox::warning(this, str_china("应用程序"),
//                                     str_china("无法读取文件 %1:\n%2.")
//                                     .arg(fileName)
//                                     .arg(localFile->errorString()));
//                return;
//            }
        }else{
            return;
        }
    }


    if (bytesReceived < TotalBytes){
#if 1
        bytesReceived += tcpServerConnection->bytesAvailable();
//        inBlock = tcpServerConnection->readAll();
//        localFile->write(inBlock);
        inBlock.append(tcpServerConnection->readAll());

#else
        bytesReceived += tcpServerConnection->bytesAvailable();
        inBlock = tcpServerConnection->readAll();
        localFile->write(inBlock);
        inBlock.resize(0);
#endif
    }
    ui->serverProgressBar->setMaximum(TotalBytes);
    ui->serverProgressBar->setValue(bytesReceived);
//    qDebug()<<"bytesReceived---->>>>>>>>>"<< bytesReceived;
    ui->serverStatusLabel->setText(str_china("已接收 %1MB")
                                   .arg(bytesReceived / (1024 * 1024)));

    if (bytesReceived == TotalBytes) {
//        tcpServerConnection->close();
//        ui->startButton->setEnabled(true);

//        QByteArray array = socket.read( dataSize );
        QBuffer buffer(&inBlock);
        buffer.open( QIODevice::ReadOnly );

        QImageReader reader(&buffer, STREAM_PIC_FORT);
        QImage image = reader.read();

        if ( !image.isNull() ) {
            ui->imageLabel->setPixmap( QPixmap::fromImage( image ) );
//            ui->imageLabel->setText( tr("") );
        }
        else {
            ui->imageLabel->setText( tr("<i>Invalid image received!</i>") );
        }


        QApplication::restoreOverrideCursor();

        inBlock.resize(0);

//        localFile->close();
        TotalBytes = 0;
        bytesReceived = 0;
        fileNameSize = 0;





//        qDebug() << "consume time:" << etime;
//        qDebug() << "transfer rate:" << (bytesReceived/T1M)/(etime/1000.00)
//                 <<"MB/s";
//        qDebug() << "transfer rate:" << (bytesReceived/T1K)/(etime/1000)
//                 <<"KB/s";
    }
//    etime += cTime.elapsed();
//    qDebug() << "consume time:" << etime;

#else
    QDataStream in(tcpServerConnection);
    in.setVersion(QDataStream::Qt_4_3);
    QTime cTime;
    cTime.start();
    static quint64 etime = 0;


    if(bytesReceived <= sizeof(qint64)*2){
        if((tcpServerConnection->bytesAvailable() >= sizeof(qint64)*2)&&(fileNameSize ==0)){
            in>>TotalBytes>>fileNameSize;
            bytesReceived += sizeof(qint64)*2;
        }
        if((tcpServerConnection->bytesAvailable() >= fileNameSize)&&(fileNameSize !=0)){
            in>>fileName;
            qDebug() << "filename:" <<fileName;

            bytesReceived += fileNameSize;
            localFile = new QFile(fileName);
            if (!localFile->open(QFile::WriteOnly )) {
                QMessageBox::warning(this, str_china("应用程序"),
                                     str_china("无法读取文件 %1:\n%2.")
                                     .arg(fileName)
                                     .arg(localFile->errorString()));
                return;
            }
        }else{
            return;
        }
    }


    if (bytesReceived < TotalBytes){
        bytesReceived += tcpServerConnection->bytesAvailable();
        inBlock = tcpServerConnection->readAll();
        localFile->write(inBlock);
        inBlock.resize(0);
    }
    ui->serverProgressBar->setMaximum(TotalBytes);
    ui->serverProgressBar->setValue(bytesReceived);
    qDebug()<<bytesReceived;
    ui->serverStatusLabel->setText(str_china("已接收 %1MB")
                                   .arg(bytesReceived / (1024 * 1024)));

    if (bytesReceived == TotalBytes) {
        tcpServerConnection->close();
        ui->startButton->setEnabled(true);
        QApplication::restoreOverrideCursor();

        localFile->close();
        qDebug() << "consume time:" << etime;
        qDebug() << "transfer rate:" << (bytesReceived/T1M)/(etime/1000.00)
                 <<"MB/s";
        qDebug() << "transfer rate:" << (bytesReceived/T1K)/(etime/1000)
                 <<"KB/s";
    }
    etime += cTime.elapsed();
    qDebug() << "consume time:" << etime;
#endif
}

void Tcp_FileServer_Recv::displayError(QAbstractSocket::SocketError socketError)
{
    if (socketError == QTcpSocket::RemoteHostClosedError)
        return;

    QMessageBox::information(this, str_china("网络错误"),
                             str_china("产生如下错误: %1.")
                             .arg(tcpServerConnection->errorString()));
    tcpServerConnection->close();
    ui->serverProgressBar->reset();
    ui->serverStatusLabel->setText(str_china("服务端就绪"));
    ui->startButton->setEnabled(true);
    QApplication::restoreOverrideCursor();
}
