//
// Created by User on 2023/08/14.
//

#ifndef WAVE_FUNCTION_COLLAPSE_UI_SUDOKUSCENE_H
#define WAVE_FUNCTION_COLLAPSE_UI_SUDOKUSCENE_H


#include <QGraphicsScene>
#include <QWidget>
#include <QGraphicsWidget>
#include <QGraphicsGridLayout>
#include "AbstractScene.h"
#include "../graphics_items/GridTile.h"
#include "../models/SudokuBlock.h"
#include "../models/SudokuBoard.h"
#include "../graphics_items/BoardContainer.h"
#include "../widgets/ButtonGroup.h"
#include "../ui_forms/sudokuscenesidemenu.h"
#include <QGroupBox>
#include <QGraphicsProxyWidget>
#include <QThread>
#include <QGraphicsLinearLayout>

class SudokuScene : public AbstractScene
{
    Q_OBJECT
public:
    explicit SudokuScene(const std::string& filename, QWidget* parent = nullptr);
public slots:
    void animate() override;
    void reset();
    void generate();
    void enableBacktracking(bool isBacktrackingEnabled);
    void showSuperpositions(bool shouldShowSuperpositions);
    void setBacktrackCount(int count);
    void setRetriesCount(int count);
private:
    void init();
    void arrangeItems();
private:
    std::vector<std::vector<GridTile*>> m_grid_ui;
    SudokuBoard* m_board;
    int m_retries_count;
    static int s_animate_count;
    BoardContainer* m_sudoku_grid;
    QGraphicsProxyWidget* m_sudoku_menu_proxy;
    SudokuSceneSideMenu* m_sudoku_menu;
    QGraphicsLinearLayout* m_scene_layout;
    QGraphicsWidget* m_scene_container;
    QThread* m_animation_thread;
    QThread* m_reset_thread;
};


#endif //WAVE_FUNCTION_COLLAPSE_UI_SUDOKUSCENE_H
