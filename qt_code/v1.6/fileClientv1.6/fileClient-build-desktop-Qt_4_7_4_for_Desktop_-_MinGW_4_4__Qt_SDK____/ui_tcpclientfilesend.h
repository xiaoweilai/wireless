/********************************************************************************
** Form generated from reading UI file 'tcpclientfilesend.ui'
**
** Created: Sun Jan 18 16:05:42 2015
**      by: Qt User Interface Compiler version 4.7.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TCPCLIENTFILESEND_H
#define UI_TCPCLIENTFILESEND_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QMainWindow>
#include <QtGui/QProgressBar>
#include <QtGui/QPushButton>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_tcpClientFileSend
{
public:
    QWidget *centralWidget;
    QPushButton *startButton;
    QPushButton *quitButton;
    QPushButton *pauseButton;
    QProgressBar *clientProgressBar;
    QLabel *clientStatusLabel;
    QLabel *labelRate;
    QLineEdit *lineEditHost;
    QLabel *label;
    QLineEdit *lineEditPort;
    QLabel *label_2;

    void setupUi(QMainWindow *tcpClientFileSend)
    {
        if (tcpClientFileSend->objectName().isEmpty())
            tcpClientFileSend->setObjectName(QString::fromUtf8("tcpClientFileSend"));
        tcpClientFileSend->resize(537, 381);
        centralWidget = new QWidget(tcpClientFileSend);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        startButton = new QPushButton(centralWidget);
        startButton->setObjectName(QString::fromUtf8("startButton"));
        startButton->setGeometry(QRect(70, 190, 101, 61));
        quitButton = new QPushButton(centralWidget);
        quitButton->setObjectName(QString::fromUtf8("quitButton"));
        quitButton->setGeometry(QRect(360, 190, 101, 61));
        pauseButton = new QPushButton(centralWidget);
        pauseButton->setObjectName(QString::fromUtf8("pauseButton"));
        pauseButton->setGeometry(QRect(200, 190, 101, 61));
        clientProgressBar = new QProgressBar(centralWidget);
        clientProgressBar->setObjectName(QString::fromUtf8("clientProgressBar"));
        clientProgressBar->setGeometry(QRect(70, 130, 401, 31));
        clientProgressBar->setValue(0);
        clientStatusLabel = new QLabel(centralWidget);
        clientStatusLabel->setObjectName(QString::fromUtf8("clientStatusLabel"));
        clientStatusLabel->setGeometry(QRect(70, 270, 371, 31));
        labelRate = new QLabel(centralWidget);
        labelRate->setObjectName(QString::fromUtf8("labelRate"));
        labelRate->setGeometry(QRect(70, 320, 361, 41));
        lineEditHost = new QLineEdit(centralWidget);
        lineEditHost->setObjectName(QString::fromUtf8("lineEditHost"));
        lineEditHost->setGeometry(QRect(80, 70, 221, 41));
        label = new QLabel(centralWidget);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(160, 40, 41, 21));
        lineEditPort = new QLineEdit(centralWidget);
        lineEditPort->setObjectName(QString::fromUtf8("lineEditPort"));
        lineEditPort->setGeometry(QRect(340, 70, 91, 41));
        label_2 = new QLabel(centralWidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(380, 40, 31, 21));
        tcpClientFileSend->setCentralWidget(centralWidget);

        retranslateUi(tcpClientFileSend);

        QMetaObject::connectSlotsByName(tcpClientFileSend);
    } // setupUi

    void retranslateUi(QMainWindow *tcpClientFileSend)
    {
        tcpClientFileSend->setWindowTitle(QApplication::translate("tcpClientFileSend", "tcpClientFileSend", 0, QApplication::UnicodeUTF8));
        startButton->setText(QApplication::translate("tcpClientFileSend", "\345\274\200\345\247\213", 0, QApplication::UnicodeUTF8));
        quitButton->setText(QApplication::translate("tcpClientFileSend", "\351\200\200\345\207\272", 0, QApplication::UnicodeUTF8));
        pauseButton->setText(QApplication::translate("tcpClientFileSend", "\346\232\202\345\201\234", 0, QApplication::UnicodeUTF8));
        clientStatusLabel->setText(QApplication::translate("tcpClientFileSend", "\345\256\242\346\210\267\347\253\257\345\260\261\347\273\252", 0, QApplication::UnicodeUTF8));
        labelRate->setText(QString());
        label->setText(QApplication::translate("tcpClientFileSend", "\344\270\273\346\234\272", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("tcpClientFileSend", "\347\253\257\345\217\243", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class tcpClientFileSend: public Ui_tcpClientFileSend {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TCPCLIENTFILESEND_H
