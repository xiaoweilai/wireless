#include "threadsd.h"
#include <QDebug>
#include <windows.h>

#define VGASTDHEIGHT 1024
#define VGASTDWIDTH  768



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





threadSd::threadSd(QObject *parent/*, void *tcpClient*/) :
    QThread(parent)
{
    counter  = 0;
    imgset.clear();
//    m_client = (QTcpSocket*)tcpClient;
}

void threadSd::run()
{
    while(1)
    {
        if(0 == counter)
            continue;
        qDebug() << "i am running....";
        rvimg = imgset.at(0);
        rvimg = covertPixTo1024768(rvimg);
        imgset.remove(0);

//处理发送数据
        buffer.reset();
        outBlock.resize(0);

        //    QImage image;
        rvimg.save(&buffer,STREAM_PIC_FORT);

        //图片大小
        TotalBytes = (qint64)buffer.data().size();


        QDataStream sendOut(&outBlock, QIODevice::WriteOnly);
        sendOut.resetStatus();
        sendOut.setVersion(QDataStream::Qt_4_0);


        QString currentFile = QString("blog%1.%2").arg(picNametime).arg(SUFIXNAME);
        qDebug() << "send pic name:"<<currentFile;


        //发送文件名称
        sendOut <<qint64(0) <<qint64(0) <<currentFile;
        //TotalBytes为总数据长度，即（数据量长度+文件名长度+文件名）
        TotalBytes += outBlock.size(); //加上图片名称长度
        sendOut.device()->seek(0);


        PrintInfoToFile(QString("TotalBytes:%1\n")
                        .arg(TotalBytes));

        qDebug() << "send pic name len:"<<qint64((outBlock.size() - sizeof(qint64)*2));
        PrintInfoToFile(QString("send pic name len:%1\n")
                        .arg(qint64((outBlock.size() - sizeof(qint64)*2))));


        PrintInfoToFile(QString("send pic name:%1 \n\n")
                        .arg(currentFile));

        //发送文件总大小 ，
        sendOut << TotalBytes << qint64((outBlock.size() - sizeof(qint64)*2));

        bytesToWrite = TotalBytes -
                mp_client->write(outBlock);//将名称发出后，剩余图片大小
        outBlock.resize(0);









        counter--;

        Sleep(100);
    }

}

void threadSd::getImage(QImage  img, void *tcpclient)
{
    qDebug() << "beforeimage ht:" <<img.height();
    qDebug() << "beforeimage wt:" <<img.width();
    counter++;
    imgset.push_back(img);
    mp_client = (QTcpSocket *)tcpclient;
}


QImage threadSd::covertPixTo1024768(QImage & img)
{
    QImage endImg;
    endImg = img;
    int imgHt = img.width();
    int imgWt = img.height();
    if(imgHt >=VGASTDHEIGHT && imgWt >= VGASTDWIDTH)
        endImg = img.scaled(VGASTDHEIGHT, VGASTDWIDTH);

    qDebug() << "end img ht:" << endImg.height();
    qDebug() << "end img wt:" << endImg.width();
    return endImg;
}


void threadSd::PrintInfoToFile(QString str)
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
