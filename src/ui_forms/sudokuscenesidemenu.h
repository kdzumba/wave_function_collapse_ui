//
// Created by User on 2023/08/14.
//

#ifndef WAVE_FUNCTION_COLLAPSE_UI_SUDOKUSCENESIDEMENU_H
#define WAVE_FUNCTION_COLLAPSE_UI_SUDOKUSCENESIDEMENU_H

#include <QFrame>
#include "../AbstractSideMenu.h"
#include "../widgets/ButtonGroup.h"


QT_BEGIN_NAMESPACE
namespace Ui { class SudokuSceneSideMenu; }
QT_END_NAMESPACE

class SudokuSceneSideMenu : public QWidget
{
Q_OBJECT

public:
    explicit SudokuSceneSideMenu(QWidget *parent = nullptr);
    ~SudokuSceneSideMenu() override;

private:
    Ui::SudokuSceneSideMenu *ui;
    ButtonGroup* m_controls_button_group;
};


#endif //WAVE_FUNCTION_COLLAPSE_UI_SUDOKUSCENESIDEMENU_H
