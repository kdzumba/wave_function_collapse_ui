//
// Created by User on 2023/08/15.
//

#ifndef WAVE_FUNCTION_COLLAPSE_UI_GRIDTILE_H
#define WAVE_FUNCTION_COLLAPSE_UI_GRIDTILE_H

#include <QGraphicsItem>
#include <QLabel>
#include <QWidget>
#include <QRectF>
#include <QPainter>
#include <QWidget>
#include <QStyleOptionGraphicsItem>
#include <QJsonObject>
#include <QGraphicsLayoutItem>
#include "../models/SudokuBlock.h"
#include <QGraphicsTextItem>
#include <QGraphicsGridLayout>
#include <QGraphicsProxyWidget>

class GridTile : public QObject, public QGraphicsLayoutItem, public QGraphicsItem
{
    Q_OBJECT
    struct GridTileStyle
    {
        QString tile_hovered_color;
        QString tile_hovered_font_color;
        QString tile_unhovered_font_color;
    };

public:
    GridTile(SudokuBlock* model, int x, int y);
    ~GridTile() override;
    [[nodiscard]] QRectF boundingRect() const override;
    void paint(QPainter* painter, const QStyleOptionGraphicsItem * option, QWidget* widget) override;
    void setGeometry(const QRectF& geometry) override;
    QSizeF sizeHint(Qt::SizeHint which, const QSizeF &constraint) const override;
public slots:
    void advance(int step) override;
public:
    static int constexpr TILE_SIZE = 80;
    static int constexpr POSITION_OFFSET = 10;
    static int constexpr PEN_WIDTH = 2;
    static int constexpr STATES_LABEL_SIZE = TILE_SIZE / 3;
private:
    QLabel* m_value;
    QList<QList<QLabel*>> m_available_states;
    GridTileStyle m_style;
    std::pair<int, int> m_position;
    bool m_is_permanently_collapsed;
    SudokuBlock* m_tile_model;
public:
    static int s_advance_call_count;
};


#endif //WAVE_FUNCTION_COLLAPSE_UI_GRIDTILE_H
