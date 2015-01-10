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
//���뺺��
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


    //    timer = new QTimer;
    //    timer->start(100);
    //    connect(timer,SIGNAL(timeout()),this,SLOT(saveImage()));
    picNametime = 1;

    loadSize = 4*1024;
    TotalBytes = 0;
    byteWritten = 0;

    ui->startButton->setEnabled(false);
    ui->lineEditHost->setText(DEFAULT_HOSTADDR);
    ui->lineEditPort->setText(DEFAULT_PORT);


    connect(ui->startButton,SIGNAL(clicked()),this,SLOT(start()));
    connect(ui->quitButton,SIGNAL(clicked()),this,SLOT(ShutDownAll()));
    connect(ui->openButton,SIGNAL(clicked()),this,SLOT(openFile()));

    connect(&tcpClient,SIGNAL(connected()),this,
            SLOT(startTransfer()));
    connect(&tcpClient,SIGNAL(bytesWritten(qint64)),this,
            SLOT(updateClientProgress(qint64)));
    connect(&tcpClient,SIGNAL(error(QAbstractSocket::SocketError)),this,
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

tcpClientFileSend::~tcpClientFileSend()
{
    delete ui;
}

void tcpClientFileSend::openFile()
{
#if 0
    fileImage = imageVec.at(0);
    if(imageVec.count())
    {
        ui->startButton->setEnabled(true);
    }
#elif 1
    ui->startButton->setEnabled(true);

#else
    fileName = QFileDialog::getOpenFileName(this);
    if(!fileName.isEmpty())
    {
        ui->startButton->setEnabled(true);
    }
#endif
}


void tcpClientFileSend::start()
{
    //    ui->startButton->setEnabled(false);
#ifdef SHOWCURSOR
    QApplication::setOverrideCursor(Qt::WaitCursor);
#endif
    byteWritten = 0;
    ui->clientStatusLabel->setText(str_china("������..."));
    tcpClient.connectToHost(ui->lineEditHost->text(),
                            ui->lineEditPort->text().toInt());


}

/* һ�����ӽ����ɹ���QTcpSocket�ཫ����connected��Ϣ���̶�����
startTransfer()�ۺ�����
 */
void tcpClientFileSend::startTransfer()
{
#if 1

#ifdef USEIMAGEVEC//too slow
    //    fileImage = new QFile(fileName);
    if(!imageVec.count())
    {
        //        QMessageBox::warning(this,str_china("Ӧ�ó���"),
        //                             str_china("�޷���ȡͼƬ������Ϊ��:\n%1.")
        //                             .arg(localFile->errorString()));
        return;
    }

#endif

    //    Sleep(30);

#ifdef USEIMAGEVEC
    fileImage = imageVec.at(0);
#else

#if 0//����ߴ�
    fileImage = grabframeGeometry();
#else//����ߴ�
    fileImage = grabDeskScreen();
#endif


#endif

    buffer.reset();
    outBlock.resize(0);
#if 0
    QImageWriter writer(&buffer, STREAM_PIC_FORT);

    writer.write(fileImage);//д��ͼƬ
#else
    //    QImage image;
    fileImage.save(&buffer,STREAM_PIC_FORT);
#endif

    //ͼƬ��С
    TotalBytes = (qint64)buffer.data().size();

    QDataStream sendOut(&outBlock, QIODevice::WriteOnly);
    sendOut.resetStatus();
    sendOut.setVersion(QDataStream::Qt_4_0);
    //ͼƬ����
    /*
����������˳��


�ļ����� abc.jpg
�ļ����Ƶĳ���  7
ͼƬ��С  10000
ͼƬ�߶� 1366
ͼƬ���  768




*/
    QString currentFile = QString("blog%1.%2").arg(picNametime).arg(SUFIXNAME);
    DDbg() << "send pic name:"<<currentFile;
    DDbg() << "send pic size:"<<outBlock.size();
    sendOut <<qint64(0) <<qint64(0) <<currentFile;
    TotalBytes += outBlock.size(); //����ͼƬ���Ƴ���
    sendOut.device()->seek(0);
    /* �����ļ����ݸ�ʽ ��
    �ܳ��� (8bytes) | �ļ�������(qint64() 8Bytes) |���ļ�������ָ��
*/
    sendOut << TotalBytes << qint64((outBlock.size() - sizeof(qint64)*2));
    bytesToWrite = TotalBytes - tcpClient.write(outBlock);//�����Ʒ�����ʣ��ͼƬ��С
    ui->clientStatusLabel->setText(str_china("������"));
#ifdef DEBUG
    DDbg() << currentFile << TotalBytes;
#endif
    outBlock.resize(0);

#else

    localFile = new QFile(fileName);
    if(!localFile->open(QFile::ReadOnly))
    {
        QMessageBox::warning(this,str_china("Ӧ�ó���"),
                             str_china("�޷���ȡ�ļ� %1:\n%2.")
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
    /* �����ļ����ݸ�ʽ ��
    �ܳ��� (8bytes) | �ļ�������(qint64() 8Bytes) |���ļ�������ָ��
*/
    sendOut << TotalBytes << qint64((outBlock.size() - sizeof(qint64)*2));
    bytesToWrite = TotalBytes - tcpClient.write(outBlock);
    ui->clientStatusLabel->setText(str_china("������"));
    DDbg() << currentFile << TotalBytes;
    outBlock.resize(0);
#endif
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
        bytesToWrite -= (int)tcpClient.write(outBlock);
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
    ui->clientStatusLabel->setText(str_china("�ѷ��� %1MB")
                                   .arg(byteWritten / (1024 *1024)));
}

void tcpClientFileSend::displayErr(QAbstractSocket::SocketError socketError)
{
    if(socketError == QTcpSocket::RemoteHostClosedError)
        return;
    QMessageBox::information(this,str_china("����"),
                             str_china("�������´��� %1")
                             .arg(tcpClient.errorString()));

    tcpClient.close();
    ui->clientProgressBar->reset();
    ui->clientStatusLabel->setText(str_china("�ͻ��˾���"));
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

//����ߴ�
QImage tcpClientFileSend::grabDeskScreen()
{
    return  QPixmap::grabWindow(QApplication::desktop()->winId(),
                                0,
                                0,
                                QApplication::desktop()->width(),
                                QApplication::desktop()->height()).toImage();
}

//����ߴ�
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
    byteWritten = 0;
    tcpClient.disconnectFromHost();
    tcpClient.close();

    ui->clientProgressBar->reset();
    ui->clientStatusLabel->setText(str_china("�ͻ��˾���"));
    ui->startButton->setEnabled(true);
    close();

}
