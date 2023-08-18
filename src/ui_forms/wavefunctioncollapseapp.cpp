//
// Created by User on 2023/08/14.
//

// You may need to build the project (run Qt uic code generator) to get "ui_WaveFunctionCollapseApp.h" resolved

#include "wavefunctioncollapseapp.h"
#include "ui_WaveFunctionCollapseApp.h"
#include <QLayout>

WaveFunctionCollapseApp::WaveFunctionCollapseApp(QWidget *parent) : QMainWindow(parent),
ui(new Ui::WaveFunctionCollapseApp)
{
    m_workspace = new GraphicsWorkspace(this);
    this ->setWindowTitle("Wave Function Collapse");
    ui->setupUi(this);
}

WaveFunctionCollapseApp::~WaveFunctionCollapseApp()
{
    delete ui;
    delete m_workspace;
}

void WaveFunctionCollapseApp::run()
{
    this -> showMaximized();
}
