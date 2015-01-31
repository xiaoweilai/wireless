#include "tcpclientfilesend.h"
#include "ui_tcpclientfilesend.h"
#include <QTextCodec>
#include <QPixmap>
#include <QDesktopWidget>
#include <windows.h>
#include <QIODevice>

#define DEBUG    /* ������Ϣ */
#define TIMETEST /* ��ʱ���� */

const char version_filetransClient[]="v1.0";


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

    ui->setupUi(this);

    picNametime = 1;
    TotalBytes = 0;
    byteWritten = 0;

    jpgnameNo = 1; //ͼƬ���Ƽ���
    time_total = 0.0;//��ʱʱ��
    namelst.clear();//������ļ����б�
    sizelst.clear();//������ļ���С�б�
    sendDoneFlag = SEND_DONE;//���ͽ�����־

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
    ui->clientStatusLabel->setText(str_china("������ʼԶ�̴���"));
}


void tcpClientFileSend::start()
{
#ifdef SHOWCURSOR
    QApplication::setOverrideCursor(Qt::WaitCursor);
#endif
    byteWritten = 0;
    ui->clientStatusLabel->setText(str_china("������..."));
    tcpClient.connectToHost(ui->lineEditHost->text(),
                            QString(DEFAULT_PORT).toInt());
    tcpClient.setSocketOption(QAbstractSocket::LowDelayOption, 1);//�Ż�Ϊ����ӳ٣������1�������ø��Ż���

    //waitForConnected()�ȴ�����֪���������ȴ�ʱ�䡣������ӽ�����������true�����򷵻�false��
    //������falseʱ���Ե���error��ȷ���޷����ӵ�ԭ��
    if(!tcpClient.waitForConnected(3000))
    {
        qDebug() <<"Error: "<<tcpClient.errorString();
        displayErr(QAbstractSocket::SocketTimeoutError);
    }
}

/* һ�����ӽ����ɹ���QTcpSocket�ཫ����connected��Ϣ���̶�����
startTransfer()�ۺ�����
 */
void tcpClientFileSend::startTransfer()
{
#ifdef TIMETEST /* ��ʱ���� */
    QTime time;
    time.start(); //��ʼ��ʱ����msΪ��λ

#endif /* ��ʱ���� */

    //    ui->clientStatusLabel->setText(str_china("���ӳɹ�"));

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

#ifdef TIMETEST /* ��ʱ���� */

    int time_Diff = time.elapsed(); //���ش��ϴ�start()��restart()��ʼ������ʱ����λms
    //���·����ǽ�msתΪs
    float f = time_Diff / 1000.0;
    time_total += f;

#ifdef DEBUG
    qDebug() << "save  elaspe:" <<time_Diff <<"ms";
    qDebug() << "current time:" <<time_total <<"s";
#endif

#endif /* ��ʱ���� */


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
�����ļ����ݸ�ʽ ��

1.�ܳ��� (8bytes) -- ���ֽ�(�ļ���С + 8�ֽ� + �ļ���)
2.�ļ�������(qint64() 8Bytes)
3.�ļ���
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

    //�����ļ�����
    sendOut <<qint64(0) <<qint64(0) <<readFname;
    //TotalBytesΪ�����ݳ��ȣ���������������+�ļ�������+�ļ�����
    TotalBytes += outBlock.size(); //����ͼƬ���Ƴ���
    sendOut.device()->seek(0);

    //���ֽ�(�ļ���С + 8�ֽ� + �ļ���) ��
    sendOut << TotalBytes << qint64((outBlock.size() - sizeof(qint64)*2));

    bytesToWrite = TotalBytes - tcpClient.write(outBlock);//�����Ʒ�����ʣ��ͼƬ��С
    ui->clientStatusLabel->setText(str_china("������"));
#ifdef DEBUG
    qDebug() << "TotalBytes:" << TotalBytes;
#endif

    file.close();
    dir.remove(readFname);//ɾ���ļ�


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

    ui->clientStatusLabel->setText(str_china("������..."));

}

void tcpClientFileSend::displayErr(QAbstractSocket::SocketError socketError)
{
    if(socketError == QTcpSocket::RemoteHostClosedError)
        return;
    QMessageBox::information(this,str_china("����"),
                             str_china("�������´��� %1")
                             .arg(tcpClient.errorString()));

    tcpClient.close();

    ui->clientStatusLabel->setText(str_china("�ͻ��˾���"));
    ui->startButton->setEnabled(true);
    deleteImgs();
    namelst.clear();
#ifdef SHOWCURSOR
    QApplication::restoreOverrideCursor();
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


    ui->clientStatusLabel->setText(str_china("�ͻ��˾���"));
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
    return str_china("���洫��ϵͳ")
            +"\n"
            +str_china("byСκ��")
            +"\n"
            +version_filetransClient;
}

void tcpClientFileSend::aboutVer()
{
    QMessageBox::information(NULL, str_china("�汾"), GetVersion(),NULL,NULL);
    return;
}
