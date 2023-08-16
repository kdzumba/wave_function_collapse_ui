/********************************************************************************
** Form generated from reading UI file 'graphicsworkspace.ui'
**
** Created by: Qt User Interface Compiler version 6.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GRAPHICSWORKSPACE_H
#define UI_GRAPHICSWORKSPACE_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_GraphicsWorkspace
{
public:

    void setupUi(QWidget *GraphicsWorkspace)
    {
        if (GraphicsWorkspace->objectName().isEmpty())
            GraphicsWorkspace->setObjectName("GraphicsWorkspace");
        GraphicsWorkspace->resize(1419, 849);

        retranslateUi(GraphicsWorkspace);

        QMetaObject::connectSlotsByName(GraphicsWorkspace);
    } // setupUi

    void retranslateUi(QWidget *GraphicsWorkspace)
    {
        GraphicsWorkspace->setWindowTitle(QCoreApplication::translate("GraphicsWorkspace", "GraphicsWorkspace", nullptr));
    } // retranslateUi

};

namespace Ui {
    class GraphicsWorkspace: public Ui_GraphicsWorkspace {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GRAPHICSWORKSPACE_H
