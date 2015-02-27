/********************************************************************************
** Form generated from reading UI file 'tcpclientfilesend.ui'
**
** Created: Fri Feb 27 20:09:10 2015
**      by: Qt User Interface Compiler version 4.8.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TCPCLIENTFILESEND_H
#define UI_TCPCLIENTFILESEND_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QComboBox>
#include <QtGui/QGroupBox>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QMainWindow>
#include <QtGui/QPushButton>
#include <QtGui/QRadioButton>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_tcpClientFileSend
{
public:
    QWidget *centralWidget;
    QPushButton *startButton;
    QPushButton *pauseButton;
    QPushButton *openButton;
    QLabel *clientStatusLabel;
    QLineEdit *lineEditHost;
    QLabel *label;
    QComboBox *comboBox_grabScreenSize;
    QPushButton *pushbtn_about;
    QLabel *clientStatusLabel_2;
    QCheckBox *checkBox_speed;
    QPushButton *disconnectButton;
    QGroupBox *groupBox;
    QRadioButton *radioButton_smooth;
    QRadioButton *radioButton_highpix;

    void setupUi(QMainWindow *tcpClientFileSend)
    {
        if (tcpClientFileSend->objectName().isEmpty())
            tcpClientFileSend->setObjectName(QString::fromUtf8("tcpClientFileSend"));
        tcpClientFileSend->resize(531, 335);
        centralWidget = new QWidget(tcpClientFileSend);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        startButton = new QPushButton(centralWidget);
        startButton->setObjectName(QString::fromUtf8("startButton"));
        startButton->setGeometry(QRect(60, 200, 131, 71));
        QFont font;
        font.setFamily(QString::fromUtf8("\345\256\213\344\275\223"));
        font.setPointSize(20);
        startButton->setFont(font);
        pauseButton = new QPushButton(centralWidget);
        pauseButton->setObjectName(QString::fromUtf8("pauseButton"));
        pauseButton->setGeometry(QRect(220, 200, 121, 71));
        pauseButton->setFont(font);
        openButton = new QPushButton(centralWidget);
        openButton->setObjectName(QString::fromUtf8("openButton"));
        openButton->setGeometry(QRect(320, 70, 91, 51));
        openButton->setFont(font);
        clientStatusLabel = new QLabel(centralWidget);
        clientStatusLabel->setObjectName(QString::fromUtf8("clientStatusLabel"));
        clientStatusLabel->setGeometry(QRect(50, 280, 441, 31));
        clientStatusLabel->setFont(font);
        lineEditHost = new QLineEdit(centralWidget);
        lineEditHost->setObjectName(QString::fromUtf8("lineEditHost"));
        lineEditHost->setGeometry(QRect(50, 70, 261, 41));
        lineEditHost->setFont(font);
        label = new QLabel(centralWidget);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(50, 30, 241, 31));
        label->setFont(font);
        comboBox_grabScreenSize = new QComboBox(centralWidget);
        comboBox_grabScreenSize->setObjectName(QString::fromUtf8("comboBox_grabScreenSize"));
        comboBox_grabScreenSize->setGeometry(QRect(160, 130, 171, 51));
        QFont font1;
        font1.setFamily(QString::fromUtf8("\345\256\213\344\275\223"));
        font1.setPointSize(18);
        comboBox_grabScreenSize->setFont(font1);
        pushbtn_about = new QPushButton(centralWidget);
        pushbtn_about->setObjectName(QString::fromUtf8("pushbtn_about"));
        pushbtn_about->setGeometry(QRect(370, 200, 111, 71));
        pushbtn_about->setFont(font);
        clientStatusLabel_2 = new QLabel(centralWidget);
        clientStatusLabel_2->setObjectName(QString::fromUtf8("clientStatusLabel_2"));
        clientStatusLabel_2->setGeometry(QRect(50, 140, 181, 31));
        clientStatusLabel_2->setFont(font);
        checkBox_speed = new QCheckBox(centralWidget);
        checkBox_speed->setObjectName(QString::fromUtf8("checkBox_speed"));
        checkBox_speed->setGeometry(QRect(370, 30, 71, 16));
        disconnectButton = new QPushButton(centralWidget);
        disconnectButton->setObjectName(QString::fromUtf8("disconnectButton"));
        disconnectButton->setGeometry(QRect(420, 70, 91, 51));
        disconnectButton->setFont(font);
        groupBox = new QGroupBox(centralWidget);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setGeometry(QRect(340, 130, 171, 51));
        radioButton_smooth = new QRadioButton(groupBox);
        radioButton_smooth->setObjectName(QString::fromUtf8("radioButton_smooth"));
        radioButton_smooth->setGeometry(QRect(10, 10, 91, 41));
        radioButton_smooth->setFont(font);
        radioButton_smooth->setChecked(true);
        radioButton_highpix = new QRadioButton(groupBox);
        radioButton_highpix->setObjectName(QString::fromUtf8("radioButton_highpix"));
        radioButton_highpix->setGeometry(QRect(90, 10, 81, 41));
        radioButton_highpix->setFont(font);
        tcpClientFileSend->setCentralWidget(centralWidget);
        checkBox_speed->raise();
        startButton->raise();
        pauseButton->raise();
        openButton->raise();
        clientStatusLabel->raise();
        lineEditHost->raise();
        label->raise();
        comboBox_grabScreenSize->raise();
        pushbtn_about->raise();
        clientStatusLabel_2->raise();
        disconnectButton->raise();
        groupBox->raise();

        retranslateUi(tcpClientFileSend);

        QMetaObject::connectSlotsByName(tcpClientFileSend);
    } // setupUi

    void retranslateUi(QMainWindow *tcpClientFileSend)
    {
        tcpClientFileSend->setWindowTitle(QApplication::translate("tcpClientFileSend", "\346\241\214\351\235\242\344\274\240\350\276\223\347\263\273\347\273\237", 0, QApplication::UnicodeUTF8));
        startButton->setText(QApplication::translate("tcpClientFileSend", "\345\274\200\345\247\213", 0, QApplication::UnicodeUTF8));
        pauseButton->setText(QApplication::translate("tcpClientFileSend", "\346\232\202\345\201\234", 0, QApplication::UnicodeUTF8));
        openButton->setText(QApplication::translate("tcpClientFileSend", "\350\277\236\346\216\245", 0, QApplication::UnicodeUTF8));
        clientStatusLabel->setText(QApplication::translate("tcpClientFileSend", "\345\256\242\346\210\267\347\253\257\345\260\261\347\273\252", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("tcpClientFileSend", "\350\257\267\350\276\223\345\205\245\346\234\215\345\212\241\345\231\250IP\357\274\232", 0, QApplication::UnicodeUTF8));
        pushbtn_about->setText(QApplication::translate("tcpClientFileSend", "\345\205\263\344\272\216", 0, QApplication::UnicodeUTF8));
        clientStatusLabel_2->setText(QApplication::translate("tcpClientFileSend", "\345\210\206\350\276\250\347\216\207\357\274\232", 0, QApplication::UnicodeUTF8));
        checkBox_speed->setText(QApplication::translate("tcpClientFileSend", "\344\275\216\351\200\237\344\274\240\350\276\223", 0, QApplication::UnicodeUTF8));
        disconnectButton->setText(QApplication::translate("tcpClientFileSend", "\346\226\255\345\274\200", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QString());
        radioButton_smooth->setText(QApplication::translate("tcpClientFileSend", "\346\265\201\347\225\205", 0, QApplication::UnicodeUTF8));
        radioButton_highpix->setText(QApplication::translate("tcpClientFileSend", "\351\253\230\346\270\205", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class tcpClientFileSend: public Ui_tcpClientFileSend {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TCPCLIENTFILESEND_H
