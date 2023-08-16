//
// Created by User on 2023/08/14.
//

#ifndef WAVE_FUNCTION_COLLAPSE_UI_SUDOKUSCENESIDEMENU_H
#define WAVE_FUNCTION_COLLAPSE_UI_SUDOKUSCENESIDEMENU_H

#include <QFrame>
#include "../AbstractSideMenu.h"


QT_BEGIN_NAMESPACE
namespace Ui { class SudokuSceneSideMenu; }
QT_END_NAMESPACE

class SudokuSceneSideMenu : public QFrame, public AbstractSideMenu
{
Q_OBJECT

public:
    explicit SudokuSceneSideMenu(QWidget *parent = nullptr);
    void activate() override;
    ~SudokuSceneSideMenu() override;

private:
    Ui::SudokuSceneSideMenu *ui;
};


#endif //WAVE_FUNCTION_COLLAPSE_UI_SUDOKUSCENESIDEMENU_H
