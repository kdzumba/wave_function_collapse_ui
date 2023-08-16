/********************************************************************************
** Form generated from reading UI file 'wavefunctioncollapseapp.ui'
**
** Created by: Qt User Interface Compiler version 6.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WAVEFUNCTIONCOLLAPSEAPP_H
#define UI_WAVEFUNCTIONCOLLAPSEAPP_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_WaveFunctionCollapseApp
{
public:
    QWidget *centralwidget;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *WaveFunctionCollapseApp)
    {
        if (WaveFunctionCollapseApp->objectName().isEmpty())
            WaveFunctionCollapseApp->setObjectName("WaveFunctionCollapseApp");
        WaveFunctionCollapseApp->resize(400, 300);
        centralwidget = new QWidget(WaveFunctionCollapseApp);
        centralwidget->setObjectName("centralwidget");
        WaveFunctionCollapseApp->setCentralWidget(centralwidget);
        menubar = new QMenuBar(WaveFunctionCollapseApp);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 400, 17));
        WaveFunctionCollapseApp->setMenuBar(menubar);
        statusbar = new QStatusBar(WaveFunctionCollapseApp);
        statusbar->setObjectName("statusbar");
        WaveFunctionCollapseApp->setStatusBar(statusbar);

        retranslateUi(WaveFunctionCollapseApp);

        QMetaObject::connectSlotsByName(WaveFunctionCollapseApp);
    } // setupUi

    void retranslateUi(QMainWindow *WaveFunctionCollapseApp)
    {
        WaveFunctionCollapseApp->setWindowTitle(QCoreApplication::translate("WaveFunctionCollapseApp", "WaveFunctionCollapseApp", nullptr));
    } // retranslateUi

};

namespace Ui {
    class WaveFunctionCollapseApp: public Ui_WaveFunctionCollapseApp {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WAVEFUNCTIONCOLLAPSEAPP_H
