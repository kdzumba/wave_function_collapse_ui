#include <QApplication>
#include <QPushButton>
#include "src/ui_forms/wavefunctioncollapseapp.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    auto app = new WaveFunctionCollapseApp;
    app -> run();
    return QApplication::exec();
}
