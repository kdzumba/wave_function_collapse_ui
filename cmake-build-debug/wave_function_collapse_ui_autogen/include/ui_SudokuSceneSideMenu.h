/********************************************************************************
** Form generated from reading UI file 'sudokuscenesidemenu.ui'
**
** Created by: Qt User Interface Compiler version 6.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SUDOKUSCENESIDEMENU_H
#define UI_SUDOKUSCENESIDEMENU_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SudokuSceneSideMenu
{
public:

    void setupUi(QWidget *SudokuSceneSideMenu)
    {
        if (SudokuSceneSideMenu->objectName().isEmpty())
            SudokuSceneSideMenu->setObjectName("SudokuSceneSideMenu");
        SudokuSceneSideMenu->resize(400, 300);

        retranslateUi(SudokuSceneSideMenu);

        QMetaObject::connectSlotsByName(SudokuSceneSideMenu);
    } // setupUi

    void retranslateUi(QWidget *SudokuSceneSideMenu)
    {
        SudokuSceneSideMenu->setWindowTitle(QCoreApplication::translate("SudokuSceneSideMenu", "SudokuSceneSideMenu", nullptr));
    } // retranslateUi

};

namespace Ui {
    class SudokuSceneSideMenu: public Ui_SudokuSceneSideMenu {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SUDOKUSCENESIDEMENU_H
