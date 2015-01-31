#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "dosendthread.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
private://var
    Ui::MainWindow *ui;
    DoSendThread *getImgThd;
    QList<QImage> imglist;
    QList<QTimer*> timerlist;

private://func
    QImage grabframeGeometry();
    QImage grabDeskScreen();
    QImage covertPixTo1024768(QImage img);
    void TimerSets();
    void ButtonSets();
    void ComBoxSets();
private slots:
    void receiveMsgBoxSignal();
    void grabScreenSignal();
    void startActive();
    void pauseActive();
    void quitActive();
};

#endif // MAINWINDOW_H
