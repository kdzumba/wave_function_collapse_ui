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

class SudokuScene : public AbstractScene
{
public:
    explicit SudokuScene(const std::string& filename, QWidget* parent = nullptr);
    void animate() override;
private:
    void init();
    void arrangeItems();
private:
    std::vector<std::vector<GridTile*>> m_grid_ui;
    SudokuBoard* m_board;
    static int constexpr BOARD_SIZE = 9;
    QList<QGraphicsGridLayout*> m_layouts;
    QList<QGraphicsWidget*> m_containers;
};


#endif //WAVE_FUNCTION_COLLAPSE_UI_SUDOKUSCENE_H
