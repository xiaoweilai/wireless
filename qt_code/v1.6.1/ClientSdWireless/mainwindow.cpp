#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDesktopWidget>
#include <QDesktopServices>
#include <QTime>

/*��ʱ������*/
#define TIMERNUMS 100

//�ֱ��ʱȴ˴���ת��Ϊ�˷ֱ���
#define VGASTDHEIGHT 1024
#define VGASTDWIDTH  768

//��ӡ������Ϣ
#define DEBUGINFO
//ת���ֱ���
#define ZIPCONVERTPIX

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    getImgThd = new DoSendThread(NULL);
    getImgThd->start();
    connect(getImgThd,SIGNAL(emitMsgBoxSignal()),this,SLOT(receiveMsgBoxSignal()),Qt::DirectConnection);

    TimerSets();

}
//��ʱ����
void MainWindow::TimerSets()
{
    timer = new QTimer;
    connect(timer,SIGNAL(timeout()),this,SLOT(grabScreenSignal()),Qt::DirectConnection);
    timer->start(100);

    for( int i=0;i<TIMERNUMS;i++)
    {
        QTimer *tmptimer = new QTimer;
        timerlist.push_back(tmptimer);
    }

    QList<QTimer*>::iterator i;
    for (i = timerlist.begin(); i != timerlist.end(); ++i) {
        //      *i = (*i).toLower(); // ʹ�� * �������ȡ��������ָ��Ԫ��
        //       qDebug()<<*i;
        connect(*i,SIGNAL(timeout()),this,SLOT(grabScreenSignal()),Qt::DirectConnection);
        (*i)->start(100);

    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::receiveMsgBoxSignal()
{
    qDebug() << "receiveMsgBoxSignal";
    //    imglist.push_back(grabframeGeometry());
    //    QImage img = grabframeGeometry();
    //    qDebug() << "grab list size:"<<imglist.count();
}

void MainWindow::grabScreenSignal()
{
    static float time_total = 0;
    QTime time;
    time.start(); //��ʼ��ʱ����msΪ��λ
#if 0
    /*
grab list size: 2867
"time:30.8666 png/s"
now elaspe: 92.8837 s
    */
    imglist.push_back(covertPixTo1024768(grabframeGeometry()));
#else
/*
grab list size: 2433
"time:20.5653 png/s"
now elaspe: 118.306 s
*/
    imglist.push_back(covertPixTo1024768(grabDeskScreen()));
#endif
    qDebug() << "grab list size:"<<imglist.count();


    int time_Diff = time.elapsed(); //���ش��ϴ�start()��restart()��ʼ������ʱ����λms

    //���·����ǽ�msתΪs
    float f = time_Diff / 1000.0;
    time_total += f;
    //    float pngrate = imglist.count() / time_total;
    //    QString tr_timeDiff = QString("1 png time:%1").arg(pngrate); //float->QString


    //    qDebug() << tr_timeDiff;

    float pngrate = imglist.count() / time_total ;
    QString tr_timeDiff = QString("time:%1 png/s").arg(pngrate); //float->QString
    qDebug() << tr_timeDiff;
    qDebug() << "now elaspe:" <<time_total <<"s";

}

//����������ߴ�
QImage MainWindow::grabframeGeometry()
{
    return QPixmap::grabWindow(QApplication::desktop()->winId(),
                               pos().x(),
                               pos().y(),
                               frameGeometry().width(),
                               frameGeometry().height()).toImage();

}

//����������ߴ�
QImage MainWindow::grabDeskScreen()
{
    return  QPixmap::grabWindow(QApplication::desktop()->winId(),
                                0,
                                0,
                                QApplication::desktop()->width(),
                                QApplication::desktop()->height()).toImage();
}

//ת��ͼƬ�ֱ���
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
