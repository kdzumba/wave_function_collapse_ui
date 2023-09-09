//
// Created by User on 2023/08/14.
//

#ifndef WAVE_FUNCTION_COLLAPSE_UI_SUDOKUSCENESIDEMENU_H
#define WAVE_FUNCTION_COLLAPSE_UI_SUDOKUSCENESIDEMENU_H

#include <QFrame>
#include "../AbstractSideMenu.h"
#include "../widgets/ButtonGroup.h"
#include "../widgets/SliderGroup.h"


QT_BEGIN_NAMESPACE
namespace Ui { class SudokuSceneSideMenu; }
QT_END_NAMESPACE

class SudokuSceneSideMenu : public AbstractSideMenu
{
Q_OBJECT

public:
    explicit SudokuSceneSideMenu(QWidget *parent = nullptr);
    ~SudokuSceneSideMenu() override;
private slots:
    void handleSolveButtonClicked();
    void handleResetButtonClicked();
    void handleGenerateButtonClicked();
    void handleBacktrackToggled(bool isSelected);
    void handleSuperpositionsToggled(bool isSelected);
    void handleBacktrackCountChanged(int value);
    void handleRetriesCountChanged(int value);
signals:
    void solveButtonClicked();
    void resetButtonClicked();
    void generateButtonClicked();
    void backtrackOptionSelected(bool isSelected);
    void showSuperpositionsSelected(bool isSelected);
    void backtrackCountChanged(int value);
    void retriesCountChanged(int value);

private:
    Ui::SudokuSceneSideMenu *ui;
    ButtonGroup* m_controls_button_group;
    SliderGroup* m_controls_slider_group;
};


#endif //WAVE_FUNCTION_COLLAPSE_UI_SUDOKUSCENESIDEMENU_H
