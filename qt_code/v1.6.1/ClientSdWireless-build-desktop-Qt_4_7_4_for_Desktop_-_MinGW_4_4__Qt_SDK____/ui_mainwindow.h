/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created: Tue Jan 20 21:54:18 2015
**      by: Qt User Interface Compiler version 4.7.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QMainWindow>
#include <QtGui/QMenuBar>
#include <QtGui/QPushButton>
#include <QtGui/QSpinBox>
#include <QtGui/QStatusBar>
#include <QtGui/QToolBar>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QLabel *label;
    QPushButton *pushButton_start;
    QPushButton *pushButton_pause;
    QPushButton *pushButton_quit;
    QSpinBox *spinBox_ipadr1;
    QSpinBox *spinBox_ipadr2;
    QSpinBox *spinBox_ipadr3;
    QSpinBox *spinBox_ipadr4;
    QLineEdit *lineEdit;
    QLineEdit *lineEdit_2;
    QLineEdit *lineEdit_3;
    QLineEdit *lineEdit_4;
    QPushButton *pushButton_connect;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(400, 300);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        label = new QLabel(centralWidget);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(20, 10, 171, 31));
        QFont font;
        font.setFamily(QString::fromUtf8("Times New Roman"));
        font.setPointSize(12);
        label->setFont(font);
        pushButton_start = new QPushButton(centralWidget);
        pushButton_start->setObjectName(QString::fromUtf8("pushButton_start"));
        pushButton_start->setGeometry(QRect(40, 120, 91, 91));
        QFont font1;
        font1.setFamily(QString::fromUtf8("\345\271\274\345\234\206"));
        font1.setPointSize(24);
        pushButton_start->setFont(font1);
        pushButton_pause = new QPushButton(centralWidget);
        pushButton_pause->setObjectName(QString::fromUtf8("pushButton_pause"));
        pushButton_pause->setGeometry(QRect(150, 120, 91, 91));
        pushButton_pause->setFont(font1);
        pushButton_quit = new QPushButton(centralWidget);
        pushButton_quit->setObjectName(QString::fromUtf8("pushButton_quit"));
        pushButton_quit->setGeometry(QRect(270, 120, 91, 91));
        pushButton_quit->setFont(font1);
        spinBox_ipadr1 = new QSpinBox(centralWidget);
        spinBox_ipadr1->setObjectName(QString::fromUtf8("spinBox_ipadr1"));
        spinBox_ipadr1->setGeometry(QRect(40, 50, 71, 51));
        QFont font2;
        font2.setFamily(QString::fromUtf8("Times New Roman"));
        font2.setPointSize(20);
        spinBox_ipadr1->setFont(font2);
        spinBox_ipadr1->setMinimum(1);
        spinBox_ipadr1->setMaximum(255);
        spinBox_ipadr1->setValue(192);
        spinBox_ipadr2 = new QSpinBox(centralWidget);
        spinBox_ipadr2->setObjectName(QString::fromUtf8("spinBox_ipadr2"));
        spinBox_ipadr2->setGeometry(QRect(110, 50, 71, 51));
        spinBox_ipadr2->setFont(font2);
        spinBox_ipadr2->setMinimum(1);
        spinBox_ipadr2->setMaximum(255);
        spinBox_ipadr2->setValue(168);
        spinBox_ipadr3 = new QSpinBox(centralWidget);
        spinBox_ipadr3->setObjectName(QString::fromUtf8("spinBox_ipadr3"));
        spinBox_ipadr3->setGeometry(QRect(180, 50, 71, 51));
        spinBox_ipadr3->setFont(font2);
        spinBox_ipadr3->setMinimum(1);
        spinBox_ipadr3->setMaximum(255);
        spinBox_ipadr3->setValue(1);
        spinBox_ipadr4 = new QSpinBox(centralWidget);
        spinBox_ipadr4->setObjectName(QString::fromUtf8("spinBox_ipadr4"));
        spinBox_ipadr4->setGeometry(QRect(250, 50, 71, 51));
        spinBox_ipadr4->setFont(font2);
        spinBox_ipadr4->setMinimum(1);
        spinBox_ipadr4->setMaximum(255);
        spinBox_ipadr4->setValue(102);
        lineEdit = new QLineEdit(centralWidget);
        lineEdit->setObjectName(QString::fromUtf8("lineEdit"));
        lineEdit->setEnabled(false);
        lineEdit->setGeometry(QRect(90, 50, 21, 51));
        QFont font3;
        font3.setFamily(QString::fromUtf8("Times New Roman"));
        font3.setPointSize(26);
        lineEdit->setFont(font3);
        lineEdit->setAutoFillBackground(true);
        lineEdit_2 = new QLineEdit(centralWidget);
        lineEdit_2->setObjectName(QString::fromUtf8("lineEdit_2"));
        lineEdit_2->setEnabled(false);
        lineEdit_2->setGeometry(QRect(160, 50, 21, 51));
        lineEdit_2->setFont(font3);
        lineEdit_2->setAutoFillBackground(true);
        lineEdit_3 = new QLineEdit(centralWidget);
        lineEdit_3->setObjectName(QString::fromUtf8("lineEdit_3"));
        lineEdit_3->setEnabled(false);
        lineEdit_3->setGeometry(QRect(230, 50, 21, 51));
        lineEdit_3->setFont(font3);
        lineEdit_3->setAutoFillBackground(true);
        lineEdit_4 = new QLineEdit(centralWidget);
        lineEdit_4->setObjectName(QString::fromUtf8("lineEdit_4"));
        lineEdit_4->setEnabled(false);
        lineEdit_4->setGeometry(QRect(300, 50, 21, 51));
        lineEdit_4->setFont(font3);
        lineEdit_4->setAutoFillBackground(true);
        pushButton_connect = new QPushButton(centralWidget);
        pushButton_connect->setObjectName(QString::fromUtf8("pushButton_connect"));
        pushButton_connect->setGeometry(QRect(300, 20, 91, 91));
        pushButton_connect->setFont(font1);
        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 400, 23));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        MainWindow->setStatusBar(statusBar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("MainWindow", "\350\257\267\350\277\236\346\216\245\346\234\215\345\212\241\345\231\250IP\345\234\260\345\235\200\357\274\232", 0, QApplication::UnicodeUTF8));
        pushButton_start->setText(QApplication::translate("MainWindow", "\345\274\200\345\247\213", 0, QApplication::UnicodeUTF8));
        pushButton_pause->setText(QApplication::translate("MainWindow", "\346\232\202\345\201\234", 0, QApplication::UnicodeUTF8));
        pushButton_quit->setText(QApplication::translate("MainWindow", "\351\200\200\345\207\272", 0, QApplication::UnicodeUTF8));
        lineEdit->setText(QApplication::translate("MainWindow", ".", 0, QApplication::UnicodeUTF8));
        lineEdit_2->setText(QApplication::translate("MainWindow", ".", 0, QApplication::UnicodeUTF8));
        lineEdit_3->setText(QApplication::translate("MainWindow", ".", 0, QApplication::UnicodeUTF8));
        lineEdit_4->setText(QString());
        pushButton_connect->setText(QApplication::translate("MainWindow", "\350\277\236\346\216\245", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
