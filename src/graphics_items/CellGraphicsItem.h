//
// Created by User on 2023/08/23.
//

#ifndef WAVE_FUNCTION_COLLAPSE_UI_CELLGRAPHICSITEM_H
#define WAVE_FUNCTION_COLLAPSE_UI_CELLGRAPHICSITEM_H

#include <QGraphicsPixmapItem>
#include <QGraphicsLayoutItem>
#include "../models/Cell.h"
#include <QObject>
#include <QPainter>
#include<QWidget>

class CellGraphicsItem : public QObject, public QGraphicsPixmapItem, public QGraphicsLayoutItem
{   Q_OBJECT
public:
    CellGraphicsItem(Cell* model, int x, int y);
    void paint(QPainter* painter, const QStyleOptionGraphicsItem * option, QWidget* widget) override;
    void setGeometry(const QRectF& geometry) override;
    QSizeF sizeHint(Qt::SizeHint which, const QSizeF &constraint) const override;
    QRectF boundingRect() const override;
public slots:
    void advance(int step) override;
private:
    static int constexpr IMAGE_WIDTH = 14;
    static int constexpr IMAGE_HEIGHT = 14;
    Cell* m_cell_model;
};


#endif //WAVE_FUNCTION_COLLAPSE_UI_CELLGRAPHICSITEM_H
