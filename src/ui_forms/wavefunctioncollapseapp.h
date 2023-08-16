//
// Created by User on 2023/08/14.
//

#ifndef WAVE_FUNCTION_COLLAPSE_UI_WAVEFUNCTIONCOLLAPSEAPP_H
#define WAVE_FUNCTION_COLLAPSE_UI_WAVEFUNCTIONCOLLAPSEAPP_H

#include <QMainWindow>
#include "graphicsworkspace.h"
#include "../AbstractSideMenu.h"


QT_BEGIN_NAMESPACE
namespace Ui { class WaveFunctionCollapseApp; }
QT_END_NAMESPACE

class WaveFunctionCollapseApp : public QMainWindow
{
    Q_OBJECT
public:
    explicit WaveFunctionCollapseApp(QWidget *parent = nullptr);
    ~WaveFunctionCollapseApp() override;
    void run();

private:
    Ui::WaveFunctionCollapseApp *ui;
    std::vector<AbstractSideMenu*> m_menus;
    GraphicsWorkspace* m_workspace;
};


#endif //WAVE_FUNCTION_COLLAPSE_UI_WAVEFUNCTIONCOLLAPSEAPP_H
