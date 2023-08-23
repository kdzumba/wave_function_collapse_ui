//
// Created by User on 2023/08/23.
//

#ifndef WAVE_FUNCTION_COLLAPSE_UI_CELLGRAPHICSITEM_H
#define WAVE_FUNCTION_COLLAPSE_UI_CELLGRAPHICSITEM_H

#include <QGraphicsPixmapItem>
#include "../models/Cell.h"

class CellGraphicsItem : public QGraphicsPixmapItem
{
public:
    CellGraphicsItem(Cell* model, int x, int y);
private:
    std::pair<int, int> m_position;
    Cell* m_cell_model;
};


#endif //WAVE_FUNCTION_COLLAPSE_UI_CELLGRAPHICSITEM_H
