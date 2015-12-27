/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.3.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *importMetaTedsAction;
    QAction *importPhyTedsAction;
    QAction *importTransducerTedsAction;
    QAction *importTransducerTedsAction2;
    QAction *importTransducerTedsAction3;
    QAction *actionGetArg;
    QWidget *centralWidget;
    QPlainTextEdit *textReceived;
    QPlainTextEdit *textSended;
    QLabel *label;
    QLabel *label_2;
    QLabel *statusBar;
    QPushButton *buttonWrite;
    QPushButton *buttonWritePhy;
    QPushButton *buttonWriteTs;
    QPushButton *buttonWriteTeds;
    QPushButton *buttonQuery;
    QMenuBar *menuBar;
    QMenu *menu_TEDS;
    QMenu *menuSysArg;
    QToolBar *mainToolBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(811, 514);
        importMetaTedsAction = new QAction(MainWindow);
        importMetaTedsAction->setObjectName(QStringLiteral("importMetaTedsAction"));
        importPhyTedsAction = new QAction(MainWindow);
        importPhyTedsAction->setObjectName(QStringLiteral("importPhyTedsAction"));
        importTransducerTedsAction = new QAction(MainWindow);
        importTransducerTedsAction->setObjectName(QStringLiteral("importTransducerTedsAction"));
        importTransducerTedsAction2 = new QAction(MainWindow);
        importTransducerTedsAction2->setObjectName(QStringLiteral("importTransducerTedsAction2"));
        importTransducerTedsAction3 = new QAction(MainWindow);
        importTransducerTedsAction3->setObjectName(QStringLiteral("importTransducerTedsAction3"));
        actionGetArg = new QAction(MainWindow);
        actionGetArg->setObjectName(QStringLiteral("actionGetArg"));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        textReceived = new QPlainTextEdit(centralWidget);
        textReceived->setObjectName(QStringLiteral("textReceived"));
        textReceived->setGeometry(QRect(330, 260, 451, 211));
        textSended = new QPlainTextEdit(centralWidget);
        textSended->setObjectName(QStringLiteral("textSended"));
        textSended->setGeometry(QRect(330, 70, 451, 111));
        label = new QLabel(centralWidget);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(230, 80, 54, 12));
        label_2 = new QLabel(centralWidget);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(230, 270, 54, 12));
        statusBar = new QLabel(centralWidget);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        statusBar->setGeometry(QRect(30, 460, 771, 20));
        buttonWrite = new QPushButton(centralWidget);
        buttonWrite->setObjectName(QStringLiteral("buttonWrite"));
        buttonWrite->setGeometry(QRect(60, 320, 141, 23));
        buttonWritePhy = new QPushButton(centralWidget);
        buttonWritePhy->setObjectName(QStringLiteral("buttonWritePhy"));
        buttonWritePhy->setGeometry(QRect(60, 350, 141, 23));
        buttonWriteTs = new QPushButton(centralWidget);
        buttonWriteTs->setObjectName(QStringLiteral("buttonWriteTs"));
        buttonWriteTs->setGeometry(QRect(60, 380, 141, 23));
        buttonWriteTeds = new QPushButton(centralWidget);
        buttonWriteTeds->setObjectName(QStringLiteral("buttonWriteTeds"));
        buttonWriteTeds->setGeometry(QRect(60, 410, 141, 23));
        buttonQuery = new QPushButton(centralWidget);
        buttonQuery->setObjectName(QStringLiteral("buttonQuery"));
        buttonQuery->setGeometry(QRect(60, 440, 141, 23));
        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 811, 20));
        menu_TEDS = new QMenu(menuBar);
        menu_TEDS->setObjectName(QStringLiteral("menu_TEDS"));
        menuSysArg = new QMenu(menuBar);
        menuSysArg->setObjectName(QStringLiteral("menuSysArg"));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);

        menuBar->addAction(menu_TEDS->menuAction());
        menuBar->addAction(menuSysArg->menuAction());
        menu_TEDS->addAction(importMetaTedsAction);
        menu_TEDS->addAction(importPhyTedsAction);
        menu_TEDS->addAction(importTransducerTedsAction);
        menuSysArg->addAction(actionGetArg);

        retranslateUi(MainWindow);
        QObject::connect(importMetaTedsAction, SIGNAL(triggered()), MainWindow, SLOT(importMetaTedsAction()));
        QObject::connect(importPhyTedsAction, SIGNAL(triggered()), MainWindow, SLOT(importPhyTedsAction()));
        QObject::connect(importTransducerTedsAction, SIGNAL(triggered()), MainWindow, SLOT(importTransducerTedsAction()));
        QObject::connect(buttonWrite, SIGNAL(clicked()), MainWindow, SLOT(buttonWriteData()));
        QObject::connect(buttonWritePhy, SIGNAL(clicked()), MainWindow, SLOT(buttonWritePhy()));
        QObject::connect(buttonWriteTs, SIGNAL(clicked()), MainWindow, SLOT(buttonWriteTs()));
        QObject::connect(buttonWriteTeds, SIGNAL(clicked()), MainWindow, SLOT(buttonWriteTeds()));
        QObject::connect(buttonQuery, SIGNAL(clicked()), MainWindow, SLOT(queryTeds()));
        QObject::connect(actionGetArg, SIGNAL(triggered()), MainWindow, SLOT(getSysArgs()));

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", 0));
        importMetaTedsAction->setText(QApplication::translate("MainWindow", "\350\257\273\345\217\226Meta-TEDS", 0));
        importPhyTedsAction->setText(QApplication::translate("MainWindow", "\350\257\273\345\217\226Phy-TEDS", 0));
        importTransducerTedsAction->setText(QApplication::translate("MainWindow", "\350\257\273\345\217\226Transducer-TEDS", 0));
        importTransducerTedsAction2->setText(QApplication::translate("MainWindow", "\350\257\273\345\217\226Transducer-TEDS2", 0));
        importTransducerTedsAction3->setText(QApplication::translate("MainWindow", "\350\257\273\345\217\226Transducer-TEDS3", 0));
        actionGetArg->setText(QApplication::translate("MainWindow", "getArg", 0));
        label->setText(QApplication::translate("MainWindow", "send", 0));
        label_2->setText(QApplication::translate("MainWindow", "receive", 0));
        statusBar->setText(QApplication::translate("MainWindow", "status", 0));
        buttonWrite->setText(QApplication::translate("MainWindow", "write Meta TEDS", 0));
        buttonWritePhy->setText(QApplication::translate("MainWindow", "write PHY TEDS", 0));
        buttonWriteTs->setText(QApplication::translate("MainWindow", "write TS TEDS", 0));
        buttonWriteTeds->setText(QApplication::translate("MainWindow", "write  TEDS", 0));
        buttonQuery->setText(QApplication::translate("MainWindow", "query command", 0));
        menu_TEDS->setTitle(QApplication::translate("MainWindow", "\345\212\237\350\203\275", 0));
        menuSysArg->setTitle(QApplication::translate("MainWindow", "SysArg", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
