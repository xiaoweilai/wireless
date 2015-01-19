#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDesktopWidget>
#include <QDesktopServices>
#include <QTime>

/*定时器个数*/
#define TIMERNUMS 40

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
//定时器集
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
        //      *i = (*i).toLower(); // 使用 * 运算符获取遍历器所指的元素
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
    time.start(); //开始计时，以ms为单位
#if 1
    /*
grab list size: 2867
"time:30.8666 png/s"
now elaspe: 92.8837 s
    */
    imglist.push_back(grabframeGeometry());
#else
/*
grab list size: 2433
"time:20.5653 png/s"
now elaspe: 118.306 s
*/
    imglist.push_back(grabDeskScreen());
#endif
    qDebug() << "grab list size:"<<imglist.count();


    int time_Diff = time.elapsed(); //返回从上次start()或restart()开始以来的时间差，单位ms

    //以下方法是将ms转为s
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

//截屏，界面尺寸
QImage MainWindow::grabframeGeometry()
{
    return QPixmap::grabWindow(QApplication::desktop()->winId(),
                               pos().x(),
                               pos().y(),
                               frameGeometry().width(),
                               frameGeometry().height()).toImage();

}

//截屏，桌面尺寸
QImage MainWindow::grabDeskScreen()
{
    return  QPixmap::grabWindow(QApplication::desktop()->winId(),
                                0,
                                0,
                                QApplication::desktop()->width(),
                                QApplication::desktop()->height()).toImage();
}

